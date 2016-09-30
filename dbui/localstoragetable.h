#pragma once



#include <QString>
#include <QStringList>
#include <QSqlDatabase>
#include <QQuickItem>
#include <QQmlListProperty>
#include <QSqlTableModel>
#include "localstoragetablemodel.h"

class LocalStorageTable
    : public QQuickItem
{
    Q_OBJECT

    Q_PROPERTY(QString tableName READ tableName WRITE setTableName NOTIFY tableNameChanged)
    Q_PROPERTY(QString database READ database WRITE setDatabase NOTIFY databaseChanged)
    Q_PROPERTY(LocalStorageTableModel* model READ model NOTIFY modelChanged)
    Q_PROPERTY(QStringList headers READ headers WRITE setHeaders)
    Q_PROPERTY(bool addNew READ addNew WRITE setAddNew NOTIFY addNewChanged)
    Q_PROPERTY(bool sorted READ sorted WRITE setSorted NOTIFY sortedChanged)
    Q_PROPERTY(QString sortRole READ sortRole WRITE setSortRole NOTIFY sortRoleChanged)


public:
    LocalStorageTable();
    LocalStorageTable(QQuickItem *parent );
    ~LocalStorageTable();

    QString tableName() const;
    void setTableName(const QString &tableName);

    QString database() const;
    void setDatabase(const QString &database);

    LocalStorageTableModel *model() const;
    QStringList headers();
    void setHeaders(QStringList list);


    bool addNew() const;
    void setAddNew(bool addNew);


    bool sorted() const;
    void setSorted(bool sorted);

    QString sortRole() const;
    void setSortRole(const QString &sortRole);

signals:
    void tableNameChanged();
    void databaseChanged();
    void addNewChanged();
    void dataNotFound();
    void addNewTextChanged();
    void modelChanged();
    void sortedChanged();
    void sortRoleChanged();

private:

    QString tableName_;
    QString database_;
    QString sortRole_;
    QStringList headers_;
    bool addNew_;
    bool sorted_;


public slots:
};


