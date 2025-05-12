// Copyright (C) 2017 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR BSD-3-Clause

pragma ComponentBehavior: Bound

import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import ZTicketAsset 1.0

ScrollablePage {

    Column {
        spacing: 40
        width: parent.width


        Label {
            width: parent.width
            wrapMode: Label.Wrap
            horizontalAlignment: Qt.AlignHCenter
            text: qsTr("User Login")
        }

        DropArea {
            id: dragTarget
            anchors.horizontalCenter: parent.horizontalCenter
            width: 150; height: 150

            Image {
                anchors.fill: parent
                source: "qrc:/images/user.png"
            }
            onEntered: {
                if(checkFileExtension(drag.text))
                {
                    drag.accept()
                }
            }

            function checkFileExtension(filePath) {
                return filePath.split('.').pop() === "csv"
            }

            onDropped: {
                if(checkFileExtension(drop.text))
                {
                    backend.importFromCsv(drop.text);
                }
            }
        }

        Component.onCompleted: {
            loginDialog.open();
        }

        Dialog {

            id: loginDialog
            width: 400
            title: qsTr("Login")
            standardButtons: Dialog.Ok | Dialog.Cancel

            ColumnLayout {
                spacing: 20
                width: loginDialog.width * 0.8;
                anchors.horizontalCenter: parent.horizontalCenter

                Image {
                    Layout.alignment: Qt.AlignCenter
                    Layout.preferredWidth: 64
                    Layout.preferredHeight: 64
                    source: "qrc:/images/user.png"
                }
                TextField {
                    id: username
                    Layout.alignment: Qt.AlignCenter
                    Layout.fillWidth: true
                    Layout.preferredWidth: 360
                    placeholderText: qsTr("nickname")
                }

                TextField {
                    id: password
                    width: parent.width
                    Layout.alignment: Qt.AlignCenter
                    Layout.fillWidth: true
                    Layout.preferredWidth: 360
                    placeholderText: qsTr("password")
                    echoMode: TextInput.Password
                }

                Button {
                    id: addButton
                    text: qsTr("Add User")
                    width: parent.width
                    Layout.alignment: Qt.AlignLeft
                    Layout.fillWidth: true
                    Layout.preferredWidth: 360

                    onClicked: inputDialog.open()

                    Dialog {
                        id: inputDialog

                        x: (parent.width - width) / 2
                        y: (parent.height - height) / 2
                        parent: Overlay.overlay

                        focus: true
                        modal: true
                        title: qsTr("New User")
                        standardButtons: Dialog.Ok | Dialog.Cancel

                        ColumnLayout {
                            spacing: 20
                            anchors.fill: parent
                            Label {
                                elide: Label.ElideRight
                                text: qsTr("Please enter the credentials:")
                                Layout.fillWidth: true
                            }

                            Image {
                                id: logo
                                width: parent.width / 2
                                fillMode: Image.PreserveAspectFit
                                source: "../images/cam-logo.png"
                            }

                            TextField {
                                id: newname
                                focus: true
                                placeholderText: qsTr("Name")
                                Layout.fillWidth: true
                            }
                            TextField {
                                id: newsurname
                                focus: true
                                placeholderText: qsTr("Surname")
                                Layout.fillWidth: true
                            }
                            TextField {
                                id: newemail
                                focus: true
                                placeholderText: qsTr("email")
                                Layout.fillWidth: true
                            }
                        }
                        onAccepted:
                        {
                            if(newname.text !== "" && newsurname.text !== "" && newemail.text !== "")
                                backend.addUser([newname.text, newsurname.text, newemail.text]);
                        }
                    }
                }
            }
            onAccepted:
            {
                if(username.text !== "" && password.text !== "")
                    backend.login([username.text, password.text]);
            }
        }
    }
}

