#include "database.h"
#include "datastruct.h"

#include <QString>
#include <QDebug>
#include <QCoreApplication>

DataBase::DataBase(const QString& dbName, const QString& tableName, QObject *parent)
{
    if (QSqlDatabase::contains(dbName)) {
        m_DataBase = QSqlDatabase::database(dbName);
//        qDebug() << "构造初始化contains: " << dbName;
    }
    else {
        m_DataBase = QSqlDatabase::addDatabase("QMYSQL", QString(dbName));
        m_DataBase.setDatabaseName(dbName);
//        qDebug() << "构造初始化not contains: " << dbName;
    }
    m_DataBase.setHostName("127.0.0.1");
    m_DataBase.setPort(3306);
    m_DataBase.setUserName("root");
    m_DataBase.setPassword("123456");
    bool ok = m_DataBase.open();
    if (ok){
        qDebug()<<"open database success";
    }
    else {
       // qDebug()<<"error open database because"<<m_db.lastError().text();
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

    auto strSql = QString("INSERT INTO %1 (row,equipment,craft,current,pts,ts) "
                          "VALUES (%2, '%3', '%4', '%5', '%6', '%7')")
                  .arg(m_tableName)
                  .arg(row)
                  .arg(data->strEquipment)
                  .arg(data->strCraft)
                  .arg(data->fCurrent)
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

bool DataBase::getDataList(QList<MachineData *> &list,const QString &device, const QString &startTime, const QString &endTime)
{
    if (!m_DataBase.open()) {
        return false;
    }

    QSqlQuery query(m_DataBase);
    query.prepare(QString("SELECT * FROM %1 WHERE starting_time >= '%2' AND end_time <= '%3' AND device_no = '%4'").arg(m_tableName).arg(startTime).arg(endTime).arg(device));


    query.exec();
//    qDebug() << m_DataBase << query.size();
    while (query.next())
    {
        MachineData* data = new MachineData;
        data->strEquipment = query.value(2).toString();
        data->strCraft = query.value(1).toString();
        data->fCurrent = query.value(3).toFloat();
        data->strPts = query.value(4).toString();
        data->strTs = query.value(5).toString();
        list.append(data);

        qDebug()<<"equipment:"<<data->strEquipment<<",craft:"<<data->strCraft<<",current:"
               <<data->fCurrent<<",pts:"<<data->strPts<<",ts:"<<data->strTs;
    }
    return true;
}

bool DataBase::update(int row, MachineData* pData)
{
    if (!m_DataBase.open()) {
        return false;
    }

    QSqlQuery query(m_DataBase);
    QString strSql = QString("UPDATE %1 SET equipment='%2',craft='%3',current='%4',"
                             "pts='%6',ts='%7' WHERE row=%8")
            .arg(m_tableName)
            .arg(pData->strEquipment)
            .arg(pData->strCraft)
            .arg(pData->fCurrent)
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
                              "equipment VARCHAR(255) NOT NULL COMMENT'设备', "
                              "craft VARCHAR(255) NOT NULL COMMENT'加工工艺', "
                              "current FLOAT DEFAULT '0.0' COMMENT'电流',"
                              "pts DATETIME NOT NULL DEFAULT CURRENT_TIMESTAMP COMMENT'开始时间',"
                              "ts DATETIME NOT NULL DEFAULT CURRENT_TIMESTAMP COMMENT'结束时间')").arg(tableName));
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
