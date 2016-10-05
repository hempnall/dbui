#include "localstoragedatabase.h"
#include <QCryptographicHash>
#include <QDir>
#include <QFile>
#include <QString>
#include <QDebug>
#include <QSettings>
#include <QSqlDatabase>
#include <QQmlEngine>
#include "localstoragedatabaseexception.h"



static QString qmlsqldatabase_databasesPath(
        const QString& offline_storage_path)
{
    return offline_storage_path
           +  QDir::separator()
           + QLatin1String("Databases");
}

static void qmlsqldatabase_initDatabasesPath(
        const QString& offline_storage_path)
{
    QString databasesPath = qmlsqldatabase_databasesPath(offline_storage_path);
    if (!QDir().mkpath(databasesPath)) {
        qWarning() << "LocalStorage: can't create path - " << databasesPath;
    }
}

static QString qmlsqldatabase_databaseFile(
        const QString& connectionName,
        const QString& offline_storage_path)
{
    return qmlsqldatabase_databasesPath(offline_storage_path) + QDir::separator() + connectionName;
}

LocalStorageDatabase::LocalStorageDatabase()
{

}

LocalStorageDatabase::LocalStorageDatabase(QQuickItem *parent)
    : QQuickItem(parent)
{

}

bool LocalStorageDatabase::create(const QString &dbname)
{
    try {
        if (exists(dbname)) {
            return false;
        }
        QString offlineStoragePath = getLocalStoragePath();
        LocalStorageDatabase::openDatabase(offlineStoragePath,dbname);
        return true;

    } catch (LocalStorageDatabaseException& ex) {
        return false;
    }
}

bool LocalStorageDatabase::exists(const QString &dbname)
{

    QString offlineStoragePath = getLocalStoragePath();
    QString basename = getDatabaseBasename(  offlineStoragePath  ,dbname);

    return QFile::exists(basename+QLatin1String(".sqlite")) || QFile::exists(basename+QLatin1String(".ini"));

}

bool LocalStorageDatabase::remove(const QString &dbname)
{

    try {

        if (!exists(dbname)) {
            return false;
        }

        QString offlineStoragePath = getLocalStoragePath();
        QString basename = getDatabaseBasename( offlineStoragePath , dbname);

        QFile::remove(basename+QLatin1String(".sqlite"));
        QFile::remove(basename+QLatin1String(".ini"));

        return true;

    } catch (LocalStorageDatabaseException& ex) {

        return false;

    }
}

bool LocalStorageDatabase::forcedelete(const QString &dbname)
{

    try {
        QString offlineStoragePath = getLocalStoragePath();
        QString basename = getDatabaseBasename(offlineStoragePath,dbname);
        QFile::remove(basename+QLatin1String(".sqlite"));
        QFile::remove(basename+QLatin1String(".ini"));
        return true;
    } catch (LocalStorageDatabaseException& ex) {


        return false;
    }
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

QSqlDatabase LocalStorageDatabase::openDatabase(const QString &name,QQmlEngine* eng)
{

    QSqlDatabase database;
    QString hash_name = getDatabaseId(name);
    if (QSqlDatabase::connectionNames().contains(hash_name)) {
        database = QSqlDatabase::database(hash_name);
        return database;
    }

    if (eng != nullptr) {
        return LocalStorageDatabase::openDatabase(  LocalStorageDatabase::getLocalStoragePath(eng) , name );
    }

    throw LocalStorageDatabaseException("unable to open database");
}

QString LocalStorageDatabase::getDatabaseBasename(const QString &storagePath, const QString &dbname)
{
    QCryptographicHash md5(QCryptographicHash::Md5);
    md5.addData(dbname.toUtf8());
    QString dbid(QLatin1String(md5.result().toHex()));
    QString basename = qmlsqldatabase_databaseFile(dbid, storagePath);
    return basename;
}

QString LocalStorageDatabase::getLocalStoragePath()
{
    QQmlEngine* engine =  qmlEngine(this);
    return LocalStorageDatabase::getLocalStoragePath(engine);
}

QString LocalStorageDatabase::getLocalStoragePath(QQmlEngine* eng)
{
    QQmlEngine* engine =  eng;
    if (!engine) {
        throw LocalStorageDatabaseException("not running within the context of a qmlengine");
    }
    QString offlineStoragePath = engine->offlineStoragePath();
    return offlineStoragePath;
}


QString LocalStorageDatabase::getDatabaseId(const QString &name)
{
    QCryptographicHash md5(QCryptographicHash::Md5);
    md5.addData(name.toUtf8());
    QString dbid(QLatin1String(md5.result().toHex()));
    return dbid;
}
