#include "localstoragedatabase.h"
#include "localstoragetable.h"
#include "localstoragetablemodel.h"
#include <QDebug>

LocalStorageTable::LocalStorageTable()
    : addNew_(false)
{

}

LocalStorageTable::LocalStorageTable(  QQuickItem* parent)
      : QQuickItem(parent),sortRole_(),addNew_(false),sorted_(false)
{

}

LocalStorageTable::~LocalStorageTable() {

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

LocalStorageTableModel *LocalStorageTable::model() const
{
    QSqlDatabase database = LocalStorageDatabase::openDatabase(database_ , qmlEngine(this));

    LocalStorageTableModel * model =
            new LocalStorageTableModel(
                (QObject*) this,
                database,
                tableName_,
                addNew_,
                sorted_,
                sortRole_);

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

bool LocalStorageTable::sorted() const
{
    return sorted_;
}

void LocalStorageTable::setSorted(bool sorted)
{
    if (sorted_ != sorted) {
        sorted_ = sorted;
        emit sortedChanged();
    }
}

QString LocalStorageTable::sortRole() const
{
    return sortRole_;
}

void LocalStorageTable::setSortRole(const QString &sortRole)
{
    setSorted(true);
    if (sortRole_ != sortRole) {
        sortRole_ = sortRole;
        emit sortRoleChanged();
    }
}





