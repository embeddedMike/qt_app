QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets #printsupport

CONFIG += c++17 force_debug_info

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    src/JsonApi.cpp \
    src/JsonParser.cpp \
    src/main.cpp \
    src/mainwindow.cpp \
    src/DbManager.cpp
    #qcustomplot.cpp

HEADERS += \
    inc/JsonApi.hpp \
    inc/JsonParser.hpp \
    inc/mainwindow.h \
    inc/DbManager.hpp
    #qcustomplot.h


INCLUDEPATH += /usr/local/include \
               /usr/local/Cellar/nlohmann-json/3.7.3/include/

LIBS += -lcurl \
        -lsqlite3

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
