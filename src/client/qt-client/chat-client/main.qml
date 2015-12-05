import QtQuick 2.5
import QtQuick.Controls 1.4
import QtQuick.Layouts 1.2
import QtQuick.Controls.Styles 1.4
import "controls" as Awesome
import QtQuick.Window 2.2
import QtGraphicalEffects 1.0

ApplicationWindow {

    property var _themes: ["#e62020","#1E90FF", "#FF5800", "#4F7942","#116062"]
    property int _index: 0
    property color _backColor: _themes[_index]
    property color _textColor: "#f6f4f2"
    property int _buttonSize: 30//mainWindow.height/24
    property int _interval: 10//mainWindow.height/95

    property string mateName: "ERROR"
    property string me: "ERROR"
    property variant clickPos: "1,1"

    signal loginSignal(string nickname, string ip, string port)
    signal disconnectSignal()
    signal sendMessSignal(string mess)
    signal selectUser(string mess)

    flags: Qt.FramelessWindowHint
    visible: true
    width: Screen.desktopAvailableWidth/3.5
    height: Screen.desktopAvailableHeight*0.9
    color: _textColor
    id: mainWindow
    title: qsTr("Chat")

    Connections {
        target: core
        onFailed: {
            warns.text = mess;
            animateOpacity.stop();
//            headText.text = "Users";
//            users.visible = true;
            disconnectUI();
        }
        onDisconnected: {
            warns.text = mess;
            animateOpacity.stop();
        }
        onConnected: {
            warns.text = mess;
            animateOpacity.stop();
            headText.text = "Users";
            users.visible = true;
        }
    }

    function disconnectUI() {
        users.visible = false;
        chatList.visible = false;
        headText.text = "Chat";
        head.visible = true;
        loginGroup.visible = true;
    }

    SequentialAnimation {
        id: animateOpacity
         NumberAnimation {
            properties: "opacity"
            from: 1.0
            to: 0.5
            duration: 1000
            target: head
        }
         NumberAnimation {
            properties: "opacity"
            from: 0.5
            to: 1.0
            duration: 1000
            target: head
        }
        alwaysRunToEnd: true
        loops: Animation.Infinite
   }

    Item {
        anchors.fill: parent
        FontAwesome {
            id: awesome
            resource: "/resource/fontawesome-webfont.ttf"
        }

        Rectangle {
            id: head
            y: 7*_interval
            anchors.horizontalCenter: parent.horizontalCenter
            width:  _buttonSize*5
            height: width
            radius: width*0.5
            anchors.horizontalCenterOffset: 0
            color: _backColor
            Text {
                id: headText
                color: _textColor
                text: qsTr("Chat")
                anchors.verticalCenterOffset: 0
                anchors.horizontalCenterOffset: 0
                font.pixelSize: _buttonSize
                anchors.centerIn: parent
            }
        }

        LoginGroup {
            id: loginGroup
            y: head.y+head.height+7*_interval
        }


        ListView {
            id: users
            visible: false
            anchors.horizontalCenter: parent.horizontalCenter
            y: head.y+head.height+7*_interval
            width: mainWindow.width*0.75
            height: mainWindow.height - y - 12*_interval
            delegate:
                Rectangle {
                    id: wrapper
                    width: parent.width
                    anchors.horizontalCenter: parent.horizontalCenter
                    height: _buttonSize
                    color: ListView.isCurrentItem ? _backColor : "#00000000"
                    Text {
                        width: parent.width
                        color: wrapper.ListView.isCurrentItem ? _textColor : _backColor
                        height: _buttonSize
                        text: name
                        font.pixelSize: _buttonSize*(3/4) - 3
                        verticalAlignment: Text.AlignVCenter
                        horizontalAlignment: Text.AlignHCenter
                    }
                    MouseArea {
                        anchors.fill: parent
                        onClicked: users.currentIndex = index
                        onDoubleClicked: {
                            users.visible = false;
                            mateName = name;
                            chatList.visible = true;
                            head.visible = false;
                        }
                    }
                }

            focus: true
            model: ListModel {
                id: userModel
                ListElement {
                    name: "User1"
                }

                ListElement {
                    name: "User2"
                }

                ListElement {
                    name: "User3"
                }
            }
        }

        Item {
            visible: false
            id: chatList
            anchors.fill: parent

            ChatModel {
                id: chatView
            }

            TextArea {
                id: inputMess
                height: 75
                x:-1
                y: mainWindow.height - send.height - height
                width: mainWindow.width + 2
                backgroundVisible: true
                highlightOnFocus: false
                font.pixelSize: _buttonSize*(3/4)-3
                wrapMode: Text.Wrap

                style: TextAreaStyle {
                    backgroundColor: mainWindow.color
                    textColor: _backColor
                    selectionColor: _backColor
                }
            }

            BarButt {
                id: send
                x: 0
                y: mainWindow.height - height
                width: mainWindow.width
                text: awesome.icons.fa_paper_plane_o
                tooltip: "Send"
                onClicked: {
                    if (/\S/.test(inputMess.text) !== "") {
                        chatView.model.append({user: me, date: Qt.formatDateTime(new Date(), "ddd hh:mm:ss"), content: inputMess.text});
                        chatView.currentIndex = chatView.count - 1;
                    }
                }
            }
        }

        Item {
            id: barbuttons
            BarButt {
                id: close
                x:  mainWindow.width - width
                y: 0
                text: awesome.icons.fa_times
                height: _buttonSize
                width: _buttonSize
                tooltip: "Quit"
                onClicked: {
                    Qt.quit()
                }
            }

            BarButt {
                id: sett
                x: close.x - width
                y: 0
                text: awesome.icons.fa_paint_brush
                height: _buttonSize
                width: _buttonSize
                tooltip: "Change theme"
                onClicked: {
                    _index++;
                    if (_index>_themes.length-1) {
                        _index=0
                    }
                    _backColor = _themes[_index];
                }
            }

            BarButt {
                id: usersBar
                x:  sett.x - width
                y: 0
                text: awesome.icons.fa_users
                height: _buttonSize
                width: _buttonSize
                tooltip: "To users"
                onClicked: {
                    if (head.visible == false) {
                        chatList.visible = false;
                        head.visible = true;
                        users.visible = true;
                    }
                }
            }
            BarButt {
                id: disconnect
                x:  usersBar.x - width
                y: 0
                text: awesome.icons.fa_power_off
                height: _buttonSize
                width: _buttonSize
                tooltip: "Disconnect"
                onClicked: {
                    if (!loginGroup.visible) {
                        disconnectSignal();
                        disconnectUI();
                    }
                }
            }
        }

        Rectangle {
            width: disconnect.x
            height: _buttonSize
            color: _backColor
            Text {
                width: parent.width
                height: parent.height
                x: _interval
                id: warns
                text: qsTr("Enter connection data")
                color: _textColor
                verticalAlignment: Text.AlignVCenter
            }
            MouseArea {
                anchors.fill: parent
                onPressed: {
                    clickPos  = Qt.point(mouse.x,mouse.y)
                }

                onPositionChanged: {
                    var delta = Qt.point(mouse.x-clickPos.x, mouse.y-clickPos.y)
                    mainWindow.x += delta.x;
                    mainWindow.y += delta.y;
                }
            }
        }

//        Rectangle {
//            id: errorBox
//            width: mainWindow.width
//            height: 4*_buttonSize
//            y: mainWindow.height - height
//            color: _backColor
//            BarButt {
//                text: awesome.icons.fa_times
//                height: _buttonSize
//                width: _buttonSize
//                x: parent.width - width
//                y: 0
//                onClicked: {
//                    parent.visible = false
//                }
//            }
//            Text {
//                id: errorText
//                x: 4*_interval
//                y: 4*_interval
//                width: parent.width - 8*_interval
//                height: parent.height - 8*_interval
//                text: "error C4430: missing type specifier - int assumed. Note: C++ does not support default-int"
//                color: _textColor
//                wrapMode: Text.Wrap
//                verticalAlignment: Text.AlignVCenter
//                horizontalAlignment: Text.AlignHCenter
//            }
//        }
    }
}


