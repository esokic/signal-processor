QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

CONFIG += c++17


# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    ansamblsignala.cpp \
    exportfilesetting.cpp \
    layout.cpp \
    main.cpp \
    mainwindow.cpp \
    manipulacijaprocesorima.cpp \
    prikaz.cpp \
    procesor.cpp \
    qcustomplot/qcustomplot.cpp \
    selectsignalsdialog.cpp \
    signal.cpp \
    signalsexport.cpp

HEADERS += \
    ansamblsignala.h \
    exportfilesetting.h \
    layout.h \
    mainwindow.h \
    manipulacijaprocesorima.h \
    nlohmann/json.hpp \
    nlohmann/json_fwd.hpp \
    prikaz.h \
    procesor.h \
    qcustomplot/qcustomplot.h \
    selectsignalsdialog.h \
    signal.h \
    signalsexport.h

FORMS += \
    mainwindow.ui \
    manipulacijaprocesorima.ui \
    procesor.ui \
    selectsignalsdialog.ui \
    signalsexport.ui

LIBS += -lmatio -larmadillo

INCLUDEPATH += qcustomplot/

include(QtXlsxWriter-master/src/xlsx/qtxlsx.pri)

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
