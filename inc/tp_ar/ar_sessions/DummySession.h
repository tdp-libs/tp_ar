#ifndef tp_ar_DummySession_h
#define tp_ar_DummySession_h

#include "tp_ar/AbstractSession.h"

namespace tp_ar
{

//##################################################################################################
class TP_AR_SHARED_EXPORT DummySession : AbstractSession
{
public:
  //################################################################################################
  DummySession();

  //################################################################################################
  virtual ~DummySession();

private:
  struct Private;
  Private* d;
  friend struct Private;
};

}

#endif
