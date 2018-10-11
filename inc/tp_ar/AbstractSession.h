#ifndef tp_ar_AbstractSession_h
#define tp_ar_AbstractSession_h

#include "tp_ar/Globals.h"

namespace tp_ar
{
struct Frame;

//##################################################################################################
class TP_AR_SHARED_EXPORT AbstractSession
{
public:
  //################################################################################################
  AbstractSession();

  //################################################################################################
  virtual ~AbstractSession();

  //################################################################################################
  void addFrameReceivedCallback(const std::function<void(const Frame&)>* frameReceivedCallback);

  //################################################################################################
  void frameFrameReceivedCallback(const std::function<void(const Frame&)>* frameReceivedCallback);

private:
  struct Private;
  Private* d;
  friend struct Private;
};

}

#endif
