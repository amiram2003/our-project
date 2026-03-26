import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

Rectangle {
    id: root
    
    property var target: null 
    property bool isUpperCase: false
    property bool isArabic: false
    
    width: parent.width
    height: parent.height * 0.5    
    color: "#333333"               
    visible: false                 
    z: 100                         

    GridLayout {
        anchors.fill: parent
        anchors.margins: 5
        columns: 10                
        rowSpacing: 5
        columnSpacing: 5

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
                
                // --- CRITICAL FIX: Prevent button from stealing focus ---
                focusPolicy: Qt.NoFocus 
                
                contentItem: Text {
                    text: root.isArabic ? modelData.ar : (root.isUpperCase ? modelData.en.toUpperCase() : modelData.en.toLowerCase())
                    color: "black" 
                    font.pixelSize: 18
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                }

                background: Rectangle {
                    color: parent.pressed ? "#00eaff" : "#E0E0E0" 
                    radius: 4
                }
        onClicked: {
    if (root.target) {
        // بنقرأ الحرف بناءً على اللغة وحالة الـ Shift زي ما انتِ معرفاهم فوق
        let charToInsert = root.isArabic ? modelData.ar : (root.isUpperCase ? modelData.en.toUpperCase() : modelData.en.toLowerCase())
        
        root.target.insert(root.target.cursorPosition, charToInsert)
        root.target.forceActiveFocus()
    }
}
            }
        }

        // Shift Button
        Button {
            Layout.fillWidth: true; Layout.fillHeight: true; Layout.columnSpan: 2
            focusPolicy: Qt.NoFocus // FIX
            contentItem: Text {
                text: "⇧"; font.pixelSize: 24
                color: root.isUpperCase ? "#00eaff" : "black"
                horizontalAlignment: Text.AlignHCenter; verticalAlignment: Text.AlignVCenter
            }
            background: Rectangle { color: parent.pressed ? "#00eaff" : "#E0E0E0"; radius: 4 }
            onClicked: {
                root.isUpperCase = !root.isUpperCase
                if (target) target.forceActiveFocus()
            }
        }

        // Language Button
        Button {
            Layout.fillWidth: true; Layout.fillHeight: true; Layout.columnSpan: 2
            focusPolicy: Qt.NoFocus // FIX
            contentItem: Text {
                text: "🌐"; font.pixelSize: 20; color: "black"
                horizontalAlignment: Text.AlignHCenter; verticalAlignment: Text.AlignVCenter
            }
            background: Rectangle { color: parent.pressed ? "#00eaff" : "#E0E0E0"; radius: 4 }
            onClicked: {
                root.isArabic = !root.isArabic
                if (target) target.forceActiveFocus()
            }
        }

        // Backspace Button
        Button {
            Layout.fillWidth: true; Layout.fillHeight: true; Layout.columnSpan: 2
            focusPolicy: Qt.NoFocus // FIX
            contentItem: Text {
                text: "⌫"; font.pixelSize: 20; color: "black"
                horizontalAlignment: Text.AlignHCenter; verticalAlignment: Text.AlignVCenter
            }
            background: Rectangle { color: parent.pressed ? "#ff4444" : "#E0E0E0"; radius: 4 }
           onClicked: {
            if (target && target.cursorPosition > 0) {
             target.remove(target.cursorPosition - 1, target.cursorPosition)
             target.forceActiveFocus()
            }
       }
        }

        // Done Button
        Button {
            text: "DONE"
            Layout.fillWidth: true; Layout.fillHeight: true; Layout.columnSpan: 4
            focusPolicy: Qt.NoFocus // FIX
            font.bold: true
            contentItem: Text {
                text: parent.text; font: parent.font; color: "white"
                horizontalAlignment: Text.AlignHCenter; verticalAlignment: Text.AlignVCenter
            }
            background: Rectangle { color: parent.pressed ? "#0088cc" : "#00eaff"; radius: 4 }
            onClicked: root.visible = false
        }
    }
}