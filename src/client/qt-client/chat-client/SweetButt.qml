import QtQuick 2.0
import QtQuick.Controls 1.4
import QtQuick.Controls.Styles 1.4

Button {
    id: butt
    height: _buttonSize
    style: ButtonStyle {
        background: Rectangle {
            color: control.pressed ? Qt.darker(_backColor) : control.enabled ? _backColor : Qt.lighter(_backColor)
            radius: 0
        }
        label: Text {
//            renderType: Text.NativeRendering
            color: _textColor
            text: butt.text
            font.pixelSize: _buttonSize*(3/4)-3
            anchors.centerIn: parent
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignHCenter
        }
    }
}
