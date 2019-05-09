#ifndef tp_ar_AbstractSession_h
#define tp_ar_AbstractSession_h

#include "tp_ar/Globals.h"

#include "tp_utils/CallbackCollection.h"
#include "tp_utils/TPPixel.h"

namespace tp_maps
{
class Map;
}

namespace tp_ar
{
struct Frame;
class AbstractBackgroundLayer;

//##################################################################################################
class TP_AR_SHARED_EXPORT AbstractSession
{
public:
  //################################################################################################
  AbstractSession(tp_maps::Map* map);

  //################################################################################################
  virtual ~AbstractSession();

  //################################################################################################
  //! Called each time a frame is received
  /*!
  \warning This should only be used in the main thread.
  */
  tp_utils::CallbackCollection<void(const Frame&)> frameReceived;

  //################################################################################################
  virtual void viewFrame(const std::function<void(const tp_ar::Frame&)>& closure) = 0;

  //################################################################################################
  virtual void viewFrameData(const std::function<void(const tp_ar::Frame& frame, size_t w, size_t h, const TPPixel* data)>& closure) = 0;

  //################################################################################################
  virtual void pollAr();

  //################################################################################################
  virtual AbstractBackgroundLayer* backgroundLayer() = 0;
};

}

#endif
