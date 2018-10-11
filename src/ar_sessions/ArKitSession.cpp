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
};

//##################################################################################################
ArKitSession::ArKitSession():
  d(new Private())
{
  QTimer* timer = new QTimer();
  timer->start(1000);
  timer->setSingleShot(true);
  QObject::connect(timer, &QTimer::timeout, [&, timer]()
  {
    timer->deleteLater();
    d->arShim = new tp_ar::ArKitShim();
  });
}

//##################################################################################################
ArKitSession::~ArKitSession()
{
  delete d;
}

}
