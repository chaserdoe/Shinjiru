QT += core gui widgets network networkauth qml quick

TARGET = Shinjiru
TEMPLATE = app

VERSION_MAJOR = 3
VERSION_MINOR = 4
VERSION_PATCH = 0

DEFINES += "VERSION_MAJOR=$$VERSION_MAJOR"\
       "VERSION_MINOR=$$VERSION_MINOR"\
       "VERSION_PATCH=$$VERSION_PATCH"

VERSION = $${VERSION_MAJOR}.$${VERSION_MINOR}.$${VERSION_PATCH}

CONFIG += c++14
CONFIG += force_debug_info

DEFINES += QT_DEPRECATED_WARNINGS

include(lib/robot.pri)
include(lib/anitomy.pri)
include(lib/discord-rpc.pri)
include(lib/breakpad.pri)
include(lib/QSimpleUpdater/QSimpleUpdater.pri)

win32:RC_ICONS += res/icon.ico

win32-msvc* {
  QMAKE_CXXFLAGS_WARN_ON -= -W3
  QMAKE_CXXFLAGS_WARN_ON += -W4 -w44640
  QMAKE_CXXFLAGS_RELEASE += -O2 -MD
  QMAKE_LFLAGS_RELEASE = /INCREMENTAL:NO /DEBUG /MAP /OPT:REF
}

macx {
  QMAKE_CXXFLAGS += -x objective-c++

  QMAKE_LFLAGS += -F /System/Library/Frameworks/ApplicationServices.framework/
  LIBS += -framework ApplicationServices

  QMAKE_LFLAGS += -F /System/Library/Frameworks/AppKit.framework/
  LIBS += -framework AppKit

  QMAKE_INFO_PLIST = Info.plist
  ICON = res/icon.icns

  CONFIG -= force_debug_info
}

SOURCES += \
  src/clients/anilist.cpp \
  src/clients/discord.cpp \
  src/clients/graphql_query.cpp \
  src/clients/masato.cpp \
  src/clients/nekomimi.cpp \
  src/detection/media_store.cpp \
  src/detection/window_enumerator.cpp \
  src/main.cpp \
  src/models/media.cpp \
  src/models/media_list.cpp \
  src/models/rss_item.cpp \
  src/models/user.cpp \
  src/settings.cpp \
  src/utilities/crash_handler.cpp \
  src/utilities/file_downloader.cpp \
  ui/anime_panel.cpp \
  ui/main_window.cpp \
  ui/main_window/components/media_table_model.cpp \
  ui/main_window/components/media_table_proxy_model.cpp \
  ui/main_window/components/progress_item_delegate.cpp \
  ui/main_window/components/rss_table_model.cpp \
  ui/main_window/components/score_item_delegate.cpp \
  ui/main_window/components/status_item_delegate.cpp \
  ui/main_window/views/airing.cpp \
  ui/main_window/views/anime_list.cpp \
  ui/main_window/views/anime_table.cpp \
  ui/main_window/views/now_playing.cpp \
  ui/main_window/views/torrents.cpp \
  ui/settings_dialog.cpp \
  ui/settings_dialog/views/application.cpp \
  ui/settings_dialog/views/recognition.cpp \
  ui/settings_dialog/views/torrent_settings.cpp \
  ui/tray_icon.cpp \
  ui/settings_dialog/components/committable_widget.cpp \
  ui/main_window/components/now_playing_list_model.cpp \
  ui/settings_dialog/components/torrent_rule_model.cpp \
  ui/settings_dialog/components/torrent_rule.cpp \
  ui/settings_dialog/views/rule_import_dialog.cpp \
  src/paths.cpp \
  ui/about.cpp \
  ui/process_details.cpp \
  ui/main_window/components/rss_table_proxy_model.cpp

HEADERS += \
  src/clients/anilist.h \
  src/clients/discord.h \
  src/clients/graphql_query.h \
  src/clients/masato.h \
  src/clients/nekomimi.h \
  src/detection/media_store.h \
  src/detection/window_enumerator.h \
  src/models/media.h \
  src/models/media_list.h \
  src/models/rss_item.h \
  src/models/user.h \
  src/settings.h \
  src/utilities/crash_handler.h \
  src/utilities/file_downloader.h \
  src/utilities/singleton.h \
  ui/anime_panel.h \
  ui/main_window.h \
  ui/main_window/components/media_table_model.h \
  ui/main_window/components/media_table_proxy_model.h \
  ui/main_window/components/progress_item_delegate.h \
  ui/main_window/components/rss_table_model.h \
  ui/main_window/components/score_item_delegate.h \
  ui/main_window/components/status_item_delegate.h \
  ui/main_window/views/airing.h \
  ui/main_window/views/anime_list.h \
  ui/main_window/views/anime_table.h \
  ui/main_window/views/now_playing.h \
  ui/main_window/views/torrents.h \
  ui/settings_dialog.h \
  ui/settings_dialog/views/application.h \
  ui/settings_dialog/views/recognition.h \
  ui/settings_dialog/views/torrent_settings.h \
  ui/tray_icon.h \
  ui/settings_dialog/components/committable_widget.h \
  ui/main_window/components/now_playing_list_model.h \
  ui/settings_dialog/components/torrent_rule_model.h \
  ui/settings_dialog/components/torrent_rule.h \
  ui/settings_dialog/views/rule_import_dialog.h \
  src/paths.h \
  ui/about.h \
  ui/process_details.h \
  ui/main_window/components/rss_table_proxy_model.h

FORMS += \
  ui/anime_panel.ui \
  ui/main_window.ui \
  ui/main_window/views/airing.ui \
  ui/main_window/views/animelist.ui \
  ui/main_window/views/now_playing.ui \
  ui/main_window/views/torrents.ui \
  ui/settings_dialog.ui \
  ui/settings_dialog/views/application.ui \
  ui/settings_dialog/views/recognition.ui \
  ui/settings_dialog/views/torrent_settings.ui \
  ui/settings_dialog/views/rule_import_dialog.ui \
  ui/about.ui \
  ui/process_details.ui

INCLUDEPATH += \
  lib

RESOURCES += \
  res.qrc

TRANSLATIONS = \
  lang/shinjiru_en.ts

linux {
  isEmpty(PREFIX) {
    PREFIX = $$PWD
  }

  bin.path = $${PREFIX}/bin
  bin.files = Shinjiru

  icon.path = $${PREFIX}/share/icons/hicolor/scalable/apps
  icon.extra = $(INSTALL_FILE) res/icon.svg $(INSTALL_ROOT)$${PREFIX}/share/icons/hicolor/scalable/apps/Shinjiru.svg
  icon.uninstall = $(DEL_FILE) $(INSTALL_ROOT)$${PREFIX}/share/icons/hicolor/scalable/apps/Shinjiru.svg

  app.path = $${PREFIX}/share/applications/
  app.extra = cp -f $${PWD}/Shinjiru.desktop.in $${PWD}/me.kazakuri.Shinjiru.desktop; \
              echo \"Exec=$${PREFIX}/bin/Shinjiru\" >> $${PWD}/me.kazakuri.Shinjiru.desktop; \
              ${INSTALL_FILE} $${PWD}/me.kazakuri.Shinjiru.desktop $(INSTALL_ROOT)$${PREFIX}/share/applications/me.kazakuri.Shinjiru.desktop
  app.uninstall = $(DEL_FILE) $(INSTALL_ROOT)$${PREFIX}/share/applications/me.kazakuri.Shinjiru.desktop

  license.path = $${PREFIX}/share/licenses/Shinjiru
  license.files = LICENSE

  INSTALLS += bin icon app license
}
