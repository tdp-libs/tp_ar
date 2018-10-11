#ifndef tp_ar_ArKitSession_h
#define tp_ar_ArKitSession_h

#include "tp_ar/AbstractSession.h"

namespace tp_ar
{

//##################################################################################################
class TP_AR_SHARED_EXPORT ArKitSession : public AbstractSession
{
public:
  //################################################################################################
  ArKitSession();

  //################################################################################################
  virtual ~ArKitSession();

private:
  struct Private;
  Private* d;
  friend struct Private;
};

}

#endif
