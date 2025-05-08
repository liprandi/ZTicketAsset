pragma ComponentBehavior: Bound

import QtQuick
import QtQuick.Controls
import Qt.labs.qmlmodels
import QtQuick.Layouts
import ZTicketAsset
import "../global.js" as Global

import ZStyle

Item {
    id: tablepage

    ColumnLayout {
        // The main application layout
        anchors.fill :parent

        ListView {
            id: myModel // Give your model an ID so you can refer to it
            model: TableView


            // Function to populate the model from a JSON array
            function populateModel(jsonArray) {
                clear() // Clear any existing data
                for (var i = 0; i < jsonArray.length; i++) {
                    append(jsonArray[i])
                }
            }

            // Example of how you might load the JSON data (replace with your actual loading mechanism)
            Component.onCompleted: {
                populateModel(Global.assets);
            }

            spacing: 15
            clip: true

            header:  Rectangle {
                height: 32
                width: parent.width
                color: "#F0F1F3"

                RowLayout {
                    anchors.fill: parent

                    component HeaderText : Text {
                        Layout.alignment: Qt.AlignVCenter
                        horizontalAlignment: Qt.AlignHCenter

                        font.pixelSize: 12
                        color: "#667085"
                    }
                    HeaderText {
                        id: headerId
                        text: qsTr("id")
                        Layout.preferredWidth: myModel.width * 0.3
                    }
                    HeaderText {
                        id: headerType
                        text: qsTr("type")
                        Layout.preferredWidth: myModel.width * 0.25
                    }
                    HeaderText {
                        id: headerModel
                        text: qsTr("model")
                        Layout.preferredWidth: myModel.width * 0.25
                    }
                    HeaderText {
                        id: headerDescription
                        text: qsTr("description")
                        Layout.preferredWidth: myModel.width * 0.2
                    }
                    HeaderText {
                        id: headerDate
                        text: qsTr("date")
                        Layout.preferredWidth: myModel.width * 0.2
                    }
                }
            }

            delegate: Item {
                id: assetInfo

                required property var modelData

                width: myModel.width
                height: 25
                RowLayout {
                    anchors.fill: parent
                    anchors.leftMargin: 5
                    anchors.rightMargin: 5

                    Rectangle {
                        id: emojiItem
                        Layout.alignment: Qt.AlignVCenter
                        implicitWidth: 36
                        implicitHeight: 21
                        radius: 6
                        color: 'green'
                    }

                    Text {
                        Layout.preferredWidth: assetInfo.width * 0.3 - emojiItem.width
                        horizontalAlignment: Qt.AlignLeft
                        leftPadding: 5
                        text: assetInfo.modelData.id
                    }

                    Text {
                        Layout.preferredWidth: assetInfo.width * 0.25
                        horizontalAlignment: Qt.AlignHCenter
                        text: assetInfo.modelData.type
                    }

                    Text {
                        Layout.preferredWidth: assetInfo.width * 0.25
                        horizontalAlignment: Qt.AlignHCenter
                        text: assetInfo.modelData.model
                    }
                    Text {
                        Layout.preferredWidth: assetInfo.width * 0.25
                        horizontalAlignment: Qt.AlignHCenter
                        text: assetInfo.modelData.description
                    }
                    Text {
                        Layout.preferredWidth: assetInfo.width * 0.25
                        horizontalAlignment: Qt.AlignHCenter
                        text: assetInfo.modelData.date
                    }

                }
            }
        }
    }
}
