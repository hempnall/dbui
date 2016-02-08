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

        if (fkFieldName == "") {
            main.model.addRow(item.idRole, item.textRole, text);
        } else {
            main.model.addRowWithFK(item.idRole, item.textRole, text, fkFieldName,fkFieldValue);
        }

        hideEditBox();
        current_state = state_identity;
        console.log(main.model.lastRowAddedIndex);
        main.currentIndex = main.model.lastRowAddedIndex;
        return state_combo;
    }

    function reject_edit() {
        hideEditBox();
        current_state = state_identity;
        main.currentIndex = 0;
        return state_combo;
    }

    function combo_to_addnew(arg) {
        console.trace();
        edit.focus = true;
        edit.visible = true;
        edit.forceActiveFocus();
        main.visible = false;
        return state_textedit;
    }

    function combo_item_selected(currentIndex) {
        var id = table.model.idForIndex(currentIndex,idRole);
        var text  = table.model.textForIndex(currentIndex,main.textRole);
        selectionChanged(currentIndex,id,text);
        if (currentIndex == main.model.addRowIndex) {
            return transition_combo(transition_addnew,0);
        } else {
            return state_combo;
        }
    }

    function transition_combo(tran_id,arg1,arg2,arg3) {

        switch (tran_id) {
        case transition_addnew:
            return combo_to_addnew(arg1);

        case transition_comboselected:
            return combo_item_selected(arg1);

        default:
            throw "invalid transition in combo state" + tran_id;
        }
    }

    function transition_textedit(tran_id,arg1,arg2,arg3) {

        switch (tran_id) {

        case transition_lostfocus:
        case transition_escape:
            return reject_edit();

        case transition_accept:
            return accept_edit(arg1,arg2,arg3);

        default:
            throw "invalid transition in textedit state" + tran_id;
        }
    }

    function transition_validate(tran_id,arg1,arg2,arg3) {
        switch (tran_id) {

        case transition_failsvalidation:
            break;
        case transition_passvalidation:
            break;

        default:
            throw "invalid transition in validate state";
        }
    }

    function transition(tran_id,arg1,arg2,arg3) {

        arg1 = arg1 || "";
        arg2 = arg2 || "";
        arg3 = arg3 || "";


        switch (current_state) {

        case state_combo:
            return transition_combo(tran_id,arg1,arg2,arg3);


        case state_textedit:
            return transition_textedit(tran_id,arg1,arg2,arg3);


        case state_validate:
            return  transition_validate(tran_id,arg1,arg2,arg3);

        case state_identity:
            return current_state;

        default:
            throw "invalid state"

        }
    }



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
        edit.fkFieldName = str;
        edit.fkFieldValue = id;
     //   main.model = table.model.filter(str,id)
    }

    signal selectionChanged(int row, int id,string name)
    signal beginAddNew()
    signal endAddNew()

}

