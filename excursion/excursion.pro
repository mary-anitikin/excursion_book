#-------------------------------------------------
#
# Project created by QtCreator 2023-01-04T18:22:51
#
#-------------------------------------------------

QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = excursion
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
        main.cpp \
        widget.cpp \
    tour.cpp

HEADERS += \
        widget.h \
    tour.h

FORMS += \
        widget.ui

DISTFILES += \
    share/model.sql \
    share/data.sql

SQL_FILES = \
# модели
    share/model.sql \
# данные
    share/data.sql

OTHER_FILES += \
    $$SQL_FILES


DB_NAME=$${PROJ_PREFFIX}db.sqlite

#QMAKE_POST_LINK += \

#    mkdir $$DEPLOY_SHARE_DIR/$$DB_NAME; \
#    rm -rf $$DEPLOY_SHARE_DIR/$$DB_NAME; \
#    touch $$DEPLOY_SHARE_DIR/$$DB_NAME; \
#    chmod a+w $$DEPLOY_SHARE_DIR/$$DB_NAME; \
#    cat $$SQL_FILES | sqlite3 $$DEPLOY_SHARE_DIR/$$DB_NAME; \
#    cp $$DEPLOY_SHARE_DIR/$$DB_NAME;

