#include "tp_ar/arcore/ArCoreShim.h"

#include "tp_utils/DebugUtils.h"

#include <arcore_c_api.h>

#include <QtAndroid>
#include <QAndroidJniEnvironment>

namespace tp_ar
{
//##################################################################################################
struct ArCoreShim::Private
{
  ArSession* arSession{nullptr};
  ArFrame*   arFrame  {nullptr};

  bool installRequested{false};
};

//##################################################################################################
ArCoreShim::ArCoreShim(const std::function<void(const Frame&)>& frameReceivedCallback):
  d(new Private)
{
  tpDebug() << "A";
  ArInstallStatus installStatus;

  // If install was not yet requested, that means that we are resuming the
  // activity first time because of explicit user interaction (such as
  // launching the application)
  bool userRequestedInstall = !d->installRequested;

  QAndroidJniObject objActivity = QtAndroid::androidActivity();
  jobject jobjActivity = objActivity.object();

  QAndroidJniObject objContext = QtAndroid::androidContext();
  jobject jobjContext = objContext.object();

  QAndroidJniEnvironment envObj;

  tpDebug() << "B";
  void* env      = envObj;
  void* context  = jobjContext;
  void* activity = jobjActivity;

  if(ArCoreApk_requestInstall(env, activity, userRequestedInstall, &installStatus) != AR_SUCCESS)
  {
    tpWarning() << "ArCoreApk_requestInstall failed!";
    return;
  }
  tpDebug() << "C";

  switch (installStatus)
  {
    case AR_INSTALL_STATUS_INSTALLED:
      break;
    case AR_INSTALL_STATUS_INSTALL_REQUESTED:
      d->installRequested = true;
      return;
  }
  tpDebug() << "D";

  // This method can and will fail in user-facing situations.
  if(ArSession_create(env, context, &d->arSession) != AR_SUCCESS)
  {
    tpWarning() << "ArSession_create failed!";
    return;
  }
  tpDebug() << "E";

  if(!d->arSession)
  {
    tpWarning() << "Null ArSession!";
    return;
  }
  tpDebug() << "F";

  ArFrame_create(d->arSession, &d->arFrame);

  if(!d->arFrame)
  {
    tpWarning() << "Null ArFrame!";
    return;
  }
  tpDebug() << "G";

  int width = 100;
  int height = 100;
  int displayRotation = 0;
  tpDebug() << "H";

  ArSession_setDisplayGeometry(d->arSession, displayRotation, width, height);
  tpDebug() << "I";
}

//##################################################################################################
ArCoreShim::~ArCoreShim()
{
  if(d->arSession)
  {
    ArSession_destroy(d->arSession);
    ArFrame_destroy(d->arFrame);
  }

  delete d;
}

//##################################################################################################
void ArCoreShim::viewFrame(const std::function<void(const tp_ar::Frame&)>& closure)
{

}

}
