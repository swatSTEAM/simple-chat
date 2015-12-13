import QtQuick 2.5
import QtQuick.Controls 1.4
import QtQuick.Layouts 1.2
import QtQuick.Controls.Styles 1.4
import QtGraphicalEffects 1.0

ListView {
    id: chatView
    y: spacing+close.height
    x: parent.x+10
    width: mainWindow.width-2*10
    height: inputMess.y-close.height-spacing*2
    spacing: _buttonSize
    delegate:
        Item {
            width: parent.width*0.75
            height: messText.height + _buttonSize
            Rectangle {
                x: user == me ? chatView.width - width: 0
                id: wrap
                width: parent.width
                height: parent.height
                color: user == me ? "#00000000" : _backColor
                border.color:  _backColor
                radius: Math.min(width*0.5, (parent.width-messText.width)/2)


                MouseArea {
                    anchors.fill: parent
                    onClicked: chatView.currentIndex = index
                }
            }
            Text {
                id: messText
                color: user == me ?_backColor : _textColor
                text: content
                font.pixelSize: _interval*2
                width: wrap.width-9*_interval
                anchors.horizontalCenterOffset: 0
                anchors.verticalCenterOffset: 0
                anchors.centerIn: wrap
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignHCenter
                wrapMode: Text.Wrap
            }
            Text {
                id: dateText
                color: _backColor
                text: userFrom + " " + date
                font.pixelSize: _interval*1.3
                width: parent.width
//                        anchors.centerIn: parent
                horizontalAlignment: Text.AlignHCenter
                anchors.horizontalCenter: wrap.horizontalCenter
                y: wrap.y+wrap.height
//                        wrapMode: Text.WordWrap
            }
         }

    focus: true
    model: chatModel
}

