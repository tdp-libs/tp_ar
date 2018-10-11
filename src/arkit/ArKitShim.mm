#include "tp_ar/arkit/ArKitShim.h"
#include "tp_ar/Frame.h"

#include "tp_utils/DebugUtils.h"

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

  [self d]->frame.w = CVPixelBufferGetWidth(i);
  [self d]->frame.h = CVPixelBufferGetHeight(i);
  [self d]->frame.bytesPerRow = CVPixelBufferGetBytesPerRow(i);

  size_t sizeInBytes = [self d]->frame.h * [self d]->frame.bytesPerRow;
  [self d]->data.resize(sizeInBytes);
  memcpy([self d]->data.data(), CVPixelBufferGetBaseAddress(i), sizeInBytes);

  [self d]->frame.data = [self d]->data.data();

#warning do something about the format here

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
