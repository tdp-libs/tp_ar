TARGET = tp_ar
TEMPLATE = lib

DEFINES += TP_AR_LIBRARY

#SOURCES += src/Globals.cpp
HEADERS += inc/tp_ar/Globals.h

SOURCES += src/Frame.cpp
HEADERS += inc/tp_ar/Frame.h

SOURCES += src/AbstractSession.cpp
HEADERS += inc/tp_ar/AbstractSession.h

SOURCES += src/ar_sessions/DummySession.cpp
HEADERS += inc/tp_ar/ar_sessions/DummySession.h

HEADERS += inc/tp_ar/ar_sessions/PlatformSession.h

android{

SOURCES += src/ar_sessions/ArCoreSession.cpp
HEADERS += inc/tp_ar/ar_sessions/ArCoreSession.h

SOURCES += src/arcore/ArCoreShim.cpp
HEADERS += inc/tp_ar/arcore/ArCoreShim.h

}

else:iphoneos{

SOURCES += src/ar_sessions/ArKitSession.cpp
HEADERS += inc/tp_ar/ar_sessions/ArKitSession.h

OBJECTIVE_SOURCES += src/arkit/ArKitShim.mm
HEADERS += inc/tp_ar/arkit/ArKitShim.h

}

else{

}

