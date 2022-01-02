import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

import KarunitPlugins 1.0

Column {
    RowLayout {
        width: parent.width
        ComboBox {
            Layout.fillHeight: true
            Layout.fillWidth: true
            model: KUPLedDisplayPluginConnector.availablePorts
            onCurrentIndexChanged: {
                KUPLedDisplayPluginConnector.selectPort(currentIndex)
            }
        }

        Button {
            text: qsTr("Refresh")
            onClicked: {
                KUPLedDisplayPluginConnector.refresh()
            }
        }
    }
    RowLayout {
        width: parent.width
        TextField {
            id: input
            Layout.fillWidth: true
        }

        Button {
            text: qsTr("Send")
            onClicked: {
                KUPLedDisplayPluginConnector.send(input.text)
                input.clear()
            }
        }
    }
}
