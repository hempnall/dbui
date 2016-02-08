#pragma once


#include <QSqlDatabase>
#include <QQuickItem>

class LocalStorageDatabase
        : public QQuickItem
{
    Q_OBJECT
public:
  LocalStorageDatabase();
  LocalStorageDatabase(QQuickItem *parent );

  Q_INVOKABLE bool create(const QString& );
  Q_INVOKABLE bool exists(const QString& );
  Q_INVOKABLE bool remove(const QString& );
  Q_INVOKABLE bool forcedelete(const QString& );

  static QSqlDatabase openDatabase( const QString& offline_storage_path, const QString& database_name, const QString& version = "1.0");
  static QSqlDatabase openDatabase( QSqlDatabase database);
  static QSqlDatabase openDatabase( const QString& name );

private:
  QString getDatabaseBasename(const QString& storagePath, const QString& dbName);
  QString getLocalStoragePath();
  static QString getDatabaseId(const QString& name);


};

