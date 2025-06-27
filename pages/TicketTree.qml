import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import ZTicketAsset
import "../global.js" as Global

Item {

    Rectangle {
        //       width: 600
        //       height: 600
        anchors.fill: parent
        ListModel {
            id: objModel

            // Function to populate the model from a JSON array
            function populateModel(ticketArray, stepArray) {
                if('length' in ticketArray) {
                    for (var i = 0; i < ticketArray.length; i++) {
                        append({
                                   "emoji": ticketArray[i].emoji,
                                   "description": ticketArray[i].description,
                                   "ticket": ticketArray[i].ticket,
                                   "nickname": ticketArray[i].user,
                                   "level": 1,
                                   "subNode": []
                               });
                    }
                    if('length' in stepArray) {
                        for (var j = 0; j < stepArray.length; j++) {
                            for (var t = 0; t < objModel.count; t++) {
                                var node = objModel.get(t);
                                if(node.ticket === stepArray[j].ticket) {
                                    node.subNode.append({
                                               "emoji": stepArray[j].emoji,
                                               "description": stepArray[j].description,
                                               "ticket": stepArray[j].ticket,
                                               "nickname": stepArray[j].nickname,
                                               "level": 2,
                                               "subNode": []
                                           });
                                    break;
                                }
                            }
                        }
                    }
                }
            }

            Component.onCompleted: {
                handleUpdate();
                populateModel(Global.tickets, Global.steps);
            }
        }
        Component {
            id: objRecursiveDelegate
            Column {
                id: objRecursiveColumn
                clip: true
                MouseArea {
                    width: objRow.implicitWidth
                    height: objRow.implicitHeight
                    Dialog {
                        id: stepDialog
                        property string ticket : ""
                        property string nickname: ""

                        x: (parent.width - width) / 2
                        y: (parent.height - height) / 2
                        parent: Overlay.overlay

                        focus: true
                        modal: true
                        title: qsTr("New Step: ") + nickname + "->" + ticket
                        standardButtons: Dialog.Ok | Dialog.Cancel

                        ColumnLayout {
                            spacing: 20
                            anchors.fill: parent

                            Image {
                                id: logo
                                width: 128
                                source: "../images/cam.svg"
                            }
                            RowLayout {

                                Label {
                                    wrapMode: Label.Wrap
                                    horizontalAlignment: Qt.AlignHLeft
                                    text: ticket
                                }
                                TextField {
                                    id: newstepemoji
                                    focus: true
                                    placeholderText: "😁"
                                    Layout.preferredWidth: stepDialog.width * 0.1
                                }
                                TextField {
                                    id: newstepdescription
                                    focus: true
                                    placeholderText: qsTr("Decription")
                                    Layout.preferredWidth: stepDialog.width * 0.5
                                }
                            }
                        }
                        onAccepted:
                        {
                            if(newstepemoji.text !== "" && newstepdescription.text !== ""){
                                backend.addTicketStep([ticket, nickname, newstepemoji.text, newstepdescription.text]);
                           }
                        }
                    }
                    onDoubleClicked: {
                        for(var i = 1; i < parent.children.length; ++i) {
                            parent.children[i].visible = !parent.children[i].visible
                        }
                    }
                    onClicked: {
                        stepDialog.ticket = model.ticket;
                        stepDialog.nickname = model.nickname;
                        stepDialog.open()
                    }

                    Row {
                        id: objRow
                        Item {
                            height: 1
                            width: model.level * 20
                        }
                        Text {
                            text: (objRecursiveColumn.children.length
                                   > 2 ? objRecursiveColumn.children[1].visible ? "🔽  " : "🔼 " : "🔵  ") + model.emoji + " " + model.description
                            color: objRecursiveColumn.children.length > 2 ? "blue" : "green"
                            font {
                                bold: true
                                pixelSize: 14
                            }
                        }
                    }
                }
                Repeater {
                    model: subNode
                    delegate: objRecursiveDelegate
                }
            }
        }

        ColumnLayout {
            anchors.fill: parent
            Window {
                id: objModalInput
                modality: Qt.ApplicationModal
                visible: false
                height: 30
                width: 200
                color: "yellow"
                TextInput {
                    anchors.fill: parent
                    font {
                        bold: true
                        pixelSize: 20
                    }
                    verticalAlignment: TextInput.AlignVCenter
                    horizontalAlignment: TextInput.AlignHCenter
                    validator: RegularExpressionValidator {
                        regularExpression: /(\d{1,},)*.{1,}/
                    }
                    onFocusChanged: {
                        if (focus) {
                            selectAll()
                        }
                    }
                    text: qsTr("node0")
                    onAccepted: {
                        if (acceptableInput) {
                            objModalInput.close()
                            var szSplit = text.split(',')
                            var node = objModel
                            var found = true
                            for (var i = 0; i < szSplit.length - 1; ++i) {
                                found = false
                                for (var index = 0; index < node.count; index++) {
                                    if (node.get(index).description === szSplit[i]) {
                                        found = true
                                        node = node.get(index).subNode
                                        break
                                    }
                                }
                                if (!found) {
                                    console.log("Error - Given node does not exist !")
                                    break
                                }
                            }
                            if (found) {
                                node.append({
                                                "description": szSplit[i],
                                                "level": i,
                                                "subNode": []
                                            })
                            }
                        }
                    }
                }
            }
            Button {
                text: "add data to tree"
                onClicked: {
                    objModalInput.show()
                }
            }
            ListView {
                Layout.fillHeight: true
                Layout.fillWidth: true
                model: objModel
                delegate: objRecursiveDelegate
            }
        }
    }
}
