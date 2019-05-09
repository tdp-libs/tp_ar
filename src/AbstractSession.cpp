#include "tp_ar/AbstractSession.h"

#include "tp_utils/MutexUtils.h"

namespace tp_ar
{

//##################################################################################################
AbstractSession::AbstractSession(tp_maps::Map* map)
{
  TP_UNUSED(map);
}

//##################################################################################################
AbstractSession::~AbstractSession() = default;

//##################################################################################################
void AbstractSession::pollAr()
{

}

}
