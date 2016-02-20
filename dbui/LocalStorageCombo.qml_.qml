import QtQuick 2.0
import QtQuick.Controls 1.4
import com.hempnall.dbui 1.0

Item {    

    id: item

    property string idRole
    property alias textRole : main.textRole
    property LocalStorageTable table
    property string filter

    property int current_state : state_combo
    property int state_combo : 1
    property int state_textedit : 2
    property int state_validate : 3
    property int state_identity : 4
    property int transition_addnew : 1
    property int transition_lostfocus : 2
    property int transition_escape : 3
    property int transition_accept : 4
    property int transition_failsvalidation : 5
    property int transition_passvalidation : 6
    property int transition_comboselected : 7
    property int transition_reject : 8


    function hideEditBox() {
        edit.text = "";
        main.focus  = true;
        main.visible = true;
        main.forceActiveFocus();
        edit.visible = false;
    }

    function accept_edit(text,fkFieldName,fkFieldValue) {


        var nextId;
        if (fkFieldName == "") {
            nextId = main.model.addRow(item.idRole, item.textRole, text);
        } else {
            nextId = main.model.addRowWithFK(item.idRole, item.textRole, text, fkFieldName,fkFieldValue);
        }

        hideEditBox();
        current_state = state_identity;
        console.log(main.model.lastRowAddedIndex);
        console.log("idRole=" + idRole + " fkFieldValue=" + fkFieldValue);
        if (nextId != -1 ) {
            filterComboChanged(idRole,nextId);
        }
        main.currentIndex = main.model.lastRowAddedIndex;
        return state_combo;
    }


    var id = table.model.idForIndex(currentIndex,idRole);
    var text  = table.model.textForIndex(currentIndex,main.textRole);
    FocusScope {

        anchors.fill: parent


        ComboBox {

            id: main
            model: table.model
            anchors.fill: parent
            visible: true
            focus: true
            activeFocusOnPress: true


            onCurrentIndexChanged: {
                current_state = transition(transition_comboselected,currentIndex);
            }

            onPressedChanged: {
                if (pressed) {
                    current_state = transition(transition_comboselected,currentIndex);
                }
            }

        }

        TextField {
            id: edit
            visible: false
            width: main.width

            anchors.verticalCenter: parent.verticalCenter

            placeholderText: "Enter new value and press return"

            property string fkFieldName
            property int fkFieldValue

            onActiveFocusChanged: {
                if (!activeFocus) {
                    current_state = transition(transition_lostfocus,0)
                }
            }

            Keys.onEscapePressed: {
                current_state = transition(transition_escape,0)
            }


            onAccepted: {
                current_state = transition(transition_accept , text,fkFieldName,fkFieldValue);
            }

            Component.onCompleted: {
                fkFieldName = "";
            }
        }

    }


    function filterItems(str,id) {
        console.log(">>>>str=" + str + " id=" + id);
        edit.fkFieldName = str;
        edit.fkFieldValue = id;
//        var abc = table.model.filter(str,id);
//        console.log("qqq" + abc.rowCount());
        main.model.filter(str,id);// = abc;
    }

    signal selectionChanged(int row, int id,string name)
    signal filterComboChanged(string id_field_name,int id_field_value);
    signal beginAddNew()
    signal endAddNew()

}

