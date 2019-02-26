#ifndef tp_ar_PlatformSession_h
#define tp_ar_PlatformSession_h

#ifdef TDP_IOS
#include "tp_ar/ar_sessions/ArKitSession.h"
namespace tp_ar
{
using PlatformSession = ArKitSession;
}
#elif defined TDP_ANDROID
#include "tp_ar/ar_sessions/ArCoreSession.h"
namespace tp_ar
{
using PlatformSession = ArCoreSession;
}
#else
#include "tp_ar/ar_sessions/DummySession.h"
namespace tp_ar
{
using PlatformSession = DummySession;
}
#endif

#endif
