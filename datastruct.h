#ifndef DATASTRUCT_H
#define DATASTRUCT_H

#include <QAbstractTableModel>

struct MachineData {
    QString strEquipment;      // 设备
    QString strCraft;   // 加工工艺
    float   fCurrent;   // 电流
    int     nStatus;    // 三态
    QString strPts;     // 开始时间
    QString strTs;      // 结束时间
    QString strTime;    // 当前时间
};

#endif // DATASTRUCT_H
