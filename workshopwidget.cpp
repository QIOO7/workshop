#include "workshopwidget.h"
#include "ui_workshopwidget.h"

QT_CHARTS_USE_NAMESPACE

workshopWidget::workshopWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::workshopWidget)
{
    ui->setupUi(this);

    InitFunc();
    DrawPoints();
}

workshopWidget::~workshopWidget()
{
    delete ui;
}

void workshopWidget::InitFunc()
{
    connect(ui->queryShow, &QPushButton::clicked, this, &workshopWidget::quaryStatistics);
    connect(ui->queryCount, &QPushButton::clicked, this, &workshopWidget::quaryStatistics);
}

void workshopWidget::DrawPoints()
{
    QLineSeries *series = new QLineSeries();
    series->append(0, 6);
    series->append(2, 4);
    series->append(3, 8);
    series->append(7, 4);
    series->append(10, 5);
    *series << QPointF(11, 1) << QPointF(13, 3) << QPointF(17, 6) << QPointF(18, 3) << QPointF(20, 2);

    QChart *chart = new QChart();
    chart->legend()->hide();
    chart->addSeries(series);
    chart->createDefaultAxes();

    QChartView *chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);
    ui->showTab->layout()->addWidget(chartView);
}

void workshopWidget::quaryCurrent()
{

}

void workshopWidget::quaryStatistics()
{

}

