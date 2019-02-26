DEPENDENCIES += lib_glm
DEPENDENCIES += tp_utils
INCLUDEPATHS += tp_ar/inc/
LIBRARIES    += tp_ar

android{
LIBS += -larcore_sdk_c
LIBS += -larcore_sdk_jni
QT += androidextras
}

iphoneos{
QMAKE_LFLAGS += -framework ARKit
QMAKE_LFLAGS += -framework AVFoundation
}
