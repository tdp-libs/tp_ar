#ifndef tp_ar_ArCoreSession_h
#define tp_ar_ArCoreSession_h

#include "tp_ar/AbstractSession.h"

namespace tp_ar
{

//##################################################################################################
class TP_AR_SHARED_EXPORT ArCoreSession : public AbstractSession
{
public:
  //################################################################################################
  ArCoreSession();

  //################################################################################################
  ~ArCoreSession() override;

  //################################################################################################
  void viewFrame(const std::function<void(const tp_ar::Frame&)>& closure) override;

  //################################################################################################
  void pollAr() override;

private:
  struct Private;
  Private* d;
  friend struct Private;
};

}

#endif
