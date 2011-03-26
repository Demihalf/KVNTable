QT       += core gui
CONFIG   += warn_on \
            qt

TARGET   = ../bin/kvn-table
TEMPLATE = app

SOURCES  += main.cpp\
            mainwindow.cpp \
    buttonslistwidget.cpp \
    tablestagestandings.cpp \
    tabletotalstandings.cpp \
    newtabledialog.cpp \
    tablestandings.cpp
HEADERS  += mainwindow.h \
    buttonslistwidget.h \
    tablestagestandings.h \
    newtabledialog.h \
    tabletotalstandings.h \
    tablestandings.h
FORMS    += mainwindow.ui \
    buttonslistwidget.ui \
    newtabledialog.ui

MOC_DIR = ../tmp
OBJECTS_DIR = ../tmp
RCC_DIR = ../tmp
RESOURCES += \
    resources.qrc

OTHER_FILES += \
    style.css
