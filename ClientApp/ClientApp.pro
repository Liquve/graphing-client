QT += widgets network

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    authform.cpp \
    client_api.cpp \
    forgotform.cpp \
    main.cpp \
    manager_of_forms.cpp \
    regform.cpp \
    taskform.cpp

HEADERS += \
    authform.h \
    client_api.h \
    forgotform.h \
    manager_of_forms.h \
    regform.h \
    taskform.h

FORMS += \
    authform.ui \
    forgotform.ui \
    manager_of_forms.ui \
    regform.ui \
    taskform.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    res.qrc
