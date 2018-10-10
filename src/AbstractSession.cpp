#include "tp_ar/AbstractSession.h"

namespace tp_ar
{

//##################################################################################################
struct AbstractSession::Private
{

};

//##################################################################################################
AbstractSession::AbstractSession():
  d(new Private())
{

}

//##################################################################################################
AbstractSession::~AbstractSession()
{
  delete d;
}

}
