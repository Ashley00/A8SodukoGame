QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    board.cpp \
    gameoverdialog.cpp \
    main.cpp \
    mainwindow.cpp \
    model.cpp \
    numbers.cpp

HEADERS += \
    board.h \
    gameoverdialog.h \
    mainwindow.h \
    model.h \
    numbers.h

FORMS += \
    board.ui \
    gameoverdialog.ui \
    mainwindow.ui \
    numbers.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    Images.qrc
