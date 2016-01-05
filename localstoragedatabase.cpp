#include "localstoragedatabase.h"
#include <QCryptographicHash>
#include <QDir>
#include <QString>
#include <QDebug>
#include <QSettings>
#include <QSqlDatabase>
#include "localstoragedatabaseexception.h"

static QString qmlsqldatabase_databasesPath(const QString& offline_storage_path)
{
    return offline_storage_path +
           QDir::separator() + QLatin1String("Databases");
}

static void qmlsqldatabase_initDatabasesPath(const QString& offline_storage_path)
{
    QString databasesPath = qmlsqldatabase_databasesPath(offline_storage_path);
    if (!QDir().mkpath(databasesPath)) {
        qWarning() << "LocalStorage: can't create path - " << databasesPath;
    }
}

static QString qmlsqldatabase_databaseFile(const QString& connectionName, const QString& offline_storage_path)
{
    return qmlsqldatabase_databasesPath(offline_storage_path) + QDir::separator() + connectionName;
}

QSqlDatabase LocalStorageDatabase::openDatabase(
        const QString& offline_storage_path,
        const QString &database_name,
        const QString& dbversion
        )
{
    qmlsqldatabase_initDatabasesPath(offline_storage_path);

    QSqlDatabase database;

    QCryptographicHash md5(QCryptographicHash::Md5);
    md5.addData(database_name.toUtf8());
    QString dbid(QLatin1String(md5.result().toHex()));

    QString basename = qmlsqldatabase_databaseFile(dbid, offline_storage_path);
    bool created = false;
    QString version = dbversion;

    {
        QSettings ini(basename+QLatin1String(".ini"),QSettings::IniFormat);

        if (QSqlDatabase::connectionNames().contains(dbid)) {
            database = QSqlDatabase::database(dbid);
            version = ini.value(QLatin1String("Version")).toString();
            if (version != dbversion && !dbversion.isEmpty() && !version.isEmpty()) {
                throw LocalStorageDatabaseException("unable to create databse");
            }
        } else {
            created = !QFile::exists(basename+QLatin1String(".sqlite"));
            database = QSqlDatabase::addDatabase(QLatin1String("QSQLITE"), dbid);
            if (created) {
                ini.setValue(QLatin1String("Name"), database_name);

                ini.setValue(QLatin1String("Version"), version);


                ini.setValue(QLatin1String("Driver"), QLatin1String("QSQLITE"));
            } else {
                if (!dbversion.isEmpty() && ini.value(QLatin1String("Version")) != dbversion) {
                    // Incompatible
                    throw LocalStorageDatabaseException("SQL: database version mismatch");
                }
                version = ini.value(QLatin1String("Version")).toString();
            }
            database.setDatabaseName(basename+QLatin1String(".sqlite"));
        }
        if (!database.isOpen()) {
            database.open();
        }
    }

    return database;

}

QSqlDatabase LocalStorageDatabase::openDatabase(QSqlDatabase db)
{
    return db;

}

QSqlDatabase LocalStorageDatabase::openDatabase(const QString &name)
{
    QSqlDatabase database;
    QString hash_name = getDatabaseId(name);
    if (QSqlDatabase::connectionNames().contains(hash_name)) {
        database = QSqlDatabase::database(hash_name);
        return database;
    } else {
        throw LocalStorageDatabaseException("unable to open database - although it should really be open");
    }
}

QString LocalStorageDatabase::getDatabaseId(const QString &name)
{
    QCryptographicHash md5(QCryptographicHash::Md5);
    md5.addData(name.toUtf8());
    QString dbid(QLatin1String(md5.result().toHex()));
    return dbid;
}
