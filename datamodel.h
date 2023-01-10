#ifndef DataModel_H
#define DataModel_H

#include <QAbstractTableModel>
#include <QMutex>

struct MachineData;
class DataBase;
class DataModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    explicit DataModel(QObject *parent = nullptr);
    ~DataModel();

    void dataInitialize(const QString& dbName = "data.db");

    void addData(MachineData* pData);
    void addData(QList<MachineData*>& list);

    void closeDatabase();
    void clearData();
    void delData(int nRow);

private:
    DataBase* m_pDataBase;
    bool m_bInitialized = false;

    QVector<MachineData*> m_dataMap;
    quint32 m_nRow = 0;
};

#endif // DataModel_H
