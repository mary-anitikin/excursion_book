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
    void fillTableStart(); //заполнение таблиц исходными данными задачи
    int selectMaxFromTable(QString nameColumn, QString nameTable);
    void deleteRowFromTable(int idR, QString idName, QString nameTable);

signals:

public slots:

private:
    QSqlDatabase db;

};

#endif // WORKDB_H
