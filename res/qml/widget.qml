import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

import KarunitPlugins 1.0

ColumnLayout {
    anchors.fill: parent

    RowLayout {
        Layout.fillWidth: true
        Layout.fillHeight: false

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
        Layout.fillWidth: true
        Layout.fillHeight: false

        TextField {
            id: input
            Layout.fillWidth: true
        }

        Button {
            text: qsTr("Save")
            onClicked: {
                KUPLedDisplayPluginConnector.save(input.text)
            }
        }

        Button {
            text: qsTr("Send")
            onClicked: {
                KUPLedDisplayPluginConnector.send(input.text)
                input.clear()
            }
        }
    }

    ListView {
        Layout.fillWidth: true
        Layout.fillHeight: true

        id: listview
        model: KUPLedDisplayPluginConnector.savedTexts
        spacing: 5

        delegate: SwipeDelegate {
            id: swipeDelegate
            width: listview.width

            spacing: 5

            text: modelData

            onClicked: {
                KUPLedDisplayPluginConnector.send(modelData)
            }

            ListView.onRemove: SequentialAnimation {
                PropertyAction {
                    target: swipeDelegate
                    property: "ListView.delayRemove"
                    value: true
                }
                NumberAnimation {
                    target: swipeDelegate
                    property: "height"
                    to: 0
                    easing.type: Easing.InOutQuad
                }
                PropertyAction {
                    target: swipeDelegate
                    property: "ListView.delayRemove"
                    value: false
                }
            }

            swipe.right: Label {
                id: deleteLabel
                text: qsTr("Delete")
                color: "white"
                verticalAlignment: Label.AlignVCenter
                padding: 12
                height: parent.height
                anchors.right: parent.right

                SwipeDelegate.onClicked: {
                    KUPLedDisplayPluginConnector.remove(index)
                }

                background: Rectangle {
                    color: deleteLabel.SwipeDelegate.pressed ? Qt.darker(
                                                                   "tomato",
                                                                   1.1) : "tomato"
                }
            }
        }
    }
}
