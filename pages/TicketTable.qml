import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import ZTicketAsset
import "../global.js" as Global

Item {

    ListView {
        id: myTicket
        clip: true
        anchors.fill: parent

        // Model: simple list of items
        model: ListModel {

            // Function to populate the model from a JSON array
            function populateModel(jsonArray) {
                for (var i = 0; i < jsonArray.length; i++) {
                    append(jsonArray[i])
                }
            }

            Component.onCompleted: {
                populateModel(Global.tickets);
            }
        }

        // Delegate: how each item appears
        delegate: Item {

            required property var modelData

            width: myTicket.width
            height: 50
            RowLayout {

                anchors.fill: parent
                anchors.leftMargin: 5
                anchors.rightMargin: 5

                Text {
                    Layout.preferredWidth: myTicket.width * 0.10
                    horizontalAlignment: Qt.AlignCenter
                    text: modelData.ticket
                }

                Text {
                    Layout.preferredWidth: myTicket.width * 0.20
                    horizontalAlignment: Qt.AlignCenter
                    text: modelData.dt_open
                }

                Text {
                    Layout.preferredWidth: myTicket.width * 0.20
                    horizontalAlignment: Qt.AlignCenter
                    text: modelData.user
                }

                Text {
                    Layout.preferredWidth: myTicket.width * 0.20
                    horizontalAlignment: Qt.AlignCenter
                    text: modelData.asset
                }

                Text {
                    Layout.preferredWidth: myTicket.width * 0.10
                    horizontalAlignment: Qt.AlignCenter
                    text: String.fromCharCode(modelData.status)
                }

                Text {
                    Layout.preferredWidth: myTicket.width * 0.20
                    horizontalAlignment: Qt.AlignCenter
                    text: modelData.description
                }
            }
            MouseArea {
                anchors.fill: parent
                onClicked:
                {
                    myTicket.currentIndex = modelData.index;
                }
            }
        }
        // Highlight selected item
        highlight: Rectangle {
            color: "transparent"
            border { color: "black"; width: 2 }
        }
        // Add spacing between items
        spacing: 5

        // Optional header
        header: Rectangle {
            width: myTicket.width
            height: 30
            color: "lightgray"
            Text {
                text: qsTr("Ticket List")
                font.bold: true
                anchors.centerIn: parent
            }
        }

        // Optional footer
        footer: Rectangle		{
            width: myTicket.width
            height: 30
            color: "lightgray"
            Text {
                text: myTicket.count + " items"
                anchors.centerIn: parent
            }
        }
    }

    RowLayout {

        Button {
            id: openTicket
            text: qsTr("Open Ticket")
            width: parent.width

            onClicked: {
                openDialog.open()
            }
            Dialog {
                id: openDialog

                x: (parent.width - width) / 2
                y: (parent.height - height) / 2
                parent: Overlay.overlay

                focus: true
                modal: true
                title: qsTr("New Ticket")
                standardButtons: Dialog.Ok | Dialog.Cancel

                onOpened: combolist.model.populateCombo(Global.assets);

                ColumnLayout {
                    spacing: 20
                    anchors.fill: parent

                    Image {
                        id: logo
                        width: 64
                        fillMode: Image.PreserveAspectFit
                        source: "../images/cam-logo.png"
                    }
                    RowLayout {

                        TextField {
                            id: newticketid
                            focus: true
                            placeholderText: qsTr("Short Name")
                            Layout.preferredWidth: openDialog.width * 0.2
                        }
                        TextField {
                            id: newticketdescription
                            focus: true
                            placeholderText: qsTr("Decription")
                            Layout.preferredWidth: openDialog.width * 0.8
                        }
                    }
                    RowLayout {
                        Label {
                            wrapMode: Label.Wrap
                            horizontalAlignment: Qt.AlignHLeft
                            text: qsTr("Asset:")
                        }

                        ComboBox {
                            id: combolist

                            model: ListModel {
                                id: comboModel
                                // Function to populate the model from a JSON array
                                function populateCombo(jsonArray) {
                                    clear();
                                    for (var i = 0; i < jsonArray.length; i++) {
                                        append({text: jsonArray[i]['id']});
                                    }
                                }
                            }
                        }
                    }
                }
                onAccepted:
                {
                    if(newticketid.text !== "" && newticketdescription.text !== "" && combolist.currentText !== ""){
                        backend.newTicket([newticketid.text, Global.logNickname, combolist.currentText, newticketdescription.text]);
                   }
                }
            }
        }
    }

}
