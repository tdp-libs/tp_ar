#ifndef tp_ar_AbstractBackgroundLayer_h
#define tp_ar_AbstractBackgroundLayer_h

#include "tp_ar/Globals.h"

#include "tp_maps/Layer.h"

namespace tp_ar
{

//##################################################################################################
class AbstractBackgroundLayer : public tp_maps::Layer
{
public:
  //################################################################################################
  AbstractBackgroundLayer();

  //################################################################################################
  ~AbstractBackgroundLayer() override;
};

}

#endif
