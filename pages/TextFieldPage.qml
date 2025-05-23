// Copyright (C) 2017 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR BSD-3-Clause

import QtQuick
import QtQuick.Controls

ScrollablePage {
    id: page

    Column {
        spacing: 40
        width: parent.width

        Label {
            width: parent.width
            wrapMode: Label.Wrap
            horizontalAlignment: Qt.AlignHCenter
            text: qsTr("TextField is a single-line text editor.")
        }

        TextField {
            id: username
            placeholderText: qsTr("e-mail")
            anchors.horizontalCenter: parent.horizontalCenter
        }
        TextField {
            id: password
            placeholderText: qsTr("password")
            anchors.horizontalCenter: parent.horizontalCenter
        }
    }
}
