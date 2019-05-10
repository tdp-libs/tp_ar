#include "tp_ar/ar_sessions/ArKitSession.h"
#include "tp_ar/arkit/ArKitShim.h"
#include "tp_ar/layers/ArKitBackgroundLayer.h"
#include "tp_ar/Frame.h"

#include "tp_maps/Map.h"

namespace tp_ar
{

//##################################################################################################
struct ArKitSession::Private
{
  ArKitSession* q;
  ArKitBackgroundLayer* backgroundLayer;
  ArKitShim arShim;

  //################################################################################################
  Private(ArKitSession* q_, tp_maps::Map* map):
    q(q_),
    backgroundLayer(new ArKitBackgroundLayer(q)),
    arShim(([&](Frame& frame){frameReceived(frame);}), map)
  {
    backgroundLayer->setDefaultRenderPass(tp_maps::RenderPass::Background);
    map->addLayer(backgroundLayer);
  }

  //################################################################################################
  void frameReceived(Frame& frame)
  {
    backgroundLayer->updateTexture();
    frame.backgroundLayer=backgroundLayer;
    q->frameReceived(frame);
  }
};

//##################################################################################################
ArKitSession::ArKitSession(tp_maps::Map* map):
  AbstractSession(map),
  d(new Private(this, map))
{
}

//##################################################################################################
ArKitSession::~ArKitSession()
{
  delete d;
}

//##################################################################################################
void ArKitSession::viewFrame(const std::function<void(const tp_ar::Frame&)>& closure)
{
  d->arShim.viewFrame(closure);
}

//##################################################################################################
void ArKitSession::viewFrameData(const std::function<void(const tp_ar::Frame& frame, size_t w, size_t h, const TPPixel* data)>& closure)
{
  d->arShim.viewFrameData(closure);
}

//##################################################################################################
void ArKitSession::viewYCbCr(const std::function<void(size_t w, size_t h, const std::vector<uint8_t>& data)>& closure)
{
  d->arShim.viewYCbCr(closure);
}

//##################################################################################################
glm::vec2 ArKitSession::imageScale()
{
  return d->arShim.imageScale();
}

//##################################################################################################
AbstractBackgroundLayer* ArKitSession::backgroundLayer()
{
  return d->backgroundLayer;
}

}
