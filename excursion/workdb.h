#ifndef WORKDB_H
#define WORKDB_H

#include <QWidget>
#include <QSqlDatabase>


class WorkDB : public QWidget
{
    Q_OBJECT
public:
    explicit WorkDB(QWidget *parent = nullptr);
    ~WorkDB();

    QSqlDatabase getDB();
    void createTable();
    void fillTableNormative();

signals:

public slots:

};

#endif // WORKDB_H
