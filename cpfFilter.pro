RC_FILE = cpfFilter.rc
RESOURCES = icons.qrc
contains(QT_CONFIG, opengl): QT += opengl
QT += 
INCLUDEPATH += /c/PFM_ABEv7.0.0_Win64/include
LIBS += -L /c/PFM_ABEv7.0.0_Win64/lib -lCZMIL -lnvutility -lpfm -lgdal -lxml2 -lpoppler -liconv
DEFINES += WIN32 NVWIN3X
CONFIG += console
QMAKE_LFLAGS += 
######################################################################
# Automatically generated by qmake (2.01a) Wed Jan 22 14:02:09 2020
######################################################################

TEMPLATE = app
TARGET = cpfFilter
DEPENDPATH += .
INCLUDEPATH += .

# Input
HEADERS += cpfFilter.hpp \
           cpfFilterDef.hpp \
           cpfFilterHelp.hpp \
           inputPage.hpp \
           inputPageHelp.hpp \
           runPage.hpp \
           specialOptionPage.hpp \
           specialOptionPageHelp.hpp \
           startPage.hpp \
           startPageHelp.hpp \
           typeOptionPage.hpp \
           version.hpp
SOURCES += cpfFilter.cpp \
           inputPage.cpp \
           main.cpp \
           runPage.cpp \
           specialOptionPage.cpp \
           startPage.cpp \
           typeOptionPage.cpp
RESOURCES += icons.qrc
TRANSLATIONS += cpfFilter_xx.ts