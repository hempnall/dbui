#include "localstoragedatabase.h"
#include "localstoragetable.h"
#include "localstoragetablemodel.h"


LocalStorageTable::LocalStorageTable()
{

}

LocalStorageTable::LocalStorageTable(  QQuickItem* parent)
      : QQuickItem(parent)
{

}


QString LocalStorageTable::tableName() const
{
    return tableName_;
}

void LocalStorageTable::setTableName(const QString &tableName)
{
    if (tableName != tableName_) {
        tableName_ = tableName;
        update();   // repaint with the new color
        emit tableNameChanged();
    }
}

QString LocalStorageTable::database() const
{
    return database_;
}

void LocalStorageTable::setDatabase(const QString &database)
{
    if (database != database_) {
        database_ = database;
        update();   // repaint with the new color
        emit databaseChanged();
    }
}

QSqlTableModel *LocalStorageTable::model() const
{
    QSqlDatabase database = LocalStorageDatabase::openDatabase(database_);

    QSqlTableModel * model = new LocalStorageTableModel((QObject*) this,database,tableName_,addNew_);

    return model;
}

QStringList LocalStorageTable::headers()
{
    return headers_;
}

void LocalStorageTable::setHeaders(QStringList headers)
{
    if (headers != headers_) {
        headers_ = headers;
       // update();   // repaint with the new color
       // emit headersChanged();
    }
}

bool LocalStorageTable::addNew() const
{
    return addNew_;
}

void LocalStorageTable::setAddNew(bool addNew)
{
    addNew_ = addNew;
}


