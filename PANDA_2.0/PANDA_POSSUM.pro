QT       += core gui
qtHaveModule(printsupport): QT += printsupport
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    canoe.cpp \
    cell.cpp \
    editdesign.cpp \
    loadcase.cpp \
    main.cpp \
    mainwindow.cpp \
    model.cpp \
    printview.cpp \
    runanalysis.cpp \
    spreadsheet.cpp \
    warningmessage.cpp

HEADERS += \
    canoe.h \
    cell.h \
    constants.h \
    editdesign.h \
    loadcase.h \
    mainwindow.h \
    model.h \
    printview.h \
    runanalysis.h \
    spreadsheet.h \
    warningmessage.h

FORMS += \
    editdesign.ui \
    runanalysis.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    PANDA_POSSUM.qrc

DISTFILES += \
    c4table.txt \
    inputsetup.txt \
    inputtable.txt \
    output.txt \
    weighttable.txt
