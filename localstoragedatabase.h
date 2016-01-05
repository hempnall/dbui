#pragma once


#include <QSqlDatabase>


class LocalStorageDatabase
{
public:


  static QSqlDatabase openDatabase( const QString& offline_storage_path, const QString& database_name, const QString& version = "1.0");
  static QSqlDatabase openDatabase( QSqlDatabase database);
  static QSqlDatabase openDatabase( const QString& name );

private:
  static QString getDatabaseId(const QString& name);


};

