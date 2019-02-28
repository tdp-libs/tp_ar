#ifndef tp_ar_ArCoreShim_h
#define tp_ar_ArCoreShim_h

#include <functional>

namespace tp_ar
{
struct Frame;

//##################################################################################################
class ArCoreShim
{
public:
  //################################################################################################
  ArCoreShim(const std::function<void(const Frame&)>& frameReceivedCallback);

  //################################################################################################
  ~ArCoreShim();

  //################################################################################################
  void viewFrame(const std::function<void(const tp_ar::Frame&)>& closure);

  //################################################################################################
  void pollAr();

private:
  struct Private;
  Private* d;
};

}

#endif
