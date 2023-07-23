import QtQuick 2.12
import QtQuick.Controls 2.5
import Scores 1.0
Page {
    id: scoresid
    title: "Điểm cao"
    width: 480
    height: 640
    property bool run1: true
    property bool run2: false
    property bool showRank: false
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
                console.log("On btn back")

                stackView.pop()
            }
          }
        }
    }

    BackGround{
      id: bg
      anchors.fill: parent
    }

    SelectButton{
      id: btnDiemcanhan
      text: "Điểm cá nhân"
      anchors.horizontalCenter: parent.horizontalCenter
      onClick: {
          backEnd.requestResultList(true)
//          scid.updateData(false)
          stackView.push("ShowScores.qml")
      }
    }
    NumberAnimation {
      id : runbtndcn
      duration: 250
      target : btnDiemcanhan
      from : 640
      to: 250
      properties : "y"
      running: run1
      onStopped :{
        run2 = true
      }
    }

    SelectButton{
      id: btnDiemthuhang
      text: "Điểm thứ hạng"
      anchors.horizontalCenter: parent.horizontalCenter
      onClick: {
          backEnd.requestResultList(false)
          scid.updateData(true)
          stackView.push("ShowScores.qml")
      }
    }
    NumberAnimation {
      id : runbtndth
      duration: 250
      target : btnDiemthuhang
      from : 640
      to: 300
      properties : "y"
      running: run2
      onStopped :{
      }
    }
}
