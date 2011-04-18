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
    tablestandings.cpp \
    settingsdialog.cpp
HEADERS  += mainwindow.h \
    buttonslistwidget.h \
    tablestagestandings.h \
    newtabledialog.h \
    tabletotalstandings.h \
    tablestandings.h \
    settingsdialog.h
FORMS    += mainwindow.ui \
    buttonslistwidget.ui \
    newtabledialog.ui \
    settingsdialog.ui

RESOURCES += \
    resources.qrc

OTHER_FILES += \
    style.css

RC_FILE = kvn-table.rc
    
icon.files += icons/icon128.png
target.path = /usr/local/bin/
icon.path = /usr/local/share/pixmaps/
INSTALLS += target icon

MOC_DIR = ../tmp
OBJECTS_DIR = ../tmp
RCC_DIR = ../tmp
