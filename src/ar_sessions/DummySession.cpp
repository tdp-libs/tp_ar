#include "tp_ar/ar_sessions/DummySession.h"

namespace tp_ar
{

//##################################################################################################
DummySession::DummySession()
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
