#-------------------------------------------------
#
# Project created by QtCreator 2017-01-16T09:40:09
#
#-------------------------------------------------

QT       += core gui



TARGET = piezo_analyzer
TEMPLATE = app
greaterThan(QT_MAJOR_VERSION, 4) {
    QT       += widgets serialport
} else {
    include($$QTSERIALPORT_PROJECT_ROOT/src/serialport/qt4support/serialport.prf)
}

SOURCES += main.cpp\
        mainwindow.cpp \
    headers.cpp \
    rs232plot.cpp

HEADERS  += mainwindow.h \
    headers.h \
    rs232plot.h

FORMS    += mainwindow.ui

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../../usr/local/qwt-6.1.3/lib/release/ -lqwt
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../../usr/local/qwt-6.1.3/lib/debug/ -lqwt
else:unix:!macx: LIBS += -L$$PWD/../../../usr/local/qwt-6.1.3/lib/ -lqwt

INCLUDEPATH += $$PWD/../../../usr/local/qwt-6.1.3/include
DEPENDPATH += $$PWD/../../../usr/local/qwt-6.1.3/include
