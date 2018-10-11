#include "tp_ar/ar_sessions/ArKitSession.h"

#include "tp_ar/arkit/ArKitShim.h"

#warning make this non Qt.
#include <QTimer>

namespace tp_ar
{

//##################################################################################################
struct ArKitSession::Private
{
  tp_ar::ArKitShim* arShim{nullptr};
  QTimer timer;

  ~Private()
  {
    delete arShim;
  }
};

//##################################################################################################
ArKitSession::ArKitSession():
  d(new Private())
{
  d->timer.start(1000);
  d->timer.setSingleShot(true);
  QObject::connect(&d->timer, &QTimer::timeout, [&]()
  {
    d->arShim = new tp_ar::ArKitShim([&](const Frame& frame){frameReceived(frame);});
  });
}

//##################################################################################################
ArKitSession::~ArKitSession()
{
  delete d;
}

}
