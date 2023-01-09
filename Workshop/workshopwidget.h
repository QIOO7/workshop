#ifndef WORKSHOPWIDGET_H
#define WORKSHOPWIDGET_H

#include <QWidget>
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

private:
    Ui::workshopWidget *ui;
};
#endif // WORKSHOPWIDGET_H
