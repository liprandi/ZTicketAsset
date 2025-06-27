cmake_minimum_required(VERSION 3.16)

project(ZTicketAsset VERSION 0.1 LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(INCLUDE_DIR ./include .)

add_subdirectory(ZStyle)

find_package(Qt6 REQUIRED COMPONENTS Core Gui Quick QuickControls2 Widgets)

qt_standard_project_setup(REQUIRES 6.8)

qt_add_executable(ZTicketAsset
    main.cpp
    zdatabase.h
    zdatabase.cpp
)

qt_add_qml_module(ZTicketAsset
    URI ZTicketAsset
    NO_RESOURCE_TARGET_PATH
    VERSION 1.0
    QML_FILES
        zticketasset.qml
        ToolBar.qml
        pages/LoginPage.qml
#        pages/TablePage.qml
#        pages/BusyIndicatorPage.qml
#        pages/ButtonPage.qml
#        pages/CheckBoxPage.qml
#        pages/ComboBoxPage.qml
#        pages/DelayButtonPage.qml
#        pages/DelegatePage.qml
#        pages/DialPage.qml
#        pages/DialogPage.qml
#        pages/FramePage.qml
#        pages/GroupBoxPage.qml
#        pages/PageIndicatorPage.qml
#        pages/ProgressBarPage.qml
#        pages/RadioButtonPage.qml
#        pages/RangeSliderPage.qml
#        pages/ScrollBarPage.qml
#        pages/ScrollIndicatorPage.qml
        pages/ScrollablePage.qml
#        pages/SliderPage.qml
#        pages/SpinBoxPage.qml
#        pages/StackViewPage.qml
#        pages/SwipeViewPage.qml
#        pages/SwitchPage.qml
#        pages/TabBarPage.qml
#        pages/TextAreaPage.qml
#        pages/TextFieldPage.qml
#        pages/ToolTipPage.qml
#        pages/TumblerPage.qml
        pages/TicketTree.qml
        pages/TicketTable.qml
        pages/AssetTable.qml
        pages/DatePicker.qml

global.js
    SOURCES

        emoji_map.h
        zbackend.h
        zbackend.cpp

    RESOURCES
        icons/zticketasset/20x20/back.png
        icons/zticketasset/20x20/drawer.png
        icons/zticketasset/20x20/menu.png
        icons/zticketasset/20x20/item.png
        icons/zticketasset/20x20@2/back.png
        icons/zticketasset/20x20@2/drawer.png
        icons/zticketasset/20x20@2/menu.png
        icons/zticketasset/20x20@2/item.png
        icons/zticketasset/20x20@3/back.png
        icons/zticketasset/20x20@3/drawer.png
        icons/zticketasset/20x20@3/menu.png
        icons/zticketasset/20x20@3/item.png
        icons/zticketasset/20x20@4/back.png
        icons/zticketasset/20x20@4/drawer.png
        icons/zticketasset/20x20@4/menu.png
        icons/zticketasset/20x20@4/item.png
        icons/zticketasset/index.theme
        images/arrow.png
        images/arrow@2x.png
        images/arrow@3x.png
        images/arrow@4x.png
        images/arrows.png
        images/arrows@2x.png
        images/arrows@3x.png
        images/arrows@4x.png
        images/cam.svg
        images/cam@2x.svg
        images/cam@3x.svg
        images/cam@4x.svg
        images/user.png
        images/item.png
        images/asset.png
        images/ticket.png
        icons/update.svg
        qtquickcontrols2.conf
)

# Qt for iOS sets MACOSX_BUN
#DLE_GUI_IDENTIFIER automatically since Qt 6.1.

# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
set_target_properties(ZTicketAsset PROPERTIES
#    MACOSX_BUNDLE_GUI_IDENTIFIER com.example.ZTicketAsset
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

target_link_libraries(ZTicketAsset PUBLIC
    Qt6::Core
    Qt6::Gui
    Qt6::Quick
    Qt6::QuickControls2
    Qt6::Widgets
)

if(UNIX AND NOT APPLE AND CMAKE_CROSSCOMPILING)
    find_package(Qt6 REQUIRED COMPONENTS QuickTemplates2)

    # Work around QTBUG-86533
    target_link_libraries(ZTicketAsset PRIVATE Qt6::QuickTemplates2)
endif()

include(GNUInstallDirs)

install(TARGETS ZTicketAsset
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
