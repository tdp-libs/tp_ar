# ARCore on Android

## Get the SDK
```
git clone https://github.com/google-ar/arcore-android-sdk.git
```
Add the include and lib paths to your project.inc file.
```
INCLUDEPATHS += /opt/tom/arcore-android-sdk/libraries/include/

# Select what is relevant for you or add them all and put up with the warnings
# LIBS += -L/opt/tom/arcore-android-sdk/samples/hello_ar_c/app/build/arcore-native/jni/arm64-v8a/
LIBS += -L/opt/tom/arcore-android-sdk/samples/hello_ar_c/app/build/arcore-native/jni/armeabi-v7a/
# LIBS += -L/opt/tom/arcore-android-sdk/samples/hello_ar_c/app/build/arcore-native/jni/x86/
# LIBS += -L/opt/tom/arcore-android-sdk/samples/hello_ar_c/app/build/arcore-native/jni/x86_64/
```
