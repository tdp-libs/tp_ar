#include "tp_ar/arkit/ArKitShim.h"
#include "tp_ar/Frame.h"

#include "tp_utils/DebugUtils.h"
#include "tp_utils/MutexUtils.h"

#import <ARKit/ARKit.h>

@interface ArKitShimPrivate : NSObject<ARSessionDelegate>
{

}
@property (nonatomic, assign)tp_ar::ArKitShim::Private* d;
- (void)restartSession;
@end

namespace tp_ar
{

//##################################################################################################
struct ArKitShim::Private
{
  std::function<void(const Frame&)> frameReceivedCallback;

  ARSession* session NS_AVAILABLE_IOS(12_0){nullptr};
  ArKitShimPrivate* delegate{nullptr};    

  TPMutex mutex{TPM};
  tp_ar::Frame frame;
  std::vector<uint8_t> data;

  //################################################################################################
  Private(const std::function<void(const Frame&)>& frameReceivedCallback_):
    frameReceivedCallback(frameReceivedCallback_)
  {

  }
};

//##################################################################################################
ArKitShim::ArKitShim(const std::function<void(const Frame&)>& frameReceivedCallback):
  d(new Private(frameReceivedCallback))
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

  TP_MUTEX_LOCKER(d->mutex);

  [self d]->frame.w = CVPixelBufferGetWidth(i);
  [self d]->frame.h = CVPixelBufferGetHeight(i);
  [self d]->frame.bytesPerRow = [self d]->frame.w*4;

  void* pY    = CVPixelBufferGetBaseAddressOfPlane(i, 0);
  void* pCbCr = CVPixelBufferGetBaseAddressOfPlane(i, 1);

  size_t strideY    = CVPixelBufferGetBytesPerRowOfPlane(i, 0);
  size_t strideCbCr = CVPixelBufferGetBytesPerRowOfPlane(i, 1);

  size_t sizeInBytes = [self d]->frame.h * [self d]->frame.bytesPerRow;
  [self d]->data.resize(sizeInBytes);

  const size_t byteWidth = ([self d]->frame.w * 4);
  const size_t yMax = [self d]->frame.h;
  for(size_t y=0; y<yMax; y++)
  {
    size_t j = y/2;
    uint8_t* dst     = [self d]->data.data() + (y*[self d]->frame.bytesPerRow);
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

  [self d]->frame.data = [self d]->data.data();

  [self d]->frameReceivedCallback([self d]->frame);
}

#pragma mark - ARSessionObserver

//##################################################################################################
- (void)session:(ARSession *)session didFailWithError:(NSError *)error NS_AVAILABLE_IOS(12_0)
{
  TP_UNUSED(session);
  std::string msg = [[error description] UTF8String];
  tpWarning() << "didFailWithError:\n" << msg;
  [self restartSession];
}

//##################################################################################################
- (void)sessionWasInterrupted:(ARSession *)session NS_AVAILABLE_IOS(12_0)
{
  TP_UNUSED(session);
  tpDebug() << "sessionWasInterrupted";
}

//##################################################################################################
- (void)sessionInterruptionEnded:(ARSession *)session NS_AVAILABLE_IOS(12_0)
{
  TP_UNUSED(session);
  tpDebug() << "sessionInterruptionEnded";
}

@end
