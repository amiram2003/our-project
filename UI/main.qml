import QtQuick
import QtQuick.Shapes
import QtQuick.Controls

Window { 
    id: root 
    width: 800 
    height: 480 
    visible: true 
    color: "#01080B"

    // --- Core data properties linked to C++ backend ---
    property real speed: vData.speed 
    property real maxSpeed: 160
    
    // --- Connectivity status updated via D-Bus from Cloud Service ---
    property bool wifiConnected: dbusData.wifiIP !== ""
    property bool bluetoothConnected: false
    property bool isLocked: true 

    // --- TOP STATUS BAR AREA ---
    Item {
        width: 380; height: 50 
        anchors { top: parent.top; horizontalCenter: parent.horizontalCenter; topMargin: 5 }
        
        Shape {
            anchors.fill: parent
            ShapePath {
                fillColor: "#0A1F29"; strokeColor: "#00eaff"; strokeWidth: 1
                startX: 0; startY: 0
                PathLine { x: 380; y: 0 } 
                PathLine { x: 340; y: 40 }
                PathLine { x: 40; y: 40 }
                PathLine { x: 0; y: 0 }
            }
        }
        
        Row {
            anchors.centerIn: parent
            anchors.verticalCenterOffset: -5
            spacing: 30 
            
            // WiFi Status Icon
            Image { source: wifiConnected ? "qrc:/icons/wifi_on.png" : "qrc:/icons/wifi_off.png"; width: 20; height: 20; fillMode: Image.PreserveAspectFit }
            
            // WiFi Settings Trigger Button
            Button {
                id: wifiTrigger
                width: 25; height: 25
                background: Rectangle { color: "transparent" }
                contentItem: Image {
                    source: "qrc:/icons/button.png"
                    fillMode: Image.PreserveAspectFit
                }
                onClicked: wifiPopup.open() 
            }

            // Bluetooth Status Icon
            Image { source: bluetoothConnected ? "qrc:/icons/bluetooth_on.png" : "qrc:/icons/bluetooth_off.png"; width: 18; height: 18; fillMode: Image.PreserveAspectFit }
            
            // Lock Status Icon
            Image { source: "qrc:/icons/lock.png"; width: 18; height: 18; fillMode: Image.PreserveAspectFit; opacity: isLocked ? 1.0 : 0.3 }
        }
    }

    // --- CENTRAL SPEEDOMETER GAUGE ---
    Item {
        id: gaugeContainer
        width: 350; height: 350; anchors.centerIn: parent

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

        // Digital Speed Display
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
        
        // Reboot Button Section
        Column {
            anchors { bottom: parent.bottom; bottomMargin: 10; horizontalCenter: parent.horizontalCenter }
            spacing: 5
            
            // الزيادة هنا: ضفنا MouseArea عشان يحس بالضغط
            MouseArea {
                width: 40; height: 50 // مساحة مناسبة للضغط
                anchors.horizontalCenter: parent.horizontalCenter
                
                Column {
                    anchors.fill: parent
                    spacing: 5
                    Image { 
                        source: "qrc:/icons/reboot.png"
                        width: 30; height: 30; 
                        fillMode: Image.PreserveAspectFit; 
                        anchors.horizontalCenter: parent.horizontalCenter 
                    }
                    Text { 
                        text: "REBOOT"
                        color: "#FF4444"; 
                        font.pixelSize: 10; 
                        font.bold: true; 
                        anchors.horizontalCenter: parent.horizontalCenter 
                    }
                }
                
                // اللحظة الحاسمة: بننادي الـ C++ اللي بيكلم جهاد
                onClicked: {
                    console.log("Rebooting System via Cloud...")
                    dbusData.requestReboot() 
                }
            }
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

    // --- WIFI CREDENTIALS POPUP ---
    Popup {
        id: wifiPopup
        width: 420; height: 440 // Optimized height for new keyboard rows
        modal: true
        focus: true
        x: (parent.width - width) / 2
        y: (parent.height - height) / 2
        
        background: Rectangle {
            color: "#0A1F29"
            radius: 10
            border.color: "#00eaff"
        }

        // --- Container for Input Fields ---
        Column {
            id: popupContent
            width: parent.width - 20
            anchors.top: parent.top
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.topMargin: 20
            spacing: 12

            TextField {
                id: wifiUser
                width: parent.width
                placeholderText: "SSID (User)"
                color: "white"
                background: Rectangle { color: "#1A3A4A"; radius: 4 }
                onPressed: {
                    myKeyboard.target = wifiUser
                    myKeyboard.visible = true
                }
            }

            TextField {
                id: wifiPass
                width: parent.width
                placeholderText: "Password"
                echoMode: TextInput.Password
                color: "white"
                background: Rectangle { color: "#1A3A4A"; radius: 4 }
                onPressed: {
                    myKeyboard.target = wifiPass
                    myKeyboard.visible = true
                }
            }

            Button {
                id: connectBtn
                text: "CONNECT"
                width: parent.width
                height: 40
                contentItem: Text { text: parent.text; color: "white"; horizontalAlignment: Text.AlignHCenter; verticalAlignment: Text.AlignVCenter; font.bold: true }
                background: Rectangle { color: "#00eaff"; radius: 4 }
                onClicked: {
                    if (wifiUser.text !== "" && wifiPass.text !== "") {
                        dbusData.connectToWifi(wifiUser.text, wifiPass.text)
                    }
                }
            }
        }

        // --- THE VIRTUAL KEYBOARD (Anchored to Bottom) ---
        CustomKeyboard {
            id: myKeyboard
            width: parent.width - 20
            height: 200
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 15
            anchors.horizontalCenter: parent.horizontalCenter
            visible: false 
        }
    }
    
    // Smooth animation for speedometer changes
    Behavior on speed { NumberAnimation { duration: 800; easing.type: Easing.OutCubic } }
}