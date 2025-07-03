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

                        onOpened: newstepemoji.model.populateCombo(Global.emojis);


                        ColumnLayout {
                            spacing: 20
                            anchors.fill: parent

                            Image {
                                id: logo
                                width: 128
                                source: "../images/cam.svg"
                                z: -1
                            }
                            RowLayout {

                                Label {
                                    wrapMode: Label.Wrap
                                    horizontalAlignment: Qt.AlignHLeft
                                    text: ticket
                                }
                                ComboBox {
                                    id: newstepemoji
                                    focus: true

                                    model: ListModel {
                                        id: stepcomboModel
                                        // Function to populate the model from a JSON array
                                        function populateCombo(jsonArray) {
                                            var v;
                                            clear();
                                            for (var i = 0; i < jsonArray.length; i++) {
                                                v = jsonArray[i]['html'].replace("Ux", " &#x") + ";";

                                                append({text: v});
                                            }
                                        }
                                    }
                                    contentItem: Text {
                                                    text: newstepemoji.currentText // Use the text that ComboBox determines based on textRole
                                                    font: newstepemoji.font
                                                    horizontalAlignment: Text.AlignLeft
                                                    verticalAlignment: Text.AlignVCenter
                                                    elide: Text.ElideRight

                                                    // THE KEY: Set textFormat to RichText
                                                    textFormat: Text.RichText
                                                }
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
                                   > 2 ? objRecursiveColumn.children[1].visible ? "🔽  " : "🔼 " : "🔵  ") + model.emoji + " " + model.ticket + ":" + model.description
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
            ListView {
                Layout.fillHeight: true
                Layout.fillWidth: true
                model: objModel
                delegate: objRecursiveDelegate
            }
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
                            source: "../images/cam.svg"
                            z: -1
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
}
