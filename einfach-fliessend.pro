#-------------------------------------------------
#
# Project created by QtCreator 2014-09-21T15:58:53
#
#-------------------------------------------------

QT       += core gui\
         sql\
         widgets

TARGET = einfach-fliessend
TEMPLATE = app


SOURCES += main.cpp\
        fluessigmain.cpp \
    centralwidget.cpp \
    disambiguationwindow.cpp \
    customisationwindow.cpp \
    loginwizard.cpp \
    questionstabwidget.cpp \
    fontcolorstabwidget.cpp \
    drilldialog.cpp \
    exportdialog.cpp \
    importdialog.cpp

HEADERS  += fluessigmain.h \
    centralwidget.h \
    disambiguationwindow.h \
    customisationwindow.h \
    loginwizard.h \
    questionstabwidget.h \
    fontcolorstabwidget.h \
    drilldialog.h \
    exportdialog.h \
    importdialog.h \
    namespaces.h

RESOURCES += \
    ressources.qrc

TRANSLATIONS += languages/fluessig_fr.ts \
    languages/fluessig_en.ts
