#ifndef DATABASE_H
#define DATABASE_H

#include <QObject>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlDriver>
#include <QtSql/QSqlError>

struct MachineData;
class DataBase : public QObject
{
    Q_OBJECT

public:
    DataBase(const QString& dbName, const QString& tableName, QObject *parent = Q_NULLPTR);
    ~DataBase();
public:
    // 插入数据，用行号做索引
    bool insertData(int row, MachineData* data);
    // 删除某行数据
    bool deleteData(int nCanId);
    // 获取数据
    bool getDataList(QList<MachineData*>& list);
    // 更新数据
    bool update(int row, MachineData* pData);

    // 关闭数据库
    void closeDatabase();

private:
    bool initTable(const QString& tableName);
    bool isExistTable(const QString& tableName);
    bool createTable(const QString& tableName);
private:
    QSqlDatabase m_DataBase;
    QString m_tableName;
};

#endif // DATABASE_H
