@echo off

echo "android start build......"

ndk-build NDK_PROJECT_PATH=. NDK_APPLICATION_MK=Application.mk

pause

