#ifndef tp_ar_Frame_h
#define tp_ar_Frame_h

#include "tp_ar/Globals.h"

namespace tp_ar
{

//##################################################################################################
struct TP_AR_SHARED_EXPORT Frame
{
  uint8_t* data{nullptr};
  size_t w{0};
  size_t h{0};
  size_t bytesPerRow{0};
};

}

#endif
