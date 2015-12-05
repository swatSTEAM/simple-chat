import QtQuick 2.5
import QtQuick.Controls 1.4
import QtQuick.Layouts 1.2
import QtQuick.Controls.Styles 1.4

Item {
    id: loginGroup
    anchors.horizontalCenter: parent.horizontalCenter
    width: parent.width*0.75
//    height: 154
    anchors.horizontalCenterOffset: 0

    Item {
        x: 0
        y: 0
        width: parent.width
        height: _buttonSize
        TextField {
            width: parent.width*0.75-_interval
            height: parent.height
            id: ipInput
            objectName: "ipInput"
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            font.pixelSize: _buttonSize*(3/4)-3
            maximumLength: 20
            selectByMouse: true
            placeholderText: "IP"
            validator:RegExpValidator
            {
                regExp:/^(([01]?[0-9]?[0-9]|2([0-4][0-9]|5[0-5]))\.){3}([01]?[0-9]?[0-9]|2([0-4][0-9]|5[0-5]))$/
            }

//            inputMask:  "000.000.000.000"
            style: TextFieldStyle {
                textColor: _backColor
                selectionColor: _backColor
                placeholderTextColor: Qt.lighter(_backColor)
                background: Rectangle {
                    color: "#00ffffff"
                    border.color: _backColor
                }
            }
        }
        TextField {
            x: ipInput.width+_interval
            width: parent.width-x
            height: parent.height
            id: portInput

            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            font.pixelSize: _buttonSize*(3/4)-3
            maximumLength: 4
            selectByMouse: true
            placeholderText: "Port"
            validator: IntValidator {bottom: 1; top: 65535;}


            style: TextFieldStyle {
                textColor: _backColor
                selectionColor: _backColor
                placeholderTextColor: Qt.lighter(_backColor)
                background: Rectangle {
                    color: "#00ffffff"
                    border.color: _backColor
                }
            }
        }
    }

    TextField {
        id: nickInput
        x: 0
        y: ipInput.y+_buttonSize+_interval
        width: parent.width
        height: _buttonSize
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
        font.pixelSize: _buttonSize*(3/4)-3
        maximumLength: 20
        selectByMouse: true
        placeholderText: "Nickname"
        style: TextFieldStyle {
            textColor: _backColor
            selectionColor: _backColor
            placeholderTextColor: Qt.lighter(_backColor)
            background: Rectangle {
                color: "#00ffffff"
                border.color: _backColor
            }
        }
    }


    Timer {
        id: timer
    }

    function delay(delayTime, cb) {
        timer.interval = delayTime;
        timer.repeat = false;
        timer.triggered.connect(cb);
        timer.start();
    }

    SweetButt {
        function connect() {

            loginSignal(nickInput.text, ipInput.text, portInput.text)

            loginGroup.visible = false
            headText.text = "Loading..."
            animateOpacity.start();
            me = nickInput.text;
        }

        enabled: ipInput.acceptableInput && portInput.acceptableInput && /\S/.test(nickInput.text)
        id: go
        anchors.horizontalCenter: parent.horizontalCenter
        y: nickInput.y+_buttonSize+_interval
        width: parent.width
        height: _buttonSize
        text: "GO"
        onClicked: connect()
//        action: goAction
//        Action {
//            id: goAction
//            shortcut: "Enter"
//            enabled: go.enabled && go.visible
//            onTriggered: {

//            }
//        }
    }

}
