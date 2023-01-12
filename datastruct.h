#ifndef DATASTRUCT_H
#define DATASTRUCT_H

#include <QAbstractTableModel>

struct MachineData {
    QString strEquipment;   // 设备
    QString strCraft;       // 加工工艺
    float   fCurrent;       // 电流
    QString strPts;         // 开始时间
    QString strTs;          // 结束时间
};

#endif // DATASTRUCT_H
