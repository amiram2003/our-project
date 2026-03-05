import QtQuick
import QtQuick.Controls

Window {
    width: 800
    height: 480
    visible: true
    title: "Car Dashboard"
    color: "#0a0a0a" 

    Rectangle {
        id: speedometer
        width: 300; height: 300
        anchors.centerIn: parent
        radius: 150
        color: "transparent"
        border.color: "#00d1ff" 
        border.width: 8

        Column {
            anchors.centerIn: parent
            Text {
                text: "80"
                font.pixelSize: 80
                color: "white"
                font.bold: true
                anchors.horizontalCenter: parent.horizontalCenter
            }
            Text {
                text: "km/h"
                font.pixelSize: 20
                color: "#00d1ff"
                anchors.horizontalCenter: parent.horizontalCenter
            }
        }
    }

    Row {
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 20
        anchors.horizontalCenter: parent.horizontalCenter
        spacing: 50

        Text { text: "Temp: 22°C"; color: "white"; font.pixelSize: 18 }
        Text { text: "Battery: 85%"; color: "#00ff00"; font.pixelSize: 18 }
        Text { text: "Gear: D"; color: "white"; font.pixelSize: 18; font.bold: true }
    }
}