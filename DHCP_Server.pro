QT += core gui network widgets

TARGET = DHCP_Server
TEMPLATE = app

SOURCES += main.cpp\
    dhcp_server.cpp \
    dhcp_message.cpp \
    window_fake_client.cpp \
    client.cpp \
    form_assignments.cpp \
    model_assignments.cpp \
    window_main.cpp \
    static_assignment_t.cpp \
    dhcp_assignments.cpp

HEADERS  += \
    dhcp_server.h \
    dhcp_message.h \
    dhcp_option_t.h \
    window_fake_client.h \
    client.h \
    form_assignments.h \
    model_assignments.h \
    global_config.h \
    window_main.h \
    static_assignment_t.h \
    dhcp_assignments.h

FORMS    += \
    window_fake_client.ui \
    form_assignments.ui \
    window_main.ui
