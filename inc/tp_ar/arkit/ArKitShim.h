#ifndef tp_ar_ArKitShim_h
#define tp_ar_ArKitShim_h

#include "tp_utils/TPPixel.h"

#include <functional>

namespace tp_ar
{
struct Frame;

//##################################################################################################
class ArKitShim
{
public:
  //################################################################################################
  ArKitShim(const std::function<void(Frame&)>& frameReceivedCallback);

  //################################################################################################
  ~ArKitShim();  

  //################################################################################################
  void viewFrame(const std::function<void(const tp_ar::Frame&)>& closure);

  //################################################################################################
  void viewFrameData(const std::function<void(const tp_ar::Frame& frame, size_t w, size_t h, const TPPixel* data)>& closure);

  //################################################################################################
  void viewYCbCr(const std::function<void(size_t w, size_t h, const std::vector<uint8_t>& data)>& closure);

  struct Private;
  Private* d;
};

}

#endif
