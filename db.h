#ifndef DB_H
#define DB_H

#include <QQuickItem>

class db : public QQuickItem
{
    Q_OBJECT
    Q_DISABLE_COPY(db)

public:
    db(QQuickItem *parent = 0);
    ~db();
};

#endif // DB_H

