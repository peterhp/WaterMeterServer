#-------------------------------------------------
#
# Project created by QtCreator 2018-03-19T23:12:50
#
#-------------------------------------------------

QT       += core

QT       -= gui

QMAKE_CFLAGS += -std=c99

TARGET = WaterMeterServer
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += main.cpp \
    buffer/blk_buf.cpp \
    buffer/cir_buf.cpp \
    protocol/wmp/wmp.c \
    test/test_buffer.cpp \
    test/test_crc.cpp \
    util/crc.c

HEADERS += \
    buffer/buffer.h \
    protocol/wmp/wmp.h \
    protocol/protocol.h \
    protocol/wm_context.h \
    test/test.h \
    util/crc.h \
    util/errors.h \
    util/macros.h \
    types.h
