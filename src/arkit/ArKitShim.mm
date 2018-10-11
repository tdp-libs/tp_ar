#include "tp_ar/arkit/ArKitShim.h"

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
  ARSession* session NS_AVAILABLE_IOS(12_0){nullptr};
  ArKitShimPrivate* delegate{nullptr};
};

//##################################################################################################
ArKitShim::ArKitShim():
  d(new Private())
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

  tpDebug() << "didUpdateFrame";
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
