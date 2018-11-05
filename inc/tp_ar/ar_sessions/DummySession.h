#ifndef tp_ar_DummySession_h
#define tp_ar_DummySession_h

#include "tp_ar/AbstractSession.h"

namespace tp_ar
{

//##################################################################################################
class TP_AR_SHARED_EXPORT DummySession : public AbstractSession
{
public:
  //################################################################################################
  DummySession();

  //################################################################################################
  ~DummySession() override;

  //################################################################################################
  void viewFrame(const std::function<void(const tp_ar::Frame&)>& closure) override;
};

}

#endif
