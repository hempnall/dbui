#include "shoppinglistitemtablemodel.h"
#include <QDebug>

ShoppingListItemTableModel::ShoppingListItemTableModel()
{
    generateRoleNames();


    items_.push_back(  { 1 , 3 });
    items_.push_back(  { 1 , 1 });
    items_.push_back(  { 1 , 4 });
    items_.push_back(  { 1 , 1 });
    items_.push_back(  { 1 , 5 });

}

int ShoppingListItemTableModel::rowCount( const QModelIndex &parent ) const
{
    return items_.size();
}

int ShoppingListItemTableModel::columnCount( const QModelIndex &parent ) const
{
    return 2;
}

QModelIndex ShoppingListItemTableModel::parent(const QModelIndex &child) const {
    return QModelIndex();
}




QModelIndex ShoppingListItemTableModel::index(int row, int column, const QModelIndex &parent) const {
    return createIndex(row,column);
}

QVariant ShoppingListItemTableModel::data(
    const QModelIndex &index,
    int role
    ) const
{


    const ShoppingListItem& item = items_[index.row()];

    switch ( role ) {

    case (Qt::UserRole + 1):
        return QVariant( item.count );
        break;

    case (Qt::UserRole + 2):
        return QVariant( item.item_id );
        break;
    }

    return QVariant() ;

}

void ShoppingListItemTableModel::datadata( int x , int y)
{
    qDebug() << "datadata";
    beginInsertRows( QModelIndex() , items_.size()  , items_.size()  );
    items_.push_back(  { 9 , 9 }  );
    endInsertRows();
}

void ShoppingListItemTableModel::generateRoleNames()    {

   roles_[Qt::UserRole + 0 + 1] = QVariant("Quantity").toByteArray();
   roles_[Qt::UserRole + 1 + 1] = QVariant("Item").toByteArray();

}

QHash<int, QByteArray> ShoppingListItemTableModel::roleNames() const    {
    return roles_;
}


