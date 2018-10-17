#include "tp_ar/ar_sessions/ArKitSession.h"

#include "tp_ar/arkit/ArKitShim.h"

namespace tp_ar
{

//##################################################################################################
struct ArKitSession::Private
{
  tp_ar::ArKitShim arShim;

  Private(ArKitSession* q):
    arShim([q](const Frame& frame){q->frameReceived(frame);})
  {

  }
};

//##################################################################################################
ArKitSession::ArKitSession():
  d(new Private(this))
{
}

//##################################################################################################
ArKitSession::~ArKitSession()
{
  delete d;
}

//################################################################################################
void ArKitSession::viewFrame(const std::function<void(const tp_ar::Frame&)>& closure)
{
  d->arShim.viewFrame(closure);
}

}
