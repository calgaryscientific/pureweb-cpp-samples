# NOTE: Qt solution file is missing deployment steps

HEADERS       = mainwindow.h \
                scribblearea.h \
                pureweb.h \
                QtMessageTickler.h

SOURCES       = main.cpp \
                mainwindow.cpp \
                scribblearea.cpp

CONFIG += qt debug warn_off

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

win64:INCLUDEPATH += "$$(PUREWEB_LIBS)/C++/VS2015/include"
win64:LIBS += $$(PUREWEB_LIBS)/C++/VS2015/lib/CSI.PureWeb.ImageProcessing64d.lib
win64:LIBS += $$(PUREWEB_LIBS)/C++/VS2015/lib/CSI.PureWeb.StateManager64d.lib
win64:LIBS += $$(PUREWEB_LIBS)/C++/VS2015/lib/CSI.Standard64d.lib
win64:LIBS += $$(PUREWEB_LIBS)/C++/VS2015/lib/CSI.Typeless64d.lib

unix:INCLUDEPATH += "$$(CSI_LIB)/CSI.Standard/include/"
unix:INCLUDEPATH += "$$(CSI_LIB)/ISU/include/"
unix:INCLUDEPATH += "$$(CSI_LIB)/CSI.Typeless/include/"
unix:INCLUDEPATH += "$$(PWROOT)/sdk/C++/"


unix:LIBS += $$(PUREWEB_LIBS)/C++/lib/libCSI.PureWeb.ImageProcessing64.so
unix:LIBS += $$(PUREWEB_LIBS)/C++/lib/libCSI.PureWeb.StateManager64.so
unix:LIBS += $$(PUREWEB_LIBS)/C++/lib/libCSI.Standard64.so
unix:LIBS += $$(PUREWEB_LIBS)/C++/lib/libCSI.Typeless64.so

unix:LIBS += $$(PUREWEB_LIBS)/C++/lib/libicudata.so.56
unix:LIBS += $$(PUREWEB_LIBS)/C++/lib/libicui18n.so.56
unix:LIBS += $$(PUREWEB_LIBS)/C++/lib/libicuio.so.56
unix:LIBS += $$(PUREWEB_LIBS)/C++/lib/libicule.so.56
unix:LIBS += $$(PUREWEB_LIBS)/C++/lib/libiculx.so.56
unix:LIBS += $$(PUREWEB_LIBS)/C++/lib/libicutu.so.56
unix:LIBS += $$(PUREWEB_LIBS)/C++/lib/libicuuc.so.56
unix:LIBS += $$(PUREWEB_LIBS)/C++/lib/libjpeg.so.62



unix:!mac{
    QMAKE_LFLAGS += -Wl,--rpath=\\\$\$ORIGIN/../../sdk/Libs/C++/lib
    QMAKE_RPATHDIR += \\\$\$ORIGIN/../../sdk/Libs/C++/lib
    QMAKE_RPATH=
}


release:DESTDIR = $$(STAGINGDIR)/bin
release:OBJECTS_DIR = release/.obj
release:MOC_DIR = release/.moc
release:RCC_DIR = release/.rcc
release:UI_DIR = release/.ui

debug:DESTDIR = debug
debug:OBJECTS_DIR = debug/.obj
debug:MOC_DIR = debug/.moc
debug:RCC_DIR = debug/.rcc
debug:UI_DIR = debug/.ui

