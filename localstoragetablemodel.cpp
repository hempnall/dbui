#include "localstoragetablemodel.h"
#include <QDebug>
#include <QSqlRecord>
#include <QSqlQuery>

LocalStorageTableModel::LocalStorageTableModel(QObject *parent) {
}


LocalStorageTableModel::LocalStorageTableModel(
        QObject *parent,
        QSqlDatabase database,
        const QString& tableName,
        bool addNew
        )
    : QSqlTableModel(parent,database),
      addRowEnabled_(addNew),
      addRowIndex_(-1),
      filterApplied_(false),
      nextIdValue_(-1),
      lastRowAddedIndex_(-1)
{
    setTable(tableName);
    setEditStrategy(QSqlTableModel::OnManualSubmit);
    select();
    generateRoleNames();
    if (addNew) {
     updateAddRowIndex();
    }
}



LocalStorageTableModel::~LocalStorageTableModel()
{

}

QVariant LocalStorageTableModel::data(const QModelIndex &index, int role) const
{

    if (addRowEnabled_ && index.row() == QSqlTableModel::rowCount()) {
        return QString("<add new>");
    }

    if(index.row() >= rowCount())      {
        return QString("<invalid item>");
    }

    if(role < Qt::UserRole)      {
        return QSqlQueryModel::data(index, role);
    } else     {
        return QSqlQueryModel::data(this->index(index.row(), role - Qt::UserRole - 1), Qt::DisplayRole);
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
        return "<null>";
    }

    int fieldIdx = fieldIndex(role);
    if (fieldIdx == -1) {
        return "<null>";
    }

    QModelIndex idx = index(row_index, fieldIdx);
    QVariant indexData = data(idx,Qt::DisplayRole);

    return indexData.toString();
}

QSqlTableModel* LocalStorageTableModel::filter(const QString &field_name, const int field_value)
{
    int previous_row_count = rowCount();
    beginRemoveRows(QModelIndex(),0,qMax(0,previous_row_count -1));
    endRemoveRows();

    QString filter_text = field_name + "=" + QString::number( field_value );
    setFilter(filter_text);
    if (addRowEnabled_) {
        updateAddRowIndex();
    }
    return this;
}

void LocalStorageTableModel::addRow(
        const QString &idFieldName,
        const QString &valueFieldName,
        const QString &value
        )
{
    addRowWithFK(idFieldName,valueFieldName,value);
}

void LocalStorageTableModel::addRowWithFK(
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
        qDebug() << newRecord;

    } else {

        qDebug() << "failed";
        database().rollback();

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
    if (addRowEnabled_) {
        return QSqlTableModel::rowCount(parent) + 1;
    } else {
        return QSqlTableModel::rowCount();
    }

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
    qDebug() << "addRowIndex = " << addRowIndex_;
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
    qDebug() << "last row added = " << lastRowAddedIndex;
    if (lastRowAddedIndex != lastRowAddedIndex_) {
        qDebug() << "last row added = (emit emit)";
        lastRowAddedIndex_ = lastRowAddedIndex;
        emit lastRowAddedIndexChanged();
    }
}


