cmake_minimum_required(VERSION 3.16)

project(ZTicketAsset VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets Network LinguistTools)
find_package(Qt6 REQUIRED COMPONENTS Core)
find_package(Qt6 REQUIRED COMPONENTS Widgets)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

add_compile_definitions(
    QT_DEPRECATED_WARNINGS
    QT_NO_PRIVATE_MODULE_WARNING
)

set(PROJECT_SOURCES
    main.cpp
    mainwindow.cpp
    zlogin.cpp
    zbackend.cpp
    sidebar.cpp
    zdatabase.cpp
    zemoji.cpp
    zjsontablewidget.cpp
    zassetswidget.cpp
    zticketswidget.cpp
    zstepswidget.cpp

    mainwindow.h
    zlogin.h
    zbackend.h
    sidebar.h
    zdatabase.h
    zemoji.h
    zjsontablewidget.h
    zassetswidget.h
    zticketswidget.h
    zstepswidget.h
    utility/zplaceholderlineedit.h

    mainwindow.ui
    zlogin.ui
    zassetswidget.ui
    zticketswidget.ui
    zstepswidget.ui
    resource.qrc
)

set(TS_FILES translations/app_it.ts)

# Add the executable
qt_add_executable(ZTicketAsset
    MANUAL_FINALIZATION
    ${PROJECT_SOURCES}
    dlgeditasset.h dlgeditasset.cpp dlgeditasset.ui
)

# lupdate: extract translatable strings
qt_add_lupdate(ZTicketAsset
    TS_FILES ${TS_FILES}
    SOURCES ${PROJECT_SOURCES}
)

# Generate translation binary files (.qm) from .ts
qt_add_translations(ZTicketAsset TS_FILES ${TS_FILES})

target_link_libraries(ZTicketAsset PRIVATE
    Qt6::Core
    Qt6::Gui
    Qt6::Widgets
    Qt6::Network
)
target_link_libraries(ZTicketAsset PRIVATE Qt6::Widgets)

qt_finalize_executable(ZTicketAsset)
