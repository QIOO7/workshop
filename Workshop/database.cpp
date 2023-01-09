#include "database.h"
#include "datastruct.h"

#include <QString>
#include <QDebug>
#include <QCoreApplication>

DataBase::DataBase(const QString& dbName, const QString& tableName, QObject *parent)
    : QObject(parent)
{
    if (QSqlDatabase::contains(dbName)) {
        m_DataBase = QSqlDatabase::database(dbName);
//        qDebug() << "构造初始化contains: " << dbName;
    }
    else {
        m_DataBase = QSqlDatabase::addDatabase("QSQLITE", QString(dbName));
        m_DataBase.setDatabaseName(dbName);
//        qDebug() << "构造初始化not contains: " << dbName;
    }
    m_tableName = tableName;
    initTable(tableName);
}

DataBase::~DataBase()
{
    m_DataBase.close();
}

bool DataBase::insertData(int row, MachineData *data)
{
    if (!m_DataBase.open()) {
        return false;
    }
    QSqlQuery query(m_DataBase);

    auto strSql = QString("INSERT INTO %1 (row,Id,craft,current,ctatus,pts,ts) "
                          "VALUES (%2, '%3', '%4', '%5', '%6', '%7', '%8')")
                  .arg(m_tableName)
                  .arg(row)
                  .arg(data->strId)
                  .arg(data->strCraft)
                  .arg(data->fCurrent)
                  .arg(data->nStatus)
                  .arg(data->strPts)
                  .arg(data->strTs);
    bool success = query.exec(strSql);
//    qDebug() << m_DataBase << " insert sql: "<<strSql;
    if (!success) {
        QSqlError lastError = query.lastError();
        QString err = lastError.driverText();
        qDebug()<<"exec sql failed:"<<err;
        return false;
    }
    return true;
}

bool DataBase::deleteData(int row)
{
    if (!m_DataBase.open()) {
        return false;
    }

    QSqlQuery query(m_DataBase);
    QString strSql = QString("DELETE FROM %1 WHERE row=%2").arg(m_tableName).arg(row);
//    qDebug() << m_DataBase << "str: " << strSql;
    query.prepare(strSql);
    bool success = query.exec();
    if (!success) {

        QSqlError lastError = query.lastError();
        QString err = lastError.driverText();
        qDebug()<<"exec sql failed:"<<err;
        return false;
    }

    query.prepare(QString("SELECT * FROM %1 WHERE row>%2").arg(m_tableName).arg(row));
    query.exec();
//    qDebug() << m_DataBase << query.size();
    while (query.next())
    {
        QSqlQuery query2(m_DataBase);
        QString strSql = QString("UPDATE %1 SET row='%2' WHERE row=%3").arg(m_tableName)
                          .arg(query.value("row").toInt() - 1).arg(query.value("row").toInt());
        query2.prepare(strSql);
        query2.exec();
    }

    return true;
}

bool DataBase::getDataList(QList<MachineData *> &list)
{
    if (!m_DataBase.open()) {
        return false;
    }

    QSqlQuery query(m_DataBase);
    query.prepare(QString("SELECT * FROM %1").arg(m_tableName));
    query.exec();
//    qDebug() << m_DataBase << query.size();
    while (query.next())
    {
        MachineData* data = new MachineData;
        data->strId = query.value(2).toString();
        data->strCraft = query.value(3).toString();
        data->fCurrent = query.value(4).toFloat();
        data->nStatus = query.value(5).toInt();
        data->strPts = query.value(6).toString();
        data->strTs = query.value(7).toString();
        list.append(data);

        qDebug()<<"id:"<<data->strId<<",craft:"<<data->strCraft<<",current:"
               <<data->fCurrent<<",status:"<<data->nStatus<<",pts:"
              <<data->strPts<<",ts:"<<data->strTs;
    }
    return true;
}

bool DataBase::update(int row, MachineData* pData)
{
    if (!m_DataBase.open()) {
        return false;
    }

    QSqlQuery query(m_DataBase);
    QString strSql = QString("UPDATE %1 SET Id='%2',craft='%3',current='%4',"
                             "status='%5',pts='%6',ts='%7' WHERE row=%8")
            .arg(m_tableName)
            .arg(pData->strId)
            .arg(pData->strCraft)
            .arg(pData->fCurrent)
            .arg(pData->nStatus)
            .arg(pData->strPts)
            .arg(pData->strTs)
            .arg(row);
    query.prepare(strSql);
    bool ret = query.exec();

//    qDebug() << m_DataBase << " insert sql: " << strSql;
    if (ret) {
        return true;   //新建数据库成功
    }
    else {
        QSqlError lastError = query.lastError();
        QString err = lastError.driverText();
        qDebug()<<"exec sql failed:"<<err;
        return false;
    }
    return true;
}

void DataBase::closeDatabase() {
    m_DataBase.close();
}

bool DataBase::initTable(const QString& tableName)
{
    // 打开失败
    if (!m_DataBase.open()) {
        return false;
    }
    // 判断表是否存在，不存在则创建
    if (!isExistTable(tableName)) {
        createTable(tableName);
    }
    return false;
}

bool DataBase::isExistTable(const QString& tableName)
{
    bool bRet = false;
    if (!m_DataBase.open()) {
        return bRet;
    }
    QSqlQuery query(m_DataBase);
    query.exec(QString("select count(*) from sqlite_master where type='table' and name='%1'").arg(tableName));    //关键的判断
    if (query.next())
    {
        if (query.value(0).toInt() > 0)
        {
            bRet = true;
        }
    }
    return bRet;
}

bool DataBase::createTable(const QString& tableName)
{
    if (!m_DataBase.open()) {
        return false;
    }

    QSqlQuery query(m_DataBase);
    bool success = query.exec(QString("CREATE TABLE %1 ("
                              "id INTEGER PRIMARY KEY AUTOINCREMENT, "
                              "row INTEGER NOT NULL,"
                              "Id VARCHAR(40) NOT NULL, "
                              "craft VARCHAR(40) NOT NULL, "
                              "current FLOAT NOT NULL,"
                              "status INTEGER NOT NULL,"
                              "pts VARCHAR(10) NOT NULL,"
                              "ts VARCHAR(10))").arg(tableName));
    qDebug() << m_DataBase << " success";

    if (success) {
        return true;   //新建数据库成功
    }
    else {
        QSqlError lastError = query.lastError();
        QString err = lastError.driverText();
        qDebug()<<err;
        return false;
    }

}
