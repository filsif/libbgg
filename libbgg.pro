#-------------------------------------------------
#
# Project created by QtCreator 2016-07-13T18:37:28
#
#-------------------------------------------------
TARGET = bgg
TEMPLATE = lib

include(../qmake_common.pri)

QT       += core network xml xmlpatterns gui

CONFIG += staticlib

DEFINES += LIBBGG_LIB

INCLUDEPATH += . \
    ./inc \
    ./GeneratedFiles

DEPENDPATH += .
MOC_DIR += ./GeneratedFiles
UI_DIR += ./GeneratedFiles
RCC_DIR += ./GeneratedFiles


SOURCES += src/libbgg/api.cpp \
    src/libbgg/models.cpp \
    src/libbgg/query/boardgame.cpp \
    src/libbgg/query/image.cpp \
    src/libbgg/query/search.cpp \
    src/libbgg/query/searchcollection.cpp

HEADERS += inc/libbgg/api.h \
    inc/libbgg/models.h \
    inc/libbgg/query/boardgame.h \
    inc/libbgg/query/image.h \
    inc/libbgg/query/search.h \
    inc/libbgg/bgg.h \
    inc/libbgg/query/searchcollection.h
unix {
    target.path = /usr/lib
    INSTALLS += target
}
