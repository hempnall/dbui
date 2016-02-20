#include "localstoragetablemodel.h"
#include <QDebug>
#include <QSqlRecord>
#include <QSqlQuery>
#include <localstoragetable.h>

LocalStorageTableModel::LocalStorageTableModel(QObject *parent)
    : QSqlTableModel(parent)
{
    parentTable_ = dynamic_cast<LocalStorageTable*>(parent);
}


LocalStorageTableModel::LocalStorageTableModel(
        QObject *parent,
        QSqlDatabase database,
        const QString& tableName,
        bool addNew,
        bool sorted,
        const QString& sortRole
        )
    : QSqlTableModel(parent,database),
      addRowEnabled_(addNew),
      addRowIndex_(-1),
      filterApplied_(false),
      nextIdValue_(-1),
      lastRowAddedIndex_(-1),
      sorted_(sorted)
{
    parentTable_ = dynamic_cast<LocalStorageTable*>(parent);
    setTable(tableName);


    if (sorted_) {
        int idSort = fieldIndex(sortRole);
        if ( -1 != idSort ) {
            setSort(idSort,Qt::SortOrder::AscendingOrder);
        }
    }
    setEditStrategy(QSqlTableModel::OnManualSubmit);
    select();
    generateRoleNames();
}



LocalStorageTableModel::~LocalStorageTableModel()
{

}

QVariant LocalStorageTableModel::data(const QModelIndex &index, int role) const
{


    if(index.row() >= rowCount())      {
        return QString("<invalid item>");
    }

    if(role < Qt::UserRole)      {
        return QSqlQueryModel::data(index, role);
    } else     {
        return QSqlQueryModel::data(this->index(index.row(), role - Qt::UserRole - 1), Qt::DisplayRole);
    }

}

void LocalStorageTableModel::dataNotFound()     {
    if (nullptr != parentTable_) {
        emit parentTable_->dataNotFound();
    }
}

int LocalStorageTableModel::idForIndex(int id,const QString& role)
{
    if (id == -1) {
        return -1;
    }

    int fieldIdx = fieldIndex(role);
    if (fieldIdx == -1) {
        return -1;
    }

    QModelIndex idx = index(id, fieldIdx);
    QVariant indexData = data(idx,Qt::DisplayRole);

    return indexData.toInt();

}

QString LocalStorageTableModel::textForIndex(int row_index,const QString& role)
{

    if (row_index == -1) {
        dataNotFound();
        return "";
    }


    int fieldIdx = fieldIndex(role);
    if (fieldIdx == -1) {
        dataNotFound();
        return "";
    }

    QModelIndex idx = index(row_index, fieldIdx);
    QVariant indexData = data(idx,Qt::DisplayRole);
    if (!indexData.isValid()) {
        dataNotFound();
    }

    return indexData.toString();
}

QSqlTableModel* LocalStorageTableModel::filter(const QString &field_name, const int field_value)
{

    QString filter_text = field_name + "=" + QString::number( field_value );
    setFilter(filter_text);
    //select();

    return this;
}

int LocalStorageTableModel::addRow(
        const QString &idFieldName,
        const QString &valueFieldName,
        const QString &value
        )
{
    return addRowWithFK(idFieldName,valueFieldName,value);
}

int LocalStorageTableModel::addRowWithFK(
        const QString &idFieldName,
        const QString &valueFieldName,
        const QString &value,
        const QString &fkFieldName,
        const int fkValue
        )
{

    int rowPosAfterAdd = QSqlTableModel::rowCount();

    database().transaction();
    QSqlRecord newRecord = record();

    // deduce max field value
    int nextId = calculateNextId(idFieldName);

    newRecord.setValue(idFieldName, nextId);
    newRecord.setValue(valueFieldName,value);
    if (valueFieldName != "" &&  fkValue != -1) {
        newRecord.setValue(fkFieldName,fkValue);
    }

    bool res = insertRecord(-1, newRecord);
    if (res && submitAll()) {

        database().commit();
        setLastRowAddedIndex(rowPosAfterAdd);
        updateAddRowIndex();
        return nextId;

    } else {

        database().rollback();
        return -1;

    }
}

void LocalStorageTableModel::generateRoleNames()
{
    roles_.clear();
    int nbCols = this->columnCount();

    for (int i = 0; i < nbCols; ++i)      {
        roles_[Qt::UserRole + i + 1] = QVariant(this->headerData(i, Qt::Horizontal).toString()).toByteArray();
    }


}

QHash<int, QByteArray> LocalStorageTableModel::roleNames() const
{
    return roles_;
}

int LocalStorageTableModel::rowCount(const QModelIndex &parent) const
{

        return QSqlTableModel::rowCount();

}

bool LocalStorageTableModel::addRowEnabled() const
{
    return addRowEnabled_;
}

void LocalStorageTableModel::setAddRowEnabled(bool addRowEnabled)
{
    if (addRowEnabled_ != addRowEnabled) {
        addRowEnabled_ = addRowEnabled;
    }
}

int LocalStorageTableModel::addRowIndex()
{
    return addRowIndex_;
}

void LocalStorageTableModel::updateAddRowIndex()
{
    addRowIndex_ = QSqlTableModel::rowCount();
    emit addRowIndexChanged();
}

int LocalStorageTableModel::calculateNextId(const QString &idRoleName)
{
    QString queryStr = QString("select max(%1) + 1 from %2").arg(idRoleName,tableName());

    QSqlQuery qry = database().exec(queryStr);
    while (qry.next()) {
        return  qry.value(0).toInt();
    }
    return -1;
}

int LocalStorageTableModel::lastRowAddedIndex() const
{
    return lastRowAddedIndex_;
}

void LocalStorageTableModel::setLastRowAddedIndex(int lastRowAddedIndex)
{

    if (lastRowAddedIndex != lastRowAddedIndex_) {
        lastRowAddedIndex_ = lastRowAddedIndex;
        emit lastRowAddedIndexChanged();
    }
}


