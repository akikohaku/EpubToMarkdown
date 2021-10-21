QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    OSS.h \
    golbal.h \
    mainwindow.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
QMAKE_CXXFLAGS += -utf-8

INCLUDEPATH += D:/programfiles/aliyun-oss-cpp-sdk-master/sdk/include \
               D:/programfiles/aliyun-oss-cpp-sdk-master/sdk/src \
               D:/programfiles/aliyun-oss-cpp-sdk-master/third_party/include

LIBS += -LD:/programfiles/aliyun-oss-cpp-sdk-master/third_party/lib/x64 -llibcurl
LIBS += -LD:/programfiles/aliyun-oss-cpp-sdk-master/third_party/lib/x64 -llibeay32
LIBS += -LD:/programfiles/aliyun-oss-cpp-sdk-master/third_party/lib/x64 -lssleay32
LIBS += -LD:/programfiles/aliyun-oss-cpp-sdk-master/lib/Debug -lalibabacloud-oss-cpp-sdk
