#ifndef tp_ar_Frame_h
#define tp_ar_Frame_h

#include "tp_ar/Globals.h"

#include "glm/glm.hpp"

namespace tp_ar
{
class AbstractBackgroundLayer;

//##################################################################################################
//! Details of a frame each time it is returned from the ar session
/*!
To get the image data use backgroundLayer->viewImageData to access the raw bytes.
*/
struct TP_AR_SHARED_EXPORT Frame
{
  AbstractBackgroundLayer* backgroundLayer;

  //! View matrix
  glm::mat4 cameraTransformation;

  //! Projection matrix
  glm::mat4 cameraCallibration;
};

}

#endif
