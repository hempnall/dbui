#ifndef SHOPPINGLISTITEMTABLEMODEL_H
#define SHOPPINGLISTITEMTABLEMODEL_H


#include <QVector>
#include <QHash>
#include <QByteArray>
#include <QAbstractItemModel>

struct ShoppingListItem {
    int count;
    int item_id;
};


class ShoppingListItemTableModel
    : public QAbstractItemModel
{
    Q_OBJECT

    QVector<ShoppingListItem> items_;

public:
    ShoppingListItemTableModel();
    Q_INVOKABLE QVariant data(const QModelIndex &index, int role=Qt::DisplayRole ) const;
    Q_INVOKABLE void datadata( int x , int y  ) ;

    virtual QHash<int, QByteArray> roleNames() const;
    Q_INVOKABLE virtual int rowCount(const QModelIndex & parent = QModelIndex()) const;
    virtual int columnCount( const QModelIndex &parent = QModelIndex()) const  ;
    Q_INVOKABLE virtual QModelIndex parent(const QModelIndex &child) const;
    void generateRoleNames();
    QHash<int, QByteArray> roles_;
    QModelIndex index(int row, int column, const QModelIndex &parent) const;





signals:

public slots:
};

#endif // SHOPPINGLISTITEMTABLEMODEL_H
