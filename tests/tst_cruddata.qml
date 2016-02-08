import QtQuick 2.3
import QtTest 1.0
import QtQuick.LocalStorage 2.0
import com.hempnall.dbui 1.0
import "create_data.js" as DATA

TestCase {
    name: "Create and Destroy Data (addnew)"


   // function modelCreate

    LocalStorageDatabase {
        id: test
    }

    LocalStorageTable {
        id: sections
        tableName: "Sections"
        database: "QQmlExampleDB"
    }





    function initTestCase() {
        DATA.create_data();
    }

    SignalSpy {
        id: spy
        target: sections  //.model
        signalName: "dataNotFound"
    }



    function test_normal() {
        compare(  sections.model.textForIndex(0,"section_name") ,"dairy" );
        compare( sections.model.textForIndex(2,"section_name"),"meat"  );
        compare(  spy.count,0);
        compare( sections.model.textForIndex(3,"section_name") , ""  );
        compare(sections.model.textForIndex(-1,"section_name") ,  ""  );
        compare(spy.count,2);
        compare(  sections.addNew , false );
        compare(  sections.addNewText , "" );
        compare( sections.model.rowCount(), 3);
    }




    function cleanupTestCase() {
        DATA.clear_tables();
        //test.forcedelete("QQmlExampleDB");

    }



}
