#-------------------------------------------------
#
# Project created by QtCreator 2018-03-19T23:12:50
#
#-------------------------------------------------

QT       += core network

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
    util/crc.c \
    server/wm_server.cpp \
    client/wm_client.cpp \
    test/test_client_server.cpp \
    server/conn_manager.cpp \
    server/pack_manager.cpp \
    protocol/wm_context.c \
    test/test_wm_server.cpp

HEADERS += \
    buffer/buffer.h \
    protocol/wmp/wmp.h \
    protocol/wm_context.h \
    test/test.h \
    util/crc.h \
    util/errors.h \
    util/macros.h \
    types.h \
    config.h \
    server/server.h \
    client/client.h \
    server/conn_manager.h \
    server/pack_manager.h
