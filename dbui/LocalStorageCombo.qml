import QtQuick 2.0
import QtQuick.Controls 1.4
import QtQuick.Layouts 1.1
import QtQml.StateMachine 1.0 as DSM
import com.hempnall.dbui 1.0
import QtQuick.Dialogs 1.1


RowLayout {

    id: fd
//    width: cmb.width + btn.width
//    height: cmb.height + btn.height
//    x: btn.x
//    y: btn.y

    spacing: 0
    anchors.margins: 0

    property bool addNewEnabled : false
    property LocalStorageTable table
    property string idRole
    property alias textRole : cmb.textRole
    property string fkFieldName
    property int fkFieldValue
    property int valueIndex: 0

    signal selectedItemChanged(string id_field_name,int id_field_value);

    onTableChanged: {
        addNewEnabled = table.addNew;
    }

    function refreshCombo() {
        cmb.model.filter(fkFieldName,fkFieldValue);
        cmb.refresh();
    }


    function filterItems(str,id) {
        if (str == fkFieldName && id == fkFieldValue) {
            return;
        }
        fkFieldName = str;
        fkFieldValue = id;
        valueIndex = 0;
        refreshCombo();
    }


    DSM.StateMachine {
        id: stateMachine
        initialState: state
        running: true

        DSM.State {
            id: state

            DSM.SignalTransition {
                targetState: edit_combo
                signal: btn.clicked
            }

            onEntered: {
                cmb.refresh();
            }


        }
        DSM.State {
            id: edit_combo
            DSM.SignalTransition {
                targetState: state
                signal: cmb.quitEditing;
                onTriggered: {
                    cmb.refresh();
                }
            }

            DSM.SignalTransition {
                targetState: validation
                signal: cmb.accepted;
            }

            onEntered: {
                cmb.editable = true;
                cmb.enabled = true;
                cmb.currentIndex = -1;
                btn.enabled = false
            }
        }

        DSM.State {
            id: validation
            DSM.SignalTransition {
                targetState: success_validation
                signal: validation.valid;
            }

            DSM.SignalTransition {
                targetState: validation_error
                signal: validation.error;
            }

            DSM.SignalTransition {
                signal: messageDialog.no;
                targetState: validation_error;
            }

            onEntered: {
                messageDialog.text = "Add [" +  fkFieldName + ","  + fkFieldValue  + "] " + cmb.editText + " to database?";
                messageDialog.open();
            }

            signal valid();
            signal error();

            Component.onCompleted: {
                messageDialog.no.connect(error);
                messageDialog.yes.connect(valid);
            }

        }


        DSM.State {
            id: success_validation
            DSM.SignalTransition {
                targetState: state
                signal: success_validation.successful_validation_complete;
            }
            onEntered: {
                var enteredValue = cmb.editText
                var nextId;

                if (fkFieldName == "") {
                    nextId = cmb.model.addRow(idRole, textRole, enteredValue);
                    cmb.model = table.model;
                } else {
                    nextId = cmb.model.addRowWithFK(idRole, textRole, enteredValue, fkFieldName,fkFieldValue);
                }

                valueIndex = cmb.find(enteredValue);
                successful_validation_complete();
            }
            signal successful_validation_complete;
        }

        DSM.State {
            id: validation_error
            DSM.SignalTransition {
                targetState: state
                signal: validation_error.error_processing_complete;
            }
            onEntered: {
                console.log("entered error validation");
                error_processing_complete();
            }
            signal error_processing_complete();
        }

        DSM.FinalState {
            id: finalState
        }
        onFinished: Qt.quit()
    }

    Button {
        id: btn
        text: "+"

        Layout.preferredWidth: 50
        Layout.fillWidth: true

        visible: addNewEnabled
        enabled: addNewEnabled
    }

    ComboBox {

        id: cmb
        Layout.preferredWidth:100


        model: table.model

        onCurrentIndexChanged: {
            var idx = model.idForIndex(currentIndex,idRole);
            selectedItemChanged(idRole,idx);
        }

        Keys.onEscapePressed: {
            quitEditing();
        }

        onActiveFocusChanged: {
            if (!activeFocus) {
                quitEditing();
            }
        }

        function refresh() {

            cmb.editable = false;
            btn.enabled = addNewEnabled;

            if (cmb.model.rowCount() == 0) {
                cmb.enabled  = false;
                cmb.currentIndex = -1;
            } else {
                cmb.enabled = true;
                cmb.currentIndex = valueIndex;
            }

        }

        signal quitEditing();

    }


    MessageDialog {
        id: messageDialog
        icon: StandardIcon.Question
        title: "Update Database"

        standardButtons: StandardButton.Yes | StandardButton.No
    }


}

