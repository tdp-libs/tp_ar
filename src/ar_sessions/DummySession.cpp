#include "tp_ar/ar_sessions/DummySession.h"

namespace tp_ar
{

//##################################################################################################
DummySession::DummySession(tp_maps::Map* map):
  AbstractSession(map)
{

}

//##################################################################################################
DummySession::~DummySession()
{

}

//##################################################################################################
void DummySession::viewFrame(const std::function<void(const tp_ar::Frame&)>& closure)
{
  TP_UNUSED(closure);
}

}
