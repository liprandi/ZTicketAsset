// CAM Software per Ticket Assets
//

pragma ComponentBehavior: Bound

import QtCore
import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import ZTicketAsset 1.0

import "global.js" as Global

import "." as App


ApplicationWindow {
    id: window
    width: 640
    height: 480
    visible: true
    title: qsTr("CAM Ticket Asset")

    //! [orientation]
    readonly property bool portraitMode: !orientationCheckBox.checked || window.width < window.height
    //! [orientation]

    function help() {
        let url = "https://www.liprandi.com/";
        Qt.openUrlExternally(url)
    }

    required property var builtInStyles


    Settings {
        id: settings
        property string style
    }

    ZBackEnd
    {
        id: backend

        Component.onCompleted:
        {
                // Connect the C++ signal to a QML function
            backend.credentialsChanged.connect(handleLogin);
        }


    }

    function handleLogin(jsonData)
    {
        console.log("Received JSON:", jsonData);
        Global.logJson = JSON.parse(jsonData);
        login( Global.logJson);
    }
    function login(credentials)
    {
        const name = credentials['name'].toLowerCase();
        const n = name.charAt(0).toUpperCase() + name.slice(1);
        const s = credentials['surname'].toUpperCase();

        Global.logUser = n + " " + s;
        Global.logEmail = credentials['email'];
        Global.logNickname = credentials['nickname'];
        userLogged.text = Global.logUser;
    }

    Shortcut {
        sequences: ["Esc", "Back"]
        enabled: stackView.depth > 1
        onActivated: navigateBackAction.trigger()
    }

    Shortcut {
        sequence: StandardKey.HelpContents
        onActivated: window.help()
    }

    Action {
        id: navigateBackAction
        icon.name: stackView.depth > 1 ? "back" : "drawer"
        onTriggered: {
            if (stackView.depth > 1) {
                stackView.pop()
                listView.currentIndex = -1
            } else {
                drawer.open()
            }
        }
    }

    Shortcut {
        sequence: "Menu"
        onActivated: optionsMenuAction.trigger()
    }

    Action {
        id: optionsMenuAction
        icon.name: "menu"
        onTriggered: optionsMenu.open()
    }

    header: App.ToolBar {
        RowLayout {
            spacing: 20
            anchors.fill: parent
            anchors.leftMargin: !window.portraitMode ? drawer.width : undefined

            property var menuItems: ["Item A", "Item B", "Item C", "Another Item"]

            ToolButton {
                action: navigateBackAction
                visible: window.portraitMode
            }

            Label {
                id: titleLabel
                text: listViewOption.currentItem ? (listViewOption.currentItem as ItemDelegate).text : qsTr("Ticket Assets")
                font.pixelSize: 20
                elide: Label.ElideRight
                horizontalAlignment: Qt.AlignHCenter
                verticalAlignment: Qt.AlignVCenter
                Layout.fillWidth: true
            }
            Label {
                id: userLogged
                text: qsTr("No user")
                font.pixelSize: 20
                elide: Label.ElideRight
                horizontalAlignment: Qt.AlignHright
                verticalAlignment: Qt.AlignVCenter
                Layout.fillWidth: true
            }


            ToolButton {
                action: optionsMenuAction

                Menu {
                    id: optionsMenu
                    x: parent.width - width
                    transformOrigin: Menu.TopRight

                    Action {
                        text: qsTr("Settings")
                        onTriggered: settingsDialog.open()
                    }
                    Action {
                        text: qsTr("Help")
                        onTriggered: window.help()
                    }
                    Action {
                        text: qsTr("About")
                        onTriggered: aboutDialog.open()
                    }
                    Repeater {
                        model: menuItems
                        delegate: MenuItem {
                            text: modelData
                        }
                    }
                }
            }
        }
    }

    Drawer {
        id: drawer

        width: Math.min(window.width, window.height) / 3 * 2
        height: window.height
        modal: window.portraitMode
        interactive: window.portraitMode ? (stackView.depth === 1) : false
        position: window.portraitMode ? 0 : 1
        visible: !window.portraitMode

        ListView {
            id: listViewOption

            focus: true
            currentIndex: -1
            anchors.fill: parent

            signal listViewSignal()

            model: ListModel {
                ListElement { icon: "images/user.png"; title: qsTr("Login"); source: "qrc:/pages/LoginPage.qml" }
                ListElement { icon: "images/item.png"; title: qsTr("Table"); source: "qrc:/pages/TablePage.qml" }
/*
                ListElement { icon: "images/item.png"; title: qsTr("Button"); source: "qrc:/pages/ButtonPage.qml" }
                ListElement { icon: "images/item.png"; title: qsTr("CheckBox"); source: "qrc:/pages/CheckBoxPage.qml" }
                ListElement { icon: "images/item.png"; title: qsTr("ComboBox"); source: "qrc:/pages/ComboBoxPage.qml" }
                ListElement { icon: "images/item.png"; title: qsTr("DelayButton"); source: "qrc:/pages/DelayButtonPage.qml" }
                ListElement { icon: "images/item.png"; title: qsTr("Dial"); source: "qrc:/pages/DialPage.qml" }
                ListElement { icon: "images/item.png"; title: qsTr("Dialog"); source: "qrc:/pages/DialogPage.qml" }
                ListElement { icon: "images/item.png"; title: qsTr("Delegates"); source: "qrc:/pages/DelegatePage.qml" }
                ListElement { icon: "images/item.png"; title: qsTr("Frame"); source: "qrc:/pages/FramePage.qml" }
                ListElement { icon: "images/item.png"; title: qsTr("GroupBox"); source: "qrc:/pages/GroupBoxPage.qml" }
                ListElement { icon: "images/item.png"; title: qsTr("PageIndicator"); source: "qrc:/pages/PageIndicatorPage.qml" }
                ListElement { icon: "images/item.png"; title: qsTr("ProgressBar"); source: "qrc:/pages/ProgressBarPage.qml" }
                ListElement { icon: "images/item.png"; title: qsTr("RadioButton"); source: "qrc:/pages/RadioButtonPage.qml" }
                ListElement { icon: "images/item.png"; title: qsTr("RangeSlider"); source: "qrc:/pages/RangeSliderPage.qml" }
                ListElement { icon: "images/item.png"; title: qsTr("ScrollBar"); source: "qrc:/pages/ScrollBarPage.qml" }
                ListElement { icon: "images/item.png"; title: qsTr("ScrollIndicator"); source: "qrc:/pages/ScrollIndicatorPage.qml" }
                ListElement { icon: "images/item.png"; title: qsTr("Slider"); source: "qrc:/pages/SliderPage.qml" }
                ListElement { icon: "images/item.png"; title: qsTr("SpinBox"); source: "qrc:/pages/SpinBoxPage.qml" }
                ListElement { icon: "images/item.png"; title: qsTr("StackView"); source: "qrc:/pages/StackViewPage.qml" }
                ListElement { icon: "images/item.png"; title: qsTr("SwipeView"); source: "qrc:/pages/SwipeViewPage.qml" }
                ListElement { icon: "images/item.png"; title: qsTr("Switch"); source: "qrc:/pages/SwitchPage.qml" }
                ListElement { icon: "images/item.png"; title: qsTr("TabBar"); source: "qrc:/pages/TabBarPage.qml" }
                ListElement { icon: "images/item.png"; title: qsTr("TextArea"); source: "qrc:/pages/TextAreaPage.qml" }
                ListElement { icon: "images/item.png"; title: qsTr("TextField"); source: "qrc:/pages/TextFieldPage.qml" }
                ListElement { icon: "images/item.png"; title: qsTr("ToolTip"); source: "qrc:/pages/ToolTipPage.qml" }
                ListElement { icon: "images/item.png"; title: qsTr("Tumbler"); source: "qrc:/pages/TumblerPage.qml" }
*/
            }

            delegate: ItemDelegate {
                id: delegateItem
                width: ListView.view.width
                text: title
                highlighted: ListView.isCurrentItem
                icon.source: model.icon
                icon.height: 32
                icon.width: 32
                icon.color: Qt.rgba(0.0,0.0,0.0,0.0)

                required property int index
                required property var model
                required property string title
                required property string source

                onClicked: {
                    listViewOption.currentIndex = index
                    stackView.push(source)
                    if (window.portraitMode)
                        drawer.close()
                }
            }

            ScrollIndicator.vertical: ScrollIndicator { }
        }
    }

    StackView {
        id: stackView

        anchors.fill: parent
        anchors.leftMargin: !window.portraitMode ? drawer.width : undefined

        initialItem: Pane {
            id: pane

            Image {
                id: logo
                width: pane.availableWidth / 2
                height: pane.availableHeight / 2
                anchors.centerIn: parent
                anchors.verticalCenterOffset: -50
                fillMode: Image.PreserveAspectFit
                source: "images/cam-logo.png"
            }

            Label {
                text: qsTr("CAM S.r.l. Via Monteu Roero 12/4 Carmagnola Italy")
                anchors.margins: 20
                anchors.top: logo.bottom
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.bottom: arrow.top
                horizontalAlignment: Label.AlignHCenter
                verticalAlignment: Label.AlignVCenter
                wrapMode: Label.Wrap
            }

            Image {
                id: arrow
                source: "images/arrow.png"
                anchors.left: parent.left
                anchors.bottom: parent.bottom
                visible: window.portraitMode
            }
        }
    }

    Dialog {
        id: settingsDialog
        x: Math.round((window.width - width) / 2)
        y: Math.round(window.height / 6)
        modal: true
        focus: true
        title: qsTr("Settings")

        standardButtons: Dialog.Ok | Dialog.Cancel
        onAccepted: {
            settings.style = styleBox.displayText
            settingsDialog.close()
        }
        onRejected: {
            styleBox.currentIndex = styleBox.styleIndex
            settingsDialog.close()
        }

        contentItem: ColumnLayout {
            id: settingsColumn
            spacing: 20

            RowLayout {
                spacing: 10

                Label {
                    text: qsTr("Style:")
                }

                ComboBox {
                    id: styleBox
                    property int styleIndex: -1
                    model: window.builtInStyles
                    Component.onCompleted: {
                        styleIndex = find(settings.style, Qt.MatchFixedString)
                        if (styleIndex !== -1)
                            currentIndex = styleIndex
                    }
                    Layout.fillWidth: true
                }
            }

            CheckBox {
                id: orientationCheckBox
                text: qsTr("Enable Landscape")
                checked: false
                Layout.fillWidth: true
            }

            Label {
                text: qsTr("Restart required")
                color: "#e41e25"
                opacity: styleBox.currentIndex !== styleBox.styleIndex ? 1.0 : 0.0
                horizontalAlignment: Label.AlignHCenter
                verticalAlignment: Label.AlignVCenter
                Layout.fillWidth: true
                Layout.fillHeight: true
            }
        }
    }

    Dialog {
        id: aboutDialog
        modal: true
        focus: true
        title: qsTr("About")
        x: (window.width - width) / 2
        y: window.height / 6
        width: Math.min(window.width, window.height) / 3 * 2
        contentHeight: aboutColumn.height

        Column {
            id: aboutColumn
            spacing: 20

            Label {
                width: aboutDialog.availableWidth
                text: qsTr("The Qt Quick Controls module delivers the next generation user interface controls based on Qt Quick.")
                wrapMode: Label.Wrap
                font.pixelSize: 12
            }

            Label {
                width: aboutDialog.availableWidth
                text: qsTr("In comparison to Qt Quick Controls 1, Qt Quick Controls "
                    + "are an order of magnitude simpler, lighter, and faster.")
                wrapMode: Label.Wrap
                font.pixelSize: 12
            }
        }
    }
}
