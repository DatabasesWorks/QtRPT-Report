QT       += gui xml script sql

greaterThan(QT_MAJOR_VERSION, 4) {
    QT += widgets printsupport
    DEFINES += HAVE_QT5
}

greaterThan(QT_MAJOR_VERSION, 5):
    greaterThan(QT_MINOR_VERSION, 7):QT += charts


include(../CommonFiles/CommonFiles_QtRpt.pri)

INCLUDEPATH += $$PWD

SOURCES += $$PWD/qtrpt.cpp \
           $$PWD/RptSql.cpp \
           $$PWD/RptFieldObject.cpp \
           $$PWD/RptBandObject.cpp \
           $$PWD/RptPageObject.cpp \
           $$PWD/RptCrossTabObject.cpp \
    $$PWD/RptScriptEngine.cpp
HEADERS += $$PWD/qtrpt.h \
           $$PWD/qtrptnamespace.h \
           $$PWD/RptSql.h \
           $$PWD/RptFieldObject.h \
           $$PWD/RptBandObject.h \
           $$PWD/RptPageObject.h \
           $$PWD/RptCrossTabObject.h \
    $$PWD/RptScriptEngine.h

RESOURCES += \
    $$PWD/../QtRPT/imagesRpt.qrc
