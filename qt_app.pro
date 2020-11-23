QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

CONFIG += c++17 force_debug_info test

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

#message(Configuring test build...)

#SOURCES += \
#    src/JsonApi.cpp \
#    src/JsonParser.cpp \
#    src/WeatherData.cpp \
#    src/main.cpp \
#    src/mainwindow.cpp \
#    src/DbManager.cpp \
#    src/qcustomplot.cpp

#HEADERS += \
#    inc/JsonApi.hpp \
#    inc/JsonParser.hpp \
#    inc/WeatherData.hpp \
#    inc/mainwindow.h \
#    inc/DbManager.hpp \
#    inc/qcustomplot.h

test {
    message(Test build)
    QT += testlib
    TARGET = UnitTests
    #mac:
    #INCLUDEPATH += /usr/local/include \
    #               /usr/local/Cellar/nlohmann-json/3.7.3/include/
    #               /usr/local/curl/include

    #LIBS += /Users/mike/Projects/GoogleTestWithQtLab/SimpleGTest-Solution/GoogleTest/googletest/googletest/install/libgtest.a \
    #        /Users/mike/Projects/GoogleTestWithQtLab/SimpleGTest-Solution/GoogleTest/googletest/googletest/install/libgtest_main.a \
    #        -lcurl

    LIBS += -lgtest \
            -lgmock \
            -lcurl

    #QMAKE_CXXFLAGS += --coverage
    #QMAKE_LFLAGS += --coverage

    SOURCES += \
       tests/DbManagerTest.cpp \
       tests/JsonApiTest.cpp \
       tests/JsonParserTest.cpp \
       src/DbManager.cpp \
       src/JsonApi.cpp \
       tests/test_main.cpp
} else {
    message(Normal build)
    #mac:
    #INCLUDEPATH += /usr/local/include \
    #               /usr/local/Cellar/nlohmann-json/3.7.3/include/

    #LIBS += -lcurl
    #        -lsqlite3

    SOURCES += \
        src/JsonApi.cpp \
        src/JsonParser.cpp \
        src/WeatherData.cpp \
        src/main.cpp \
        src/mainwindow.cpp \
        src/DbManager.cpp \
        src/qcustomplot.cpp

    HEADERS += \
        inc/JsonApi.hpp \
        inc/JsonParser.hpp \
        inc/WeatherData.hpp \
        inc/mainwindow.h \
        inc/DbManager.hpp \
        inc/qcustomplot.h
}


FORMS += \
    mainwindow.ui

# Default rules for deployment.
# qnx: target.path = /tmp/$${TARGET}/bin
# else: unix:!android: target.path = /opt/$${TARGET}/bin
# !isEmpty(target.path): INSTALLS += target

