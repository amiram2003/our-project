import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

Rectangle {
    id: root
    
    // Property to link the keyboard with the target TextField (SSID or Password)
    property var target: null 
    property bool isUpperCase: false
    property bool isArabic: false
    
    width: parent.width
    height: parent.height * 0.5    // Keyboard takes 50% of screen height
    color: "#333333"               // Dark background
    visible: false                 // Hidden until a TextField is clicked
    z: 100                         // Keeps the keyboard on top of other elements

    GridLayout {
        anchors.fill: parent
        anchors.margins: 5
        columns: 10                // 10 buttons per row
        rowSpacing: 5
        columnSpacing: 5

        // Repeater creates buttons for all numbers and letters in the model
        Repeater {
            model: [
                {en: "1", ar: "١"}, {en: "2", ar: "٢"}, {en: "3", ar: "٣"}, {en: "4", ar: "٤"}, {en: "5", ar: "٥"},
                {en: "6", ar: "٦"}, {en: "7", ar: "٧"}, {en: "8", ar: "٨"}, {en: "9", ar: "٩"}, {en: "0", ar: "٠"},
                {en: "q", ar: "ض"}, {en: "w", ar: "ص"}, {en: "e", ar: "ث"}, {en: "r", ar: "ق"}, {en: "t", ar: "ف"},
                {en: "y", ar: "غ"}, {en: "u", ar: "ع"}, {en: "i", ar: "ه"}, {en: "o", ar: "خ"}, {en: "p", ar: "ح"},
                {en: "a", ar: "ش"}, {en: "s", ar: "س"}, {en: "d", ar: "ي"}, {en: "f", ar: "ب"}, {en: "g", ar: "ل"},
                {en: "h", ar: "ا"}, {en: "j", ar: "ت"}, {en: "k", ar: "ن"}, {en: "l", ar: "م"}, {en: "@", ar: "ك"},
                {en: "z", ar: "ئ"}, {en: "x", ar: "ء"}, {en: "c", ar: "ؤ"}, {en: "v", ar: "ر"}, {en: "b", ar: "لا"},
                {en: "n", ar: "ى"}, {en: "m", ar: "ة"}, {en: ".", ar: "و"}, {en: "_", ar: "ز"}, {en: "-", ar: "ظ"}
            ]
            
            Button {
                Layout.fillWidth: true
                Layout.fillHeight: true
                
                contentItem: Text {
                    text: root.isArabic ? modelData.ar : (root.isUpperCase ? modelData.en.toUpperCase() : modelData.en.toLowerCase())
                    color: "black" // Changed to black for better visibility
                    font.pixelSize: 18
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                }

                background: Rectangle {
                    color: parent.pressed ? "#00eaff" : "#E0E0E0" // Light gray background
                    radius: 4
                }
                
                onClicked: if (target) target.text += text
            }
        }

        // Shift Button with Arrow Icon
        Button {
            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.columnSpan: 2
            contentItem: Text {
                text: "⇧"
                font.pixelSize: 24
                color: root.isUpperCase ? "#00eaff" : "black"
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
            }
            background: Rectangle {
                color: parent.pressed ? "#00eaff" : "#E0E0E0"
                radius: 4
            }
            onClicked: root.isUpperCase = !root.isUpperCase
        }

        // Language Button with Globe Icon
        Button {
            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.columnSpan: 2
            contentItem: Text {
                text: "🌐"
                font.pixelSize: 20
                color: "black"
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
            }
            background: Rectangle {
                color: parent.pressed ? "#00eaff" : "#E0E0E0"
                radius: 4
            }
            onClicked: root.isArabic = !root.isArabic
        }

        // Backspace Button
        Button {
            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.columnSpan: 2
            contentItem: Text {
                text: "⌫"
                font.pixelSize: 20
                color: "black"
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
            }
            background: Rectangle {
                color: parent.pressed ? "#ff4444" : "#E0E0E0"
                radius: 4
            }
            onClicked: {
                if (target && target.text.length > 0)
                    target.text = target.text.substring(0, target.text.length - 1)
            }
        }

        // Done Button
        Button {
            text: "DONE"
            font.bold: true
            highlighted: true
            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.columnSpan: 4
            
            contentItem: Text {
                text: parent.text
                font: parent.font
                color: "white"
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
            }
            
            background: Rectangle {
                color: parent.pressed ? "#0088cc" : "#00eaff"
                radius: 4
            }
            onClicked: root.visible = false
        }
    }
}