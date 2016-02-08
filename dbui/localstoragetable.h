#pragma once



#include <QString>
#include <QStringList>
#include <QSqlDatabase>
#include <QQuickItem>
#include <QQmlListProperty>
#include <QSqlTableModel>


class LocalStorageTable
    : public QQuickItem
{
    Q_OBJECT

    Q_PROPERTY(QString tableName READ tableName WRITE setTableName NOTIFY tableNameChanged)
    Q_PROPERTY(QString database READ database WRITE setDatabase NOTIFY databaseChanged)
    Q_PROPERTY(QSqlTableModel* model READ model )
    Q_PROPERTY(QStringList headers READ headers WRITE setHeaders)
    Q_PROPERTY(bool addNew READ addNew WRITE setAddNew NOTIFY addNewChanged)
    Q_PROPERTY(QString addNewText READ addNewText WRITE setAddNewText NOTIFY addNewTextChanged)

public:
    LocalStorageTable();
    LocalStorageTable(QQuickItem *parent );
    ~LocalStorageTable();

    QString tableName() const;
    void setTableName(const QString &tableName);

    QString database() const;
    void setDatabase(const QString &database);

    QSqlTableModel *model() const;
    QStringList headers();
    void setHeaders(QStringList list);


    bool addNew() const;
    void setAddNew(bool addNew);

    QString addNewText() const;
    void setAddNewText(const QString &addNewText);

signals:
    void tableNameChanged();
    void databaseChanged();
    void addNewChanged();
    void dataNotFound();
    void addNewTextChanged();

private:

    QString tableName_;
    QString database_;
    QStringList headers_;
    bool addNew_;
    QString addNewText_;




public slots:
};


