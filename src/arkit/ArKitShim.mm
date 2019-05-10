#include "tp_ar/arkit/ArKitShim.h"
#include "tp_ar/Frame.h"

#include "tp_maps/Map.h"

#include "tp_utils/DebugUtils.h"
#include "tp_utils/MutexUtils.h"
#include "tp_utils/TimeUtils.h"

#include "glm/gtc/type_ptr.hpp"
#include "glm/gtx/transform.hpp"
#include "glm/gtx/rotate_normalized_axis.hpp"

#import <ARKit/ARKit.h>

@interface ArKitShimPrivate : NSObject<ARSessionDelegate>
{}
@property (nonatomic, assign)tp_ar::ArKitShim::Private* d;
- (void)restartSession;
@end

namespace tp_ar
{

//##################################################################################################
struct ArKitShim::Private
{
  std::function<void(Frame&)> frameReceivedCallback;
  tp_maps::Map* map;

  ARSession* session NS_AVAILABLE_IOS(12_0){nullptr};
  ArKitShimPrivate* delegate{nullptr};

  TPMutex mutex{TPM};
  tp_ar::Frame frame;
  size_t frameW{0};
  size_t frameH{0};
  size_t bytesPerRow{0};
  std::vector<uint8_t> data;

  TPMutex imageScaleMutex{TPM};
  glm::vec2 imageScale{1.0f, 1.0f};

  TPMutex rgbDataMutex{TPM};
  std::vector<uint8_t> dataCopy;
  std::vector<TPPixel> rgbData;

  //################################################################################################
  Private(const std::function<void(Frame&)>& frameReceivedCallback_, tp_maps::Map* map_):
    frameReceivedCallback(frameReceivedCallback_),
    map(map_)
  {

  }
};

//##################################################################################################
ArKitShim::ArKitShim(const std::function<void(Frame&)>& frameReceivedCallback, tp_maps::Map* map):
  d(new Private(frameReceivedCallback, map))
{
  if(@available(iOS 12, *))
  {
    d->delegate = [[ArKitShimPrivate alloc] init];
    [d->delegate setD: d];
    d->session = [[ARSession alloc] init];
    [d->session setDelegate: d->delegate];
    [d->delegate restartSession];
  }
}

//##################################################################################################
ArKitShim::~ArKitShim()
{
  if(@available(iOS 12, *))
  {
    [d->session release];
    [d->delegate release];
  }
  delete d;
}

//##################################################################################################
void ArKitShim::viewFrame(const std::function<void(const tp_ar::Frame&)>& closure)
{
  TP_MUTEX_LOCKER(d->mutex);
  closure(d->frame);
}
//##################################################################################################
void ArKitShim::viewFrameData(const std::function<void(const tp_ar::Frame& frame, size_t w, size_t h, const TPPixel* data)>& closure)
{
  TP_MUTEX_LOCKER(d->rgbDataMutex);

  //We take a copy of the data here to avoid blocking the render path.
  size_t w{0};
  size_t h{0};
  tp_ar::Frame frame = d->frame;
  {
    TP_MUTEX_LOCKER(d->mutex);
    w = d->frameW;
    h = d->frameH;
    d->dataCopy = d->data;
  }

  if(w<1 || h<1)
    return;

  //We need to rotate the image to get it correctly orientated to the device.
  auto frameW = h;
  auto frameH = w;

  d->rgbData.resize(w*h);

  auto bytesPerRow = w*4;

  size_t yMax = h;
  for(size_t y=0; y<yMax; y++)
  {
    auto dstX = (yMax-1)-y;
    const uint8_t* src = d->dataCopy.data() + (bytesPerRow * y);
    const uint8_t* srcMax = src + bytesPerRow;
    for(size_t x=w-1; src<srcMax; src+=4, x--)
    {
      double Y  = double(src[0]);
      double Cb = double(src[1]);
      double Cr = double(src[2]);

      auto dstY = (frameH-1)-x;
      auto& p = d->rgbData.at((dstY*frameW)+dstX);
      p.r = uint8_t(tpBound(0, int(Y + 1.40200 * (Cr - 0x80)), 255));
      p.g = uint8_t(tpBound(0, int(Y - 0.34414 * (Cb - 0x80) - 0.71414 * (Cr - 0x80)), 255));
      p.b = uint8_t(tpBound(0, int(Y + 1.77200 * (Cb - 0x80)), 255));
      p.a = 255;
    }
  }

  closure(frame, frameW, frameH, d->rgbData.data());
}

//##################################################################################################
void ArKitShim::viewYCbCr(const std::function<void(size_t w, size_t h, const std::vector<uint8_t>& data)>& closure)
{
  TP_MUTEX_LOCKER(d->mutex);
  closure(d->frameW, d->frameH, d->data);
}

//##################################################################################################
glm::vec2 ArKitShim::imageScale()
{
  TP_MUTEX_LOCKER(d->imageScaleMutex);
  return d->imageScale;
}

}

//##################################################################################################
@implementation ArKitShimPrivate
{
  tp_ar::ArKitShim::Private* d;
}

//##################################################################################################
@synthesize d = d;

//##################################################################################################
- (void)restartSession
{
  if(@available(iOS 12, *))
  {
    [[self d]->session pause];
    [[self d]->session runWithConfiguration: [[ARWorldTrackingConfiguration alloc] init]];
  }
}

