#ifndef tp_ar_ArKitShim_h
#define tp_ar_ArKitShim_h

#include "tp_utils/TPPixel.h"

#include "glm/glm.hpp"

#include <functional>

namespace tp_maps
{
class Map;
}

namespace tp_ar
{
struct Frame;

//##################################################################################################
class ArKitShim
{
public:
  //################################################################################################
  ArKitShim(const std::function<void(Frame&)>& frameReceivedCallback, tp_maps::Map* map);

  //################################################################################################
  ~ArKitShim();  

  //################################################################################################
  void viewFrame(const std::function<void(const tp_ar::Frame&)>& closure);

  //################################################################################################
  void viewFrameData(const std::function<void(const tp_ar::Frame& frame, size_t w, size_t h, const TPPixel* data)>& closure);

  //################################################################################################
  void viewYCbCr(const std::function<void(size_t w, size_t h, const std::vector<uint8_t>& data)>& closure);

  //################################################################################################
  glm::vec2 imageScale();

  struct Private;
  Private* d;
};

}

#endif
