QT       += core gui widgets printsupport
CONFIG += c++17
DEFINES += QT_DEPRECATED_WARNINGS

SOURCES += \
    ansamblsignala.cpp \
    exportfilesetting.cpp \
    layout.cpp \
    main.cpp \
    mainwindow.cpp \
    manipulacijaprocesorima.cpp \
    mycustomplot.cpp \
    prikaz.cpp \
    procesor.cpp \
    qcustomplot/qcustomplot.cpp \
    selectsignalsdialog.cpp \
    signall.cpp \
    signalsexport.cpp


HEADERS += \
    ansamblsignala.h \
    exportfilesetting.h \
    layout.h \
    mainwindow.h \
    manipulacijaprocesorima.h \
    mycustomplot.h \
    nlohmann/json.hpp \
    nlohmann/json_fwd.hpp \
    prikaz.h \
    procesor.h \
    qcustomplot/qcustomplot.h \
    selectsignalsdialog.h \
    signall.h \
    signalsexport.h



FORMS += \
    mainwindow.ui \
    manipulacijaprocesorima.ui \
    procesor.ui \
    selectsignalsdialog.ui \
    signalsexport.ui

FORMS += \
    mainwindow.ui \
    manipulacijaprocesorima.ui \
    procesor.ui \
    selectsignalsdialog.ui \
    signalsexport.ui


INCLUDEPATH += qcustomplot/
INCLUDEPATH += nlohmann/
INCLUDEPATH += armadillo-12.8.3/include/

win32 {
    INCLUDEPATH += ../matio-master/src
    INCLUDEPATH += ../matio-master/build/src
    LIBS += -L../matio-master/build -lmatio
}

unix {
    LIBS += -lmatio -larmadillo
}

include(QtXlsxWriter-master/src/xlsx/qtxlsx.pri)

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

