import QtQuick
import QtQuick.Shapes

Window { 
    id: root 
    width: 800 
    height: 480 
    visible: true 
    color: "#01080B"

    // Core data properties linked to C++ backend
    property real speed: vData.speed 
    property real maxSpeed: 160
    
    // Connectivity status updated via D-Bus from Cloud Service
    property bool wifiConnected: dbusData.wifiIP !== "Disconnected"
    property bool bluetoothConnected: false
    property bool isLocked: true 

    // --- TOP STATUS BAR AREA ---
    Item {
        width: 300; height: 50
        anchors { top: parent.top; horizontalCenter: parent.horizontalCenter; topMargin: 5 }
        
        // Background shape for the status bar
        Shape {
            anchors.fill: parent
            ShapePath {
                fillColor: "#0A1F29"; strokeColor: "#00eaff"; strokeWidth: 1
                startX: 0; startY: 0
                PathLine { x: 300; y: 0 }
                PathLine { x: 260; y: 40 }
                PathLine { x: 40; y: 40 }
                PathLine { x: 0; y: 0 }
            }
        }
        
        // Status icons (WiFi, Bluetooth, Lock)
        Row {
            anchors.centerIn: parent
            anchors.verticalCenterOffset: -5
            spacing: 35
            Image { source: wifiConnected ? "qrc:/icons/wifi_on.png" : "qrc:/icons/wifi_off.png"; width: 20; height: 20; fillMode: Image.PreserveAspectFit }
            Image { source: bluetoothConnected ? "qrc:/icons/bluetooth_on.png" : "qrc:/icons/bluetooth_off.png"; width: 18; height: 18; fillMode: Image.PreserveAspectFit }
            Image { source: "qrc:/icons/lock.png"; width: 18; height: 18; fillMode: Image.PreserveAspectFit; opacity: isLocked ? 1.0 : 0.3 }
        }
    }

    // --- CENTRAL SPEEDOMETER GAUGE ---
    Item {
        id: gaugeContainer
        width: 350; height: 350; anchors.centerIn: parent

        // Decorative ticks for the speed scale
        Repeater {
            model: 40 
            Rectangle {
                id: tick
                width: 8
                height: 25
                radius: 2
                color: index * (root.maxSpeed / 40) <= root.speed ? "#00eaff" : "#1A3A4A"
                anchors.centerIn: parent
                antialiasing: true
                opacity: index * (root.maxSpeed / 40) <= root.speed ? 1.0 : 0.3

                transform: [
                    Rotation { 
                        angle: (index * 6.75) - 135
                        origin.x: 4
                        origin.y: 150 
                    },
                    Translate { y: -140 }
                ]
            }
        }

        // Speed value and unit display
        Column {
            anchors.centerIn: parent; spacing: -10
            Text { 
                text: root.speed.toFixed(0)
                color: "white"; font.pixelSize: 110; font.bold: true; font.family: "Monospace"
            }
            Text { 
                text: "km/h"
                color: "#00eaff"; font.pixelSize: 20; font.letterSpacing: 3; anchors.horizontalCenter: parent.horizontalCenter 
            }
        }
        
        // Reboot indicator area (Recovered)
        Column {
            anchors { bottom: parent.bottom; bottomMargin: 10; horizontalCenter: parent.horizontalCenter }
            spacing: 5
            Image { source: "qrc:/icons/reboot.png"; width: 30; height: 30; fillMode: Image.PreserveAspectFit }
            Text { text: "REBOOT"; color: "#FF4444"; font.pixelSize: 10; font.bold: true }
        }
    }

    // --- LEFT INFORMATION PANEL (Temp & Network) ---
    Rectangle {
        width: 130; height: 220
        anchors { left: parent.left; verticalCenter: parent.verticalCenter; leftMargin: 50 }
        color: "#0A1F29"; radius: 12; border.color: "#1A3A4A"
        Column {
            anchors.centerIn: parent; spacing: 35
            Column { spacing: 5; anchors.horizontalCenter: parent.horizontalCenter; Text { text: "🌡️ 24°C"; color: "white"; font.pixelSize: 18 } Text { text: "TEMP"; color: "#888"; font.pixelSize: 9; font.bold: true } }
            Column { spacing: 5; anchors.horizontalCenter: parent.horizontalCenter; Text { text: "📶 LTE"; color: "#00eaff"; font.pixelSize: 16 } Text { text: "NETWORK"; color: "#888"; font.pixelSize: 9; font.bold: true } }
        }
    }

    // --- RIGHT INFORMATION PANEL (Settings & Battery) ---
    Rectangle {
        width: 130; height: 220
        anchors { right: parent.right; verticalCenter: parent.verticalCenter; rightMargin: 50 }
        color: "#0A1F29"; radius: 12; border.color: "#1A3A4A"
        Column {
            anchors.centerIn: parent; spacing: 35
            Column { spacing: 5; anchors.horizontalCenter: parent.horizontalCenter; Image { source: "qrc:/icons/setting.png"; width: 30; height: 30; fillMode: Image.PreserveAspectFit } Text { text: "SETTING"; color: "#00eaff"; font.pixelSize: 9; font.bold: true } }
            Column { spacing: 5; anchors.horizontalCenter: parent.horizontalCenter; Text { text: "🔋 92%"; color: "#00FF00"; font.pixelSize: 18; font.bold: true } Text { text: "BATTERY"; color: "#888"; font.pixelSize: 9; font.bold: true } }
        }
    }

    // Smooth animation for speedometer changes
    Behavior on speed { NumberAnimation { duration: 800; easing.type: Easing.OutCubic } }
}