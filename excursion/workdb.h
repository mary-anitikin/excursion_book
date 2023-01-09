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
    int selectMaxFromTable(QString nameColumn, QString nameTable);

signals:

public slots:

private:
    QSqlDatabase db;

};

#endif // WORKDB_H
