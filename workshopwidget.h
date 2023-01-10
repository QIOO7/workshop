#ifndef WORKSHOPWIDGET_H
#define WORKSHOPWIDGET_H

#include <QWidget>
#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>
#include "datamodel.h"

QT_BEGIN_NAMESPACE
namespace Ui { class workshopWidget; }
QT_END_NAMESPACE

class workshopWidget : public QWidget
{
    Q_OBJECT

public:
    workshopWidget(QWidget *parent = nullptr);
    ~workshopWidget();

    /**
     * @brief InitFunc 初始化界面功能
     */
    void InitFunc();

    /**
     * @brief DrawPoints 绘制电流图
     */
    void DrawPoints();

private slots:
    /**
     * @brief quaryCurrent 查询电流图
     */
    void quaryCurrent();

    /**
     * @brief quaryStatistics 查询统计量
     */
    void quaryStatistics();

private:
    Ui::workshopWidget *ui;
};
#endif // WORKSHOPWIDGET_H
