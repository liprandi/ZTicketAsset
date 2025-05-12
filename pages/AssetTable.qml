import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import ZTicketAsset
import "../global.js" as Global

Item {

    ListView {
        id: myView
        anchors.fill: parent
        clip: true

        // Model: simple list of items
        model: ListModel {
            id: testmodel


            // Function to populate the model from a JSON array
            function populateModel(jsonArray) {
                for (var i = 0; i < jsonArray.length; i++) {
                    append(jsonArray[i])
                }
            }

            Component.onCompleted: {
                populateModel(Global.assets);
            }

        }

        // Delegate: how each item appears
        delegate: Item {
            id: listItem

            required property var modelData

            width: myView.width
            height: 50
            RowLayout {

                anchors.fill: parent
                anchors.leftMargin: 5
                anchors.rightMargin: 5

                Text {
                    Layout.preferredWidth: myView.width * 0.20
                    horizontalAlignment: Qt.AlignCenter
                    text: modelData.id
                }



                Text {
                    Layout.preferredWidth: myView.width * 0.20
                    horizontalAlignment: Qt.AlignCenter
                    text: modelData.type
                }


                Layout.preferredWidth: myView.width * 0.20
                Text {
                    horizontalAlignment: Qt.AlignCenter
                    text: modelData.model
                }

                Text {
                    Layout.preferredWidth: myView.width * 0.20
                    horizontalAlignment: Qt.AlignCenter
                    text: modelData.description
                }


                Text {
                    Layout.preferredWidth: myView.width * 0.20
                    horizontalAlignment: Qt.AlignCenter
                    text: modelData.date
                }
            }
            MouseArea {
                anchors.fill: listItem
                onClicked:
                {
                    myView.currentIndex = modelData.index;
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
            width: myView.width
            height: 30
            color: "lightgray"
            Text {
                text: qsTr("Assets List")
                font.bold: true
                anchors.centerIn: parent
            }
        }

        // Optional footer
        footer: Rectangle {
            width: myView.width
            height: 30
            color: "lightgray"
            Text {
                text: myView.count + " items"
                anchors.centerIn: parent
            }
            Button {
                id: addButton
                text: qsTr("Add Asset")
                width: parent.width

                onClicked: assetDialog.open()

                Dialog {
                    id: assetDialog

                    x: (parent.width - width) / 2
                    y: (parent.height - height) / 2
                    parent: Overlay.overlay

                    focus: true
                    modal: true
                    title: qsTr("New Asset")
                    standardButtons: Dialog.Ok | Dialog.Cancel

                    ColumnLayout {
                        spacing: 20
                        anchors.fill: parent
                        Label {
                            elide: Label.ElideRight
                            text: qsTr("Asset info")
                            Layout.fillWidth: true
                        }

                        Image {
                            id: logo
                            width: parent.width / 2
                            fillMode: Image.PreserveAspectFit
                            source: "../images/cam-logo.png"
                        }
                        RowLayout {

                            TextField {
                                id: newassetid
                                focus: true
                                placeholderText: qsTr("ID")
                                Layout.preferredWidth: assetDialog.width * 0.30
                            }
                            TextField {
                                id: newassettype
                                focus: true
                                placeholderText: qsTr("Type")
                                Layout.preferredWidth: assetDialog.width * 0.30
                            }
                            TextField {
                                id: newassetmodel
                                focus: true
                                placeholderText: qsTr("Model")
                                Layout.preferredWidth: assetDialog.width * 0.30
                            }
                        }

                        TextField {
                            id: newassetdescription
                            focus: true
                            placeholderText: qsTr("Description")
                            Layout.fillWidth: true
                        }
                        RowLayout {
                            CheckBox {
                                id: allusers
                                focus: true
                                text: qsTr("All users:")
                                Layout.preferredWidth: assetDialog.width * 0.20
                            }
                            Rectangle {
                                width: 400
                                height: 400

                                DatePicker {
                                    id: datepicker
                                }
                            }
                        }
                    }
                    onAccepted:
                    {
                        if(newassetid.text !== "" && newassettype.text !== "" && newassetmodel.text !== ""){
                            backend.addAsset([newassetid.text, newassettype.text, newassetmodel.text, newassetdescription.text], datepicker.selected_date);
                            if(allusers.checkState !== Qt.Checked)
                                backend.addLink([newassetid.text, Global.logEmail]);
                            else
                                backend.addLink([newassetid.text, "."]);
                        }
                    }

                }
            }
        }
    }
}
