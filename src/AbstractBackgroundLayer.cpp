#include "tp_ar/AbstractBackgroundLayer.h"

namespace tp_ar
{

//##################################################################################################
AbstractBackgroundLayer::AbstractBackgroundLayer()
{
  setDefaultRenderPass(tp_maps::RenderPass::Background);
}

//##################################################################################################
AbstractBackgroundLayer::~AbstractBackgroundLayer()
{

}

}
