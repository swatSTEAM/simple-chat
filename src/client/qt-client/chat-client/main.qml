import QtQuick 2.5
import QtQuick.Controls 1.4
import QtQuick.Layouts 1.2
import QtQuick.Controls.Styles 1.4
import "controls" as Awesome
import QtQuick.Window 2.2
import QtGraphicalEffects 1.0

ApplicationWindow {

    property var _themes: ["#e62020","#1E90FF", "#FF5800", "#4F7942", "#116062"]
    property int _index: 0
    property color _backColor: _themes[_index]
    property color _textColor: "#f6f4f2"
    property int _buttonSize: 30//mainWindow.height/24
    property int _interval: 10//mainWindow.height/95
    property int userIndex: 0
    property string mateName: "ERROR"
    property string me: "ERROR"
    property var usersData: [[]]
    property variant clickPos: "1,1"

    signal loginSignal(string nickname, string ip, string port)
    signal disconnectSignal()
    signal sendMessSignal(string user, string mess)

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
            warns.text = "Connection failed";
            animateOpacity.stop();
            disconnectUI();
        }

        onDisconnected: {
            warns.text = "Disconnected";
            animateOpacity.stop();
            disconnectUI();
        }

        onConnected: {
            warns.text = "Connected successful";
            animateOpacity.stop();
        }

        onOnlineUsers: {
            console.log(messList);
            var data = JSON.parse(messList);
            usersData = [];
            userModel.clear();
            for(var i = 0; i < data['online_users'].length; i++) {
                userModel.append({name: data['online_users'][i]});
                usersData.push(createModel());
            }
            headText.text = "Users";
            users.visible = true;
        }

        onUserConnected: {
            userModel.append({name: mess});
            usersData.push(createModel());
        }
        onUserDisconnected: {
            for(var i = 0; i < userModel.count; i++) {
                if (userModel.get(i).name == mess) {
                    userModel.remove(i);
                    usersData.splice(i,i+1);
                }
            }
        }
        onNewMess: {
            console.log("New mess from " + user + " :" + mess);
            for(var i = 0; i < userModel.count; i++) {
                if (userModel.get(i).name == user) {
                    usersData[i].append({userFrom: user, date: Qt.formatDateTime(new Date(), "ddd hh:mm:ss"), content: mess});
                    break;
                }
            }
        }

    }

    function disconnectUI() {
        users.visible = false;
        chatList.visible = false;
        headText.text = "Chat";
        head.visible = true;
        loginGroup.visible = true;
    }

    Component {
        id: dummy
        ListModel {
        }
    }

    function createModel() {
        var newModel = dummy.createObject();
        return newModel;
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

                            for(var i = 0; i < userModel.count; i++) {
                                if (userModel.get(i).name == mateName) {
                                    userIndex = i;
                                    chatView.model = usersData[userIndex];
                                    console.log("SELECT MATE: " + mateName);
                                    break;
                                }
                            }
                        }
                    }
                }

            focus: true
            model: ListModel {
                id: userModel
            }
        }

        ListModel {
            id: chatModel
        }

        Item {
            visible: false
            id: chatList
            anchors.fill: parent

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
                            x: userFrom == me ? chatView.width - width: 0
                            id: wrap
                            width: parent.width
                            height: parent.height
                            color: userFrom == me ? "#00000000" : _backColor
                            border.color:  _backColor
                            radius: Math.min(width*0.5, (parent.width-messText.width)/2)


                            MouseArea {
                                anchors.fill: parent
                                onClicked: chatView.currentIndex = index
                            }
                        }
                        Text {
                            id: messText
                            color: userFrom == me ?_backColor : _textColor
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
                        usersData[userIndex].append({userFrom: me, date: Qt.formatDateTime(new Date(), "ddd hh:mm:ss"), content: inputMess.text});
                        sendMessSignal(mateName, inputMess.text);
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


