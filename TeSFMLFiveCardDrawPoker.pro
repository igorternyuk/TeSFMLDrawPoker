TEMPLATE = app
CONFIG += c++1z
CONFIG -= app_bundle
CONFIG -= qt

DEFINES -= DEBUG

LIBS += `pkg-config --libs sfml-all`

SOURCES += main.cpp \
    card.cpp \
    hand.cpp \
    player.cpp \
    model.cpp \
    controller.cpp \
    view.cpp \
    deck.cpp \
    random.cpp

HEADERS += \
    card.h \
    hand.h \
    player.h \
    model.h \
    controller.h \
    view.h \
    modellistener.h \
    constantmodelinterface.h \
    deck.h \
    random.h \
    utils.h
