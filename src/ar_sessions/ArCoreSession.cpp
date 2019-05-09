#include "tp_ar/ar_sessions/ArCoreSession.h"

#include "tp_ar/arcore/ArCoreShim.h"

namespace tp_ar
{

//##################################################################################################
struct ArCoreSession::Private
{
  tp_ar::ArCoreShim arShim;

  Private(ArCoreSession* q):
    arShim([q](const Frame& frame){q->frameReceived(frame);})
  {

  }
};

//##################################################################################################
ArCoreSession::ArCoreSession(tp_maps::Map* map):
  AbstractSession(map),
  d(new Private(this))
{
}

//##################################################################################################
ArCoreSession::~ArCoreSession()
{
  delete d;
}

//################################################################################################
void ArCoreSession::viewFrame(const std::function<void(const tp_ar::Frame&)>& closure)
{
  d->arShim.viewFrame(closure);
}

//##################################################################################################
void ArCoreSession::pollAr()
{
  d->arShim.pollAr();
}

}
