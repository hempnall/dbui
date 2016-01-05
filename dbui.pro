TEMPLATE = lib
TARGET = dbui
QT += qml quick sql
CONFIG += qt plugin c++11

TARGET = $$qtLibraryTarget($$TARGET)
uri = com.hempnall.dbui

# Input
SOURCES += \
    dbui_plugin.cpp \
    db.cpp \
    localstoragedatabase.cpp \
    localstoragedatabaseexception.cpp \
    localstoragetablemodel.cpp \
    localstoragetable.cpp

HEADERS += \
    dbui_plugin.h \
    db.h \
    localstoragedatabase.h \
    localstoragedatabaseexception.h \
    localstoragetablemodel.h \
    localstoragetable.h


DISTFILES = qmldir \
    LocalStorageCombo.qml



!equals(_PRO_FILE_PWD_, $$OUT_PWD) {
    copy_qmldir.target = $$OUT_PWD/qmldir
    copy_qmldir.depends = $$_PRO_FILE_PWD_/qmldir
    copy_qmldir.commands = $(COPY_FILE) \"$$replace(copy_qmldir.depends, /, $$QMAKE_DIR_SEP)\" \"$$replace(copy_qmldir.target, /, $$QMAKE_DIR_SEP)\"
    QMAKE_EXTRA_TARGETS += copy_qmldir
    PRE_TARGETDEPS += $$copy_qmldir.target
}

qmldir.files = qmldir \
    LocalStorageCombo.qml

unix {
    installPath = $$[QT_INSTALL_QML]/$$replace(uri, \\., /)
    qmldir.path = $$installPath
    target.path = $$installPath
    INSTALLS += target qmldir
}

