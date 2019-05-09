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
  ArKitSession(tp_maps::Map* map);

  //################################################################################################
  ~ArKitSession() override;

  //################################################################################################
  void viewFrame(const std::function<void(const tp_ar::Frame&)>& closure) override;

  //################################################################################################
  void viewFrameData(const std::function<void(const tp_ar::Frame& frame, size_t w, size_t h, const TPPixel* data)>& closure) override;

  //################################################################################################
  void viewYCbCr(const std::function<void(size_t w, size_t h, const std::vector<uint8_t>& data)>& closure);

  //################################################################################################
  AbstractBackgroundLayer* backgroundLayer() override;

private:
  struct Private;
  Private* d;
  friend struct Private;
};

}

#endif
