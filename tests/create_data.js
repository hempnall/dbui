

function create_data() {
    var db = LocalStorage.openDatabaseSync("QQmlExampleDB", "1.0", "The Example QML SQL!", 1000000);

    db.transaction(
        function(tx) {


            // Create the database if it doesn't already exist
            tx.executeSql('CREATE TABLE IF NOT EXISTS Sections(section_id INT, section_name TEXT)');
            tx.executeSql('CREATE TABLE IF NOT EXISTS Items(item_id INT, section_id INT, item_name TEXT)');

            tx.executeSql('DELETE FROM Sections');
            tx.executeSql('DELETE FROM Items');


            // Add (another) greeting row
            tx.executeSql('INSERT INTO Sections VALUES(?, ?)', [ '1', 'dairy' ]);
            tx.executeSql('INSERT INTO Sections VALUES(?, ?)', [ '2', 'veg' ]);
            tx.executeSql('INSERT INTO Sections VALUES(?, ?)', [ '3', 'meat' ]);

            tx.executeSql('INSERT INTO Items VALUES(?, ?,?)', [ '1','1', 'milk' ]);
             tx.executeSql('INSERT INTO Items VALUES(?, ?,?)', [ '2','1', 'cream' ]);
             tx.executeSql('INSERT INTO Items VALUES(?, ?,?)', [ '3','1', 'cheese' ]);
            tx.executeSql('INSERT INTO Items VALUES(?, ?,?)', [ '4','2', 'carrots' ]);
            tx.executeSql('INSERT INTO Items VALUES(?, ?,?)', [ '5','2', 'cauliflower' ]);
            tx.executeSql('INSERT INTO Items VALUES(?, ?,?)', [ '6','2', 'potatoes' ]);
            tx.executeSql('INSERT INTO Items VALUES(?, ?,?)', [ '7','3', 'steak' ]);
            tx.executeSql('INSERT INTO Items VALUES(?, ?,?)', [ '8','3', 'chicken' ]);

        }
    )

}

function clear_tables() {
    var db = LocalStorage.openDatabaseSync("QQmlExampleDB", "1.0", "The Example QML SQL!", 1000000);

    db.transaction(
        function(tx) {
            // Create the database if it doesn't already exist
            tx.executeSql('DELETE FROM Sections');
            tx.executeSql('DELETE FROM Items');

        }
    )
}
