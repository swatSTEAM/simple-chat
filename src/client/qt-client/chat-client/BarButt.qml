import QtQuick 2.0
import QtQuick.Controls 1.4
import QtQuick.Controls.Styles 1.4

Button {
    id: butt
    style: ButtonStyle {
        background: Rectangle {
            color: control.pressed ? Qt.darker(_backColor) : _backColor
            radius: 0
        }
        label: Text {
            color: _textColor
            text: butt.text
            font.pixelSize: _buttonSize*(3/4)-3
            font.family: awesome.family
            anchors.centerIn: parent
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignHCenter
        }
    }
}
