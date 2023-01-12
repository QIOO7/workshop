#include "workshopwidget.h"
#include "ui_workshopwidget.h"

#include <QDateTime>

QT_CHARTS_USE_NAMESPACE

QChart *kChart;

workshopWidget::workshopWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::workshopWidget)
{
    ui->setupUi(this);

    InitFunc();
}

workshopWidget::~workshopWidget()
{
    delete ui;
}

void workshopWidget::InitFunc()
{
    // 构建按键信号槽
    connect(ui->queryShow, &QPushButton::clicked, this, &workshopWidget::quaryCurrent);
    connect(ui->queryCount, &QPushButton::clicked, this, &workshopWidget::quaryStatistics);

    // 初始化折线图
    kChart = new QChart();
    kChart->legend()->hide();
    kChart->createDefaultAxes();
    QChartView *chartView = new QChartView(kChart);
    chartView->setRenderHint(QPainter::Antialiasing);
    ui->showTab->layout()->addWidget(chartView);

    // 设定默认值
    ui->idShow->setText("034-389");
    ui->ptsShow->setText("2022-06-13 20:55:54");
    ui->tsShow->setText("2022-06-13 20:58:40");
    ui->idCount->setText("034-389");
    ui->ptsCount->setText("2022-06-13 20:55:54");
    ui->tsCount->setText("2022-06-13 20:58:40");

    //    ui->idCount->setText("125-61");
    //    ui->ptsCount->setText("2022-05-01 23:59:08");
    //    ui->tsCount->setText("2022-05-02 00:03:40");

    // 初始化数据库
    m_db = new DataBase("test", "process_data");

    // 初始化阈值
    QVector<QVector<double>> threshold ={{0.10, 1.00, 0, 4}, {0.00, 0.10, 0, 10}, {0.00, 4.00, 0, 4},
                                     {1.00, 3.80, 0, 4}, {0.26, 0.50, 0, 4}, {0.1, 14.0, 0, 4},
                                     {1.00, 20.00, 0, 4}, {1.90, 22.0, 0, 4}, {0.10, 0.40, 0, 4},
                                     {0.44, 2.40, 0, 4}, {1.10, 3.00, 0, 4}, {1.76, 4.00, 0, 4}};
    m_threshold.insert("034-389", threshold[0]);
    m_threshold.insert("125-61", threshold[1]);
    m_threshold.insert("125-67", threshold[2]);
    m_threshold.insert("K2-5-5", threshold[3]);
    m_threshold.insert("121-206", threshold[4]);
    m_threshold.insert("121-230", threshold[5]);
    m_threshold.insert("754-20", threshold[6]);
    m_threshold.insert("754-22", threshold[7]);
    m_threshold.insert("754-35", threshold[8]);
    m_threshold.insert("034-383", threshold[9]);
    m_threshold.insert("018-34", threshold[10]);
    m_threshold.insert("018-49", threshold[11]);
}

void workshopWidget::DrawPoints()
{
    kChart->removeAllSeries();
    QLineSeries *series = new QLineSeries();
    if (!m_showData.empty()) {
        m_showData[0]->strPts[10] = ' ';
        uint start = QDateTime::fromString(m_showData[0]->strPts.mid(0, 19), "yyyy-MM-dd hh:mm:ss").toTime_t();
        for (auto list: m_showData) {
            list->strPts[10] = ' ';
            QDateTime currentTime = QDateTime::fromString(list->strPts.mid(0, 19), "yyyy-MM-dd hh:mm:ss");
            series->append(currentTime.toTime_t() - start, list->fCurrent);
        }
    }
    kChart->addSeries(series);
}

void workshopWidget::quaryCurrent()
{
    m_showData.clear();
    m_db->getDataList(m_showData, ui->idShow->text(), ui->ptsShow->text(), ui->tsShow->text());

    DrawPoints();
};

void workshopWidget::quaryStatistics()
{
    m_countData.clear();
    m_db->getDataList(m_countData, ui->idCount->text(), ui->ptsCount->text(), ui->tsCount->text());

    int workTime = 0, idleTime = 0, shutdownTime = 0;
    int produce_num = 0, preStatus = -1;

    int prePoints = 0;
    for (auto list : m_countData) {
        list->strTs[10] = ' ';
        list->strPts[10] = ' ';
        QDateTime endTime = QDateTime::fromString(list->strTs.mid(0, 19), "yyyy-MM-dd hh:mm:ss");
        QDateTime startTime = QDateTime::fromString(list->strPts.mid(0, 19), "yyyy-MM-dd hh:mm:ss");

        int time = endTime.toTime_t() - startTime.toTime_t();  // endtime - startTime
        double i = list->fCurrent;  // 电流

        QString deviceNo = list->strEquipment;
        QVector<double> status = m_threshold[deviceNo]; //根据所选设备号取得该设备的特定阈值

        if (i > status[1]) {
            // 从非高阈值到高阈值的上坡视为一个加工数量
            if (preStatus != 2) {
                produce_num++;
            }
            preStatus = 2;
            // 工作
            workTime += time;
            prePoints = status[3];
        }else if (i > status[0]) {
            if (prePoints) {
                // 工作
                workTime += time;
                status[2]--;
            }else {
                // 空闲
                idleTime += time;
            }
            preStatus = 1;
        }else {
            if (prePoints) {
                // 工作
                workTime += time;
                status[2]--;
            }else {
                // 关机
                shutdownTime += time;
            }
            preStatus = 0;
        }
    }

    ui->counts->setText(QString::number(produce_num));
    ui->close->setText(QString::number(shutdownTime));
    ui->free->setText(QString::number(idleTime));
    ui->working->setText(QString::number(workTime));
};

