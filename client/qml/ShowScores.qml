import QtQuick 2.12
import QtQuick.Controls 2.5
import Scores 1.0

Page {
    id: showscoresid
    title: "Điểm cao"
    width: 480
    height: 640
    property var abclist
    Rectangle{
        color: "transparent"
        width: 60
        height: 60
        z : 99

        Image{
          source: applicationDirPath + "/assets/Sprite/btn_back.png"
          anchors.fill: parent
          MouseArea{
            anchors.fill: parent
            onClicked: {
                console.log("On btn back ShowScores")
//                scid.updateData(false)
                btnBackClick()
            }
          }
        }
    }
    BackGround{
      id: bg
      anchors.fill: parent
    }
    Rectangle{
        id: rect
        anchors.centerIn: parent
        width: 410
        height: 510
        border.color: "black"
        border.width: 2
        color: "gray"
        opacity: 0.3
    }
    ListView {
        x: 40
        y: 70
        width: 400
        height: 500

        model:scid

        interactive:false;

        delegate: Item {
            width: ListView.view.width
            height: 30

            ScorePointer{
                id: sp
                tex1: model.stt
                tex2: model.ten
                tex3: model.diem
                tex4: model.time
                colortext : "white"
            }
        }
    }
    function btnBackClick(){
       // backEnd.resetScoreScreen()
        stackView.pop()
    }
    Component.onCompleted: {
        console.log(rect.x)
        console.log(rect.y)
    }
}
