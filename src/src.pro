QT       += core gui
CONFIG   += warn_on \
            qt

TARGET   = ../bin/kvn-table
TEMPLATE = app

SOURCES  += main.cpp\
            mainwindow.cpp \
    buttonslistwidget.cpp \
    newtabledialog.cpp \
    settingsdialog.cpp \
    tablecontroller.cpp \
    stagestandingsmodel.cpp \
    tablecontainer.cpp \
    team.cpp \
    totalstandingsmodel.cpp
HEADERS  += mainwindow.h \
    buttonslistwidget.h \
    newtabledialog.h \
    settingsdialog.h \
    tablecontroller.h \
    stagestandingsmodel.h \
    tablecontainer.h \
    team.h \
    totalstandingsmodel.h
FORMS    += mainwindow.ui \
    buttonslistwidget.ui \
    newtabledialog.ui \
    settingsdialog.ui

RESOURCES += \
    resources.qrc

OTHER_FILES += \
    style.css \
    about_string.txt

RC_FILE = kvn-table.rc
    
icon.files += icons/icon128.png
target.path = /usr/local/bin/
icon.path = /usr/local/share/pixmaps/
INSTALLS += target icon

MOC_DIR = ../tmp
OBJECTS_DIR = ../tmp
RCC_DIR = ../tmp
















