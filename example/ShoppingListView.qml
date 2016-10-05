import QtQuick 2.0
import QtQuick.Controls 1.4

TableView {


   id: tv
   height: 100

   property alias model : tv.model

   TableViewColumn {
       role: "Quantity"
       title: "Quantity"
       width: 100
   }

   TableViewColumn {
       role: "Item"
       title: "Item"
       width: 200
   }


}




