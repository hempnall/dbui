#pragma once

#include <QHash>
#include <QStringList>
#include <QSqlTableModel>

class LocalStorageTable;

/// see wiki.qt.io/QML_and_QSqlTableModel
class LocalStorageTableModel
    : public QSqlTableModel
{
    Q_OBJECT

public:

    Q_PROPERTY(bool addRowEnabled READ addRowEnabled WRITE setAddRowEnabled)
    Q_PROPERTY(int addRowIndex READ addRowIndex NOTIFY addRowIndexChanged)
    Q_PROPERTY(int lastRowAddedIndex READ lastRowAddedIndex NOTIFY lastRowAddedIndexChanged)


    LocalStorageTableModel(QObject* parent = 0);
    LocalStorageTableModel(QObject* parent, QSqlDatabase db, const QString& str,bool addNew,bool sorted = false,const QString& sortRole="");
    ~LocalStorageTableModel();

    Q_INVOKABLE QVariant data(const QModelIndex &index, int role=Qt::DisplayRole ) const;
    Q_INVOKABLE int  idForIndex( int id , const QString& role );
    Q_INVOKABLE QString textForIndex( int index,const QString& role);
    Q_INVOKABLE LocalStorageTableModel* filter(const QString& field_name, const int field_value);
    Q_INVOKABLE int addRow(const QString& idFieldName, const QString& valueFieldName,  const QString& value);
    Q_INVOKABLE int addRowWithFK(const QString& idFieldName, const QString& valueFieldName,   const QString& value ,const QString& fkFieldName="", const int fkValue=-1);


    void generateRoleNames();

    virtual QHash<int, QByteArray> roleNames() const;
    Q_INVOKABLE virtual int rowCount(const QModelIndex & parent = QModelIndex()) const;

    bool addRowEnabled() const;
    void setAddRowEnabled(bool addRowEnabled);
    int addRowIndex();



    int lastRowAddedIndex() const;
    void setLastRowAddedIndex(int lastRowAddedIndex);

signals:


    void addRowIndexChanged();
    void currentItemChanged(int newCurrentItem);
    void lastRowAddedIndexChanged();


private:

    void updateAddRowIndex();
    int calculateNextId(const QString& idRoleName);
    QHash<int, QByteArray> roles_;
    bool addRowEnabled_;
    int addRowIndex_;
    bool filterApplied_;
    int nextIdValue_;
    QString addNewText_;


    QString foreignKeyFieldId_;
    QString foreignKeyFieldName_;
    int lastRowAddedIndex_;
    void dataNotFound();
    LocalStorageTable* parentTable_;
    bool sorted_;



};

