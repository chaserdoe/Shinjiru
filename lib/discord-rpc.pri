contains(QT_ARCH, i386) {
  LIBS += -L$$PWD/discord-rpc/lib/x86 -ldiscord-rpc
} else {
  LIBS += -L$$PWD/discord-rpc/lib/ -ldiscord-rpc
}



INCLUDEPATH += $$PWD/discord-rpc/include
DEPENDPATH += $$PWD/discord-rpc/include

discord.path = ../discord-rpc/$$TARGET
INSTALLS += discord

linux:QMAKE_LFLAGS += -no-pie
