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
        let url = "https://www.cam-carmagnola.com/";
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
            backend.assetsUpdated.connect(handleNewAsset);
            backend.ticketsUpdated.connect(handleNewTicket);
        }
    }

    function handleLogin()
    {
        Global.logJson = backend.credentials;
        console.log("Received JSON:", Global.logJson);
        login(Global.logJson);
        backend.readAssets();
        backend.readTickets();
        backend.readEmojis();
        Global.assets = backend.assets;
        Global.tickets = backend.tickets;
        Global.emojis = backend.emojis;
    }

    function handleUpdate()
    {
        backend.readAssets();
        backend.readTickets();
        backend.readSteps("");
        Global.assets = backend.assets;
        Global.tickets = backend.tickets;
        Global.steps = backend.steps;
    }

    function handleNewAsset()
    {

    }
    function handleNewTicket()
    {

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
                listViewOption.currentIndex = -1
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

            ToolButton {
                action: navigateBackAction
                visible: window.portraitMode
            }
            AbstractButton {
                Layout.preferredWidth: 25
                Layout.preferredHeight: 25
                Layout.alignment: Qt.AlignVCenter

                Rectangle {
                    anchors.fill: parent
                    radius: 4
                    color: "#192CDE85"
                    border.color: "#DDE2E8"
                    border.width: 1
                }

                Image {
                    source: "qrc:/icons/update.svg"
                    fillMode: Image.PreserveAspectFit
                    anchors.fill: parent
                    sourceSize.width: width
                    sourceSize.height: height
                }

                onClicked: {
                    root.colors.refreshCurrentPage()
                    root.colorViewUsers.refreshCurrentPage()
                }
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
                ListElement { icon: "images/asset.png"; title: qsTr("Assets"); source: "qrc:/pages/AssetTable.qml" }
                ListElement { icon: "images/ticket.png"; title: qsTr("Tickets"); source: "qrc:/pages/TicketTable.qml" }
                ListElement { icon: "images/ticket.png"; title: qsTr("Tickets"); source: "qrc:/pages/TicketTree.qml" }
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
                source: "images/cam.svg"
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
