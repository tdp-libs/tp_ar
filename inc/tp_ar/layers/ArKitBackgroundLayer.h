#ifndef tp_ar_ArKitBackgroundLayer_h
#define tp_ar_ArKitBackgroundLayer_h

#include "tp_ar/AbstractBackgroundLayer.h"

#include "tp_maps/Layer.h"

#include "tp_utils/TPPixel.h"

namespace tp_ar
{
class ArKitSession;

//##################################################################################################
class ArKitBackgroundLayer : public AbstractBackgroundLayer
{
public:
  //################################################################################################
  ArKitBackgroundLayer(ArKitSession* session);

  //################################################################################################
  ~ArKitBackgroundLayer() override;

  //################################################################################################
  void updateTexture();

protected:
  //################################################################################################
  void render(tp_maps::RenderInfo& renderInfo) override;

  //################################################################################################
  void invalidateBuffers() override;

private:
  struct Private;
  friend struct Private;
  Private* d;
};

}

#endif
