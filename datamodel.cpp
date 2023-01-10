#include "datamodel.h"
#include "database.h"
#include <QDebug>

DataModel::DataModel(QObject *parent)
    : QAbstractTableModel(parent)
{
}

DataModel::~DataModel()
{
    qDeleteAll(m_dataMap.begin(),m_dataMap.end());
}

void DataModel::dataInitialize(const QString& dbName)
{
    m_pDataBase = new DataBase(dbName, "MachineData", this);
    QList<MachineData*> list;
    m_pDataBase->getDataList(list);
    addData(list);
    m_bInitialized = true;
}

void DataModel::addData(MachineData *pData)
{
    if(pData == nullptr) return;

    beginResetModel();
    m_dataMap.insert(m_nRow, pData);
    ++m_nRow;
    endResetModel();
}

void DataModel::addData(QList<MachineData *> &list)
{
    for(auto& obj : list)
        addData(obj);
}

void DataModel::closeDatabase() {
    m_pDataBase->closeDatabase();
    m_bInitialized = false;
}

void DataModel::clearData()
{
    beginResetModel();
    m_dataMap.clear();
    m_nRow = 0;
    endResetModel();
}

void DataModel::delData(int nRow)
{
    beginResetModel();
    if(nRow >= m_dataMap.size()){
        return;
    }

    for(int i = nRow; i + 1 < m_nRow; ++i){
        m_dataMap[i] = m_dataMap[i + 1];
    }
    m_dataMap.pop_back();
    endResetModel();
}