#pragma mark ARSessionDelegate

//##################################################################################################
- (void)session:(ARSession *)session didUpdateFrame:(ARFrame *)frame NS_AVAILABLE_IOS(12_0)
{
  TP_UNUSED(session);
  TP_UNUSED(frame);
  auto i = [frame capturedImage];

  CVPixelBufferLockBaseAddress(i, kCVPixelBufferLock_ReadOnly);

  TP_MUTEX_LOCKER([self d]->mutex);

  [self d]->frameW = CVPixelBufferGetWidth(i);
  [self d]->frameH = CVPixelBufferGetHeight(i);
  [self d]->bytesPerRow = [self d]->frameW*4;

  {
    glm::mat4 arTransform;
    {
      simd_float4x4 transform = [[frame camera] transform];
      memcpy(glm::value_ptr(arTransform), &transform, sizeof(float)*(4*4));
    }

    {
      //Rotate to align with the orientation of the screen.
      glm::mat4 cameraTransform = glm::mat4(1.0f);
      cameraTransform = glm::rotateNormalizedAxis(arTransform, glm::radians(90.0f), {0.0f, 0.0f, 1.0f});

      //In our world z is up, in ArKit y is up. Rotate the scene to fix this.
      glm::mat4 sceneTransform(1.0f);
      sceneTransform = glm::rotateNormalizedAxis(glm::mat4(1.0f), glm::radians(270.0f), {1.0f, 0.0f, 0.0f});
      sceneTransform = glm::inverse(sceneTransform);

      [self d]->frame.cameraTransformation = sceneTransform * cameraTransform;
    }
  }

  {
    CGSize viewportSize;
    viewportSize.width  = [self d]->map->width();
    viewportSize.height = [self d]->map->height();

    //Swap x and y
    CGSize imageSize;
    imageSize.width  = [self d]->frameH;
    imageSize.height = [self d]->frameW;

    {
      glm::mat4 arProjection;
      simd_float4x4 transform = [[frame camera] projectionMatrixForOrientation: UIInterfaceOrientationPortrait viewportSize:viewportSize zNear: 0.001 zFar: 1000.0];
      memcpy(glm::value_ptr(arProjection), &transform, sizeof(float)*(4*4));
      [self d]->frame.cameraCallibration = arProjection;
    }

    if(imageSize.width>0 && imageSize.height>0)
    {
      float sx = float(imageSize.width ) / float(viewportSize.width );
      float sy = float(imageSize.height) / float(viewportSize.height);

      if(float s = tpMin(sx, sy); s>0.00001f)
      {
        TP_MUTEX_LOCKER([self d]->imageScaleMutex);
        [self d]->imageScale.x = sx/s;
        [self d]->imageScale.y = sy/s;
      }
    }
  }

  void* pY    = CVPixelBufferGetBaseAddressOfPlane(i, 0);
  void* pCbCr = CVPixelBufferGetBaseAddressOfPlane(i, 1);

  size_t strideY    = CVPixelBufferGetBytesPerRowOfPlane(i, 0);
  size_t strideCbCr = CVPixelBufferGetBytesPerRowOfPlane(i, 1);

  size_t sizeInBytes = [self d]->frameH * [self d]->bytesPerRow;
  [self d]->data.resize(sizeInBytes);

  const size_t byteWidth = ([self d]->frameW * 4);
  const size_t yMax = [self d]->frameH;
  for(size_t y=0; y<yMax; y++)
  {
    size_t j = y/2;
    uint8_t* dst     = [self d]->data.data() + (y*[self d]->bytesPerRow);
    uint8_t* srcY    = static_cast<uint8_t*>(pY   ) + (y*strideY   );
    uint8_t* srcCbCr = static_cast<uint8_t*>(pCbCr) + (j*strideCbCr);

    uint8_t* dstMax = dst + byteWidth;

    for(; dst<dstMax; dst+=8, srcY+=2, srcCbCr+=2)
    {
      dst[0] = srcY[0];
      dst[1] = srcCbCr[0];
      dst[2] = srcCbCr[1];
      dst[3] = 255;

      dst[4] = srcY[1];
      dst[5] = srcCbCr[0];
      dst[6] = srcCbCr[1];
      dst[7] = 255;
    }
  }

  [self d]->frameReceivedCallback([self d]->frame);
}

#pragma mark - ARSessionObserver

//##################################################################################################
- (void)session:(ARSession *)session didFailWithError:(NSError *)error NS_AVAILABLE_IOS(12_0)
{
  TP_UNUSED(session);
  std::string msg = [[error description] UTF8String];
  tpWarning() << "ArKitShim::didFailWithError:\n" << msg;
  [self restartSession];
}

//##################################################################################################
- (void)sessionWasInterrupted:(ARSession *)session NS_AVAILABLE_IOS(12_0)
{
  TP_UNUSED(session);
  tpWarning() << "ArKitShim::sessionWasInterrupted";
}

//##################################################################################################
- (void)sessionInterruptionEnded:(ARSession *)session NS_AVAILABLE_IOS(12_0)
{
  TP_UNUSED(session);
  tpWarning() << "ArKitShim::sessionInterruptionEnded";
}

@end
