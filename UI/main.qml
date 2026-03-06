import QtQuick
import QtQuick.Controls
import QtQuick.Shapes

Window {
    width: 800
    height: 480
    visible: true
    color: "black"

    property real speed: 35

    Rectangle {
        anchors.fill: parent
        color: "black"

        Item {
            id: gauge
            width: 280
            height: 280
            anchors.centerIn: parent

            Rectangle {
                anchors.fill: parent
                radius: width/2
                color: "#181818"
                opacity: 0.85
                border.color: "#222"
                border.width: 2
            }

            Repeater {
                model: 60
                Rectangle {
                    width: 2
                    height: index % 5 === 0 ? 18 : 8
                    color: "#00eaff"
                    opacity: index % 5 === 0 ? 0.7 : 0.3
                    radius: 1
                    anchors.centerIn: parent
                    transform: [
                        Rotation { angle: index * 6; origin.x: 1; origin.y: 130 },
                        Translate { y: -130 }
                    ]
                }
            }

            Column {
                anchors.centerIn: parent
                spacing: 0
                Text {
                    text: speed.toFixed(0)
                    color: "white"
                    font.pixelSize: 70
                    font.bold: true
                    font.family: "Arial"
                    anchors.horizontalCenter: parent.horizontalCenter
                }
                Text {
                    text: "km/h"
                    color: "#00eaff"
                    font.pixelSize: 18
                    anchors.horizontalCenter: parent.horizontalCenter
                }
            }
        }

        Column {
            anchors.left: parent.left
            anchors.leftMargin: 50
            anchors.verticalCenter: parent.verticalCenter
            spacing: 18

            Text { text: "🌤 20°C"; color: "white"; font.pixelSize: 20 }
            Text { text: "↗ 300 m"; color: "#aaaaaa"; font.pixelSize: 16 }
            Text { text: "Navigation"; color: "#666"; font.pixelSize: 14 }
        }

        Column {
            anchors.right: parent.right
            anchors.rightMargin: 50
            anchors.verticalCenter: parent.verticalCenter
            spacing: 18

            Text { text: "10:13"; color: "white"; font.pixelSize: 20 }
            Text { text: "🔊 Bluetooth"; color: "#aaaaaa"; font.pixelSize: 16 }
            Text { text: "Settings"; color: "#666"; font.pixelSize: 14 }
        }

        Timer {
            interval: 2000
            running: true
            repeat: true
            onTriggered: speed = Math.random() * 160
        }
    }
}