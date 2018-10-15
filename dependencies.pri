DEPENDENCIES += lib_glm
DEPENDENCIES += tp_utils
INCLUDEPATHS += tp_ar/inc/
LIBRARIES    += tp_ar

iphoneos{
QMAKE_LFLAGS += -framework ARKit
QMAKE_LFLAGS += -framework AVFoundation
}
