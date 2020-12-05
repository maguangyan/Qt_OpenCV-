QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    camera.cpp \
    main.cpp \
    mainwindow.cpp \
    myalgorithmlibrary.cpp \
    mythread.cpp

HEADERS += \
    camera.h \
    mainwindow.h \
    myalgorithmlibrary.h \
    mythread.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

# opencv3.3.1
win32:CONFIG(release, debug|release): LIBS += -LD:/opencv/build/x64/vc14/lib/ -lopencv_world331
else:win32:CONFIG(debug, debug|release): LIBS += -LD:/opencv/build/x64/vc14/lib/ -lopencv_world331d
else:unix: LIBS += -LD:/opencv/build/x64/vc14/lib/ -lopencv_world331

INCLUDEPATH += D:/opencv/build/include
DEPENDPATH += D:/opencv/build/include


# zbar
win32:CONFIG(release, debug|release): LIBS += -L'D:/Program Files (x86)/Tools/ZBar_64/lib/' -llibzbar64-0
else:win32:CONFIG(debug, debug|release): LIBS += -L'D:/Program Files (x86)/Tools/ZBar_64/lib/' -llibzbar64-0
else:unix: LIBS += -L'D:/Program Files (x86)/Tools/ZBar_64/lib/' -llibzbar64-0

INCLUDEPATH += 'D:/Program Files (x86)/Tools/ZBar_64/include'
DEPENDPATH += 'D:/Program Files (x86)/Tools/ZBar_64/include'

# HIKCamera
win32:CONFIG(release, debug|release): LIBS += -L'C:/Program Files (x86)/MVS/Development/Libraries/win64/' -lMvCameraControl
else:win32:CONFIG(debug, debug|release): LIBS += -L'C:/Program Files (x86)/MVS/Development/Libraries/win64/' -lMvCameraControl
else:unix: LIBS += -L'C:/Program Files (x86)/MVS/Development/Libraries/win64/' -lMvCameraControl

INCLUDEPATH += 'C:/Program Files (x86)/MVS/Development/Includes'
DEPENDPATH += 'C:/Program Files (x86)/MVS/Development/Includes'

RESOURCES += \
    res.qrc
