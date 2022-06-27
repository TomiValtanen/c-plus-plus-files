TEMPLATE = app
CONFIG += console c++17
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
        card.cpp \
        deck.cpp \
        game.cpp \
        main.cpp \
        player.cpp

HEADERS += \
    card.h \
    deck.h \
    game.h \
    player.h
