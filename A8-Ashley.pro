QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    board.cpp \
    game.cpp \
    gameoverdialog.cpp \
    main.cpp \
    mainwindow.cpp \
    model.cpp \
    numbers.cpp \
    tutorial.cpp

HEADERS += \
    board.h \
    game.h \
    gameoverdialog.h \
    mainwindow.h \
    model.h \
    numbers.h \
    tutorial.h

FORMS += \
    board.ui \
    game.ui \
    gameoverdialog.ui \
    mainwindow.ui \
    numbers.ui \
    tutorial.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    images.qrc
