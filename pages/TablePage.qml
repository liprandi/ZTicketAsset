import QtQuick
import QtQuick.Controls
import Qt.labs.qmlmodels
import ZTicketAsset

Item {
    width: 1126
    height: 800

    Rectangle {
        id: rectangle
        color: "#ffffff"
        anchors.fill: parent
        anchors.rightMargin: 0
        anchors.bottomMargin: 0
        anchors.leftMargin: 0
        anchors.topMargin: 0

        TableView {
            anchors.fill: parent
            columnSpacing: 1
            rowSpacing: 1
            clip: true

            model: ZTable {
            }

            delegate: Rectangle {
                implicitHeight: 50
                implicitWidth: 100
                border.width: 1

                Text {
                    text: display
                    anchors.centerIn: parent
                }
            }
        }
    }
}
