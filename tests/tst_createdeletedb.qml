import QtQuick 2.3
import QtTest 1.0
import com.hempnall.dbui 1.0


TestCase {
    name: "Create and Destroy DB"

    LocalStorageDatabase {
        id: test
    }

    function test_create_and_destroy() {

        verify( test.forcedelete("test_database", "could not force delete db"));
        verify( ! test.exists("test_database")  , "database already exists");
        verify(   test.create("test_database") , "unable to create database" );
        verify(   test.exists("test_database")  , "database should exist and it doesn't");
        verify(   test.remove("test_database") , "unable to destroy database" );
        verify( !  test.exists("test_database")  , "database should have been destroyed - and it hasn't been");

    }

}
