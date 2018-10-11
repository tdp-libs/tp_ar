#include "tp_ar/AbstractSession.h"

#include "tp_utils/MutexUtils.h"

namespace tp_ar
{

//##################################################################################################
struct AbstractSession::Private
{
  TPMutex mutex{TPM};
  std::vector<const std::function<void(const Frame&)>*> frameReceivedCallbacks;
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

//##################################################################################################
void AbstractSession::addFrameReceivedCallback(const std::function<void(const Frame&)>* frameReceivedCallback)
{
  TP_MUTEX_LOCKER(d->mutex);
  d->frameReceivedCallbacks.push_back(frameReceivedCallback);
}

//##################################################################################################
void AbstractSession::removeFrameReceivedCallback(const std::function<void(const Frame&)>* frameReceivedCallback)
{
  TP_MUTEX_LOCKER(d->mutex);
  tpRemoveOne(d->frameReceivedCallbacks, frameReceivedCallback);
}

//##################################################################################################
void AbstractSession::frameReceived(const Frame& frame)
{
  TP_MUTEX_LOCKER(d->mutex);
  for(auto frameReceivedCallback: d->frameReceivedCallbacks)
    (*frameReceivedCallback)(frame);
}

}
