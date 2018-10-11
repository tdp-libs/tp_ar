#ifndef tp_ar_ArKitShim_h
#define tp_ar_ArKitShim_h

#include <functional>

namespace tp_ar
{
struct Frame;

//##################################################################################################
class ArKitShim
{
public:
  //################################################################################################
  ArKitShim(const std::function<void(const Frame&)>& frameReceivedCallback);

  //################################################################################################
  ~ArKitShim();  


  struct Private;
  Private* d;
};

}

#endif
