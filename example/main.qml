import QtQuick 2.4
import QtQuick.Controls 1.4
import QtQuick.Window 2.2
import QtQuick.Layouts 1.1

import com.hempnall.dbui 1.0
import DBDB 1.0


ApplicationWindow {

    property int margin: 11
    visible: true

    width: mainLayout.implicitWidth + ( 2 * margin);
    height: mainLayout.implicitHeight + ( 2 * margin);
    minimumHeight: mainLayout.Layout.minimumHeight + (2 * margin)
    minimumWidth: mainLayout.Layout.minimumWidth + (2 * margin)

    ShoppingList {
        id: shopping_list
    }

    Action {
        id: closeApplicationAction
        text: "Close"
        shortcut: StandardKey.Quit
        enabled: txtQuantity.length > 0
        onTriggered: console.log("hello")
    }

    ColumnLayout {

        id: mainLayout
        anchors.margins: margin
        anchors.fill: parent


        GridLayout {

           anchors.fill: parent
           anchors.top: parent.top
           columns: 4
           Layout.fillWidth: true;
           flow: GridLayout.LeftToRight

           Label {
               text: "Quantity"
           }

           TextField {
               id: txtQuantity
               Layout.fillWidth: true
           }


           Label {
               text: "Item "
           }

           TextField {
               text: "Hello World"
               Layout.fillWidth: true
           }
        }




    ABCD {

  Layout.fillWidth: true


        id: fred
//        width: 100
       height: 100

        }





        RowLayout {

            Layout.fillWidth: true


            Button {
                action: closeApplicationAction
                text: "Close"
            }

            Button {
                text: "Save"
            }

        }

    }

}

