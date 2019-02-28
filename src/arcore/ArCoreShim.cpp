#include "tp_ar/arcore/ArCoreShim.h"

#include "tp_utils/DebugUtils.h"

#include "glm/gtc/type_ptr.hpp"

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
  TP_UNUSED(frameReceivedCallback);
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


  if(ArSession_resume(d->arSession) != AR_SUCCESS)
  {
    tpWarning() << "ArSession_resume error!";
    return;
  }
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
TP_UNUSED(closure);
}

//##################################################################################################
void ArCoreShim::pollAr()
{
  if(!d->arSession)
    return;

  //ArSession_setCameraTextureName(d->arSession,
  //                               background_renderer_.GetTextureId());

  // Update session to get current frame and render camera background.
  if (ArSession_update(d->arSession, d->arFrame) != AR_SUCCESS)
  {
    tpWarning() << "ArSession_update error.";
    return;
  }

  ArCamera* arCamera{nullptr};
  ArFrame_acquireCamera(d->arSession, d->arFrame, &arCamera);

  glm::mat4 viewMatrix;
  glm::mat4 projectionMatrix;
  ArCamera_getViewMatrix(d->arSession, arCamera, glm::value_ptr(viewMatrix));
  ArCamera_getProjectionMatrix(d->arSession, arCamera,
                               /*near=*/0.1f, /*far=*/100.f,
                               glm::value_ptr(projectionMatrix));

  tpDebug() << viewMatrix[3][0] << "  " << viewMatrix[3][1] << "  " << viewMatrix[3][2];

  ArTrackingState camera_tracking_state;
  ArCamera_getTrackingState(d->arSession, arCamera, &camera_tracking_state);
  ArCamera_release(arCamera);
}

}
