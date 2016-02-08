TEMPLATE = app
TARGET = tst_example
CONFIG += warn_on qmltestcase
SOURCES += tst_example.cpp

QML2_IMPORT_PATH = $$OUT_PWD


DISTFILES += \
    tst_createdeletedb.qml \
    tst_cruddata.qml \
    create_data.js \
    tst_cruddata2.qml
