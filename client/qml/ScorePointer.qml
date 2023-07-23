import QtQuick 2.12
import QtQuick.Controls 2.5

Item {
    property var colortext
    property string tex1
    property string tex2
    property string tex3
    property string tex4
    Rectangle{
        anchors.fill: parent
        Row{
            Text {
                width: 40
                id: stt
                text: tex1
                horizontalAlignment: Text.AlignHCenter
                color: colortext
                font.pixelSize: 16
                font.family: "roboto"
            }
            Text {
                width: 160
                id: tennguoichoi
                text: tex2
                horizontalAlignment: Text.AlignHCenter
                color: colortext
                font.pixelSize: 16
                font.family: "roboto"
            }
            Text {
                width: 100
                id: tienthuong
                text: tex3
                horizontalAlignment: Text.AlignHCenter
                color: colortext
                font.pixelSize: 16
                font.family: "roboto"
            }
            Text {
                width: 100
                id: thoigian
                text: tex4
                horizontalAlignment: Text.AlignHCenter
                color: colortext
                font.pixelSize: 16
                font.family: "roboto"
            }
        }
    }
}
