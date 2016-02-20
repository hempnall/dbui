import QtQuick 2.3
import QtTest 1.0
import QtQuick.LocalStorage 2.0
import com.hempnall.dbui 1.0
import "create_data.js" as DATA

TestCase {

    name: "Create and Destroy Data"


   // function modelCreate

    LocalStorageDatabase {
        id: test
    }

    property var addNewText : "<add new>"

    LocalStorageTable {
        id: sections_addnew
        tableName: "Sections"
        database: "QQmlExampleDB"
        addNew: true
    }

    LocalStorageTable {
        id: items_addnew
        tableName: "Items"
        database: "QQmlExampleDB"
    }

    function initTestCase() {
        DATA.create_data();
    }



    SignalSpy {
        id: spy_add_new
        target: sections_addnew  //.model
        signalName: "dataNotFound"
    }



    function test_a_addnew() {

        compare(  sections_addnew.model.textForIndex(0,"section_name") ,"dairy" );
        compare( sections_addnew.model.textForIndex(2,"section_name"),"meat"  );
        compare(  spy_add_new.count,0);
        compare(sections_addnew.model.textForIndex(-1,"section_name") ,  ""  );
        compare(spy_add_new.count,1);
    }

    function test_b_addnew_record() {

        compare( sections_addnew.addNew , true );

        sections_addnew.model.addRow("section_id","section_name","beauty");
        compare( sections_addnew.model.rowCount(), 4);



    }


    function test_c_filtering() {

        var filtered  = items_addnew.model.filter("section_id",1);
        compare( filtered.rowCount() , 3 );
        filtered.addRowWithFK("item_id","item_name","cream","section_id",1);
        compare( filtered.rowCount() , 4 );

        filtered = items_addnew.model.filter("section_id",1);
        compare( filtered.rowCount() , 4 );

        filtered = items_addnew.model.filter("section_id",2);
        compare( filtered.rowCount() , 3 );

        filtered = items_addnew.model.filter("section_id",4);
        compare( filtered.rowCount() , 0 );
        filtered.addRowWithFK("item_id","item_name","lipstick","section_id",4);
        compare( filtered.rowCount() , 1 );
        filtered.addRowWithFK("item_id","item_name","wash","section_id",4);
        compare( filtered.rowCount() , 2 );

        filtered  = items_addnew.model.filter("section_id",1);
        compare( filtered.rowCount() , 4 );
        filtered.addRowWithFK("item_id","item_name","dairylea","section_id",1);
        compare( filtered.rowCount() , 5 );
    }


    function cleanupTestCase() {

        DATA.clear_tables();
    }



}
