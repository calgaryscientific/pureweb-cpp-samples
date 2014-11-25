# NOTE: Qt solution file is missing deployment steps

HEADERS       = mainwindow.h \
                scribblearea.h \
                pureweb.h \
                QtMessageTickler.h

SOURCES       = main.cpp \
                mainwindow.cpp \
                scribblearea.cpp

CONFIG += qt debug warn_off

win32:INCLUDEPATH += "$$(PUREWEB_LIBS)/C++/VS2010/include"
win32:LIBS += $$(PUREWEB_LIBS)/C++/VS2010/lib/CSI.PureWeb.ImageProcessing32d.lib
win32:LIBS += $$(PUREWEB_LIBS)/C++/VS2010/lib/CSI.PureWeb.StateManager32d.lib
win32:LIBS += $$(PUREWEB_LIBS)/C++/VS2010/lib/CSI.Standard32d.lib
win32:LIBS += $$(PUREWEB_LIBS)/C++/VS2010/lib/CSI.Typeless32d.lib

unix:INCLUDEPATH += "$$(PUREWEB_LIBS)/C++/include"

unix:LIBS += $$(PUREWEB_LIBS)/C++/lib/libCSI.PureWeb.ImageProcessing64.so
unix:LIBS += $$(PUREWEB_LIBS)/C++/lib/libCSI.PureWeb.StateManager64.so
unix:LIBS += $$(PUREWEB_LIBS)/C++/lib/libCSI.Standard64.so
unix:LIBS += $$(PUREWEB_LIBS)/C++/lib/libCSI.Typeless64.so

unix:LIBS += $$(PUREWEB_LIBS)/C++/lib/libicudata.so.46
unix:LIBS += $$(PUREWEB_LIBS)/C++/lib/libicui18n.so.46
unix:LIBS += $$(PUREWEB_LIBS)/C++/lib/libicuio.so.46
unix:LIBS += $$(PUREWEB_LIBS)/C++/lib/libicule.so.46
unix:LIBS += $$(PUREWEB_LIBS)/C++/lib/libiculx.so.46
unix:LIBS += $$(PUREWEB_LIBS)/C++/lib/libicutu.so.46
unix:LIBS += $$(PUREWEB_LIBS)/C++/lib/libicuuc.so.46
unix:LIBS += $$(PUREWEB_LIBS)/C++/lib/libjpeg.so.62



#unix:!mac{
#  QMAKE_LFLAGS += -Wl,-rpath=\\\$\$ORIGIN
#  QMAKE_LFLAGS += -Wl,-rpath=\\\$\$ORIGIN/../../../SDK/Libs/C++/lib
#  QMAKE_LFLAGS_RPATH=
#}


release:DESTDIR = release
release:OBJECTS_DIR = release/.obj
release:MOC_DIR = release/.moc
release:RCC_DIR = release/.rcc
release:UI_DIR = release/.ui

debug:DESTDIR = debug
debug:OBJECTS_DIR = debug/.obj
debug:MOC_DIR = debug/.moc
debug:RCC_DIR = debug/.rcc
debug:UI_DIR = debug/.ui

