#ifndef tp_ar_AbstractSession_h
#define tp_ar_AbstractSession_h

#include "tp_ar/Globals.h"

namespace tp_ar
{

//##################################################################################################
class TP_AR_SHARED_EXPORT AbstractSession
{
public:
  //################################################################################################
  AbstractSession();

  //################################################################################################
  virtual ~AbstractSession();

private:
  struct Private;
  Private* d;
  friend struct Private;
};

}

#endif
