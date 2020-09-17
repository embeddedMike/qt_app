#include "inc/mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
  ui->setupUi(this);
  //setupStatisticalDemo(ui->statisticPlot);
  setupRealtimeDataDemo(ui->realTimePlot);
}

MainWindow::~MainWindow() { delete ui; }

void MainWindow::on_pushButton_clicked() { ui->label->setText("Hello World"); }

void MainWindow::setupRealtimeDataDemo(QCustomPlot *customPlot) {
  // demoName = "Real Time Data Demo";
  customPlot->addGraph(); // blue line
  customPlot->graph(0)->setPen(QPen(QColor(40, 110, 255)));
  customPlot->yAxis->setRange(0, 100);

  // setup a timer that repeatedly calls MainWindow::realtimeDataSlot:
  connect(&dataTimer, SIGNAL(timeout()), this, SLOT(realtimeDataSlot()));
  dataTimer.start(0); // Interval 0 means to refresh as fast as possible
}

void MainWindow::realtimeDataSlot() {

  QDateTime current_time = QDateTime::currentDateTime();
  double time = current_time.toMSecsSinceEpoch() / 1000.0;

  QSharedPointer<QCPAxisTickerDateTime> dateTimeTicker(new QCPAxisTickerDateTime);
  dateTimeTicker->setTickCount(2);
  dateTimeTicker->setTickStepStrategy(QCPAxisTicker::TickStepStrategy::tssReadability);
  dateTimeTicker->setDateTimeFormat("hh:mm:ss");
  ui->realTimePlot->xAxis->setTicker(dateTimeTicker);
  // add data to lines:
  ui->realTimePlot->graph(0)->addData(time, 1.1);
  ui->realTimePlot->xAxis->setRange(time, 10000, Qt::AlignRight);
  ui->realTimePlot->replot();
}

void MainWindow::setupDemo() {}
