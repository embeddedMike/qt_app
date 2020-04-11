#include "inc/mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
  ui->setupUi(this);
  setupStatisticalDemo(ui->statisticPlot);
  setupBarChartDemo(ui->barPlot);
  setupRealtimeDataDemo(ui->realTimePlot);
  setupDateDemo(ui->weatherPlot);
  setupDateDemo(ui->airConditionPlot);
}

MainWindow::~MainWindow() { delete ui; }

void MainWindow::on_pushButton_clicked() { ui->label->setText("Hello World"); }

void MainWindow::setupStatisticalDemo(QCustomPlot *customPlot) {
  // demoName = "Statistical Demo";
  QCPStatisticalBox *statistical =
      new QCPStatisticalBox(customPlot->xAxis, customPlot->yAxis);
  QBrush boxBrush(QColor(60, 60, 255, 100));
  boxBrush.setStyle(Qt::Dense6Pattern); // make it look oldschool
  statistical->setBrush(boxBrush);

  // specify data:
  statistical->addData(1, 1.1, 1.9, 2.25, 2.7, 4.2);
  statistical->addData(2, 0.8, 1.6, 2.2, 3.2, 4.9,
                       QVector<double>()
                           << 0.7 << 0.34 << 0.45 << 6.2
                           << 5.84); // provide some outliers as QVector
  statistical->addData(3, 0.2, 0.7, 1.1, 1.6, 2.9);

  // prepare manual x axis labels:
  customPlot->xAxis->setSubTicks(false);
  customPlot->xAxis->setTickLength(0, 4);
  customPlot->xAxis->setTickLabelRotation(20);
  QSharedPointer<QCPAxisTickerText> textTicker(new QCPAxisTickerText);
  textTicker->addTick(1, "Sample 1");
  textTicker->addTick(2, "Sample 2");
  textTicker->addTick(3, "Control Group");
  customPlot->xAxis->setTicker(textTicker);

  // prepare axes:
  customPlot->yAxis->setLabel(QString::fromUtf8("O₂ Absorption [mg]"));
  customPlot->rescaleAxes();
  customPlot->xAxis->scaleRange(1.7, customPlot->xAxis->range().center());
  customPlot->yAxis->setRange(0, 7);
  customPlot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);
}

void MainWindow::setupDateDemo(QCustomPlot *customPlot) {
  // demoName = "Date Demo";
  // set locale to english, so we get english month names:
  customPlot->setLocale(QLocale(QLocale::English, QLocale::UnitedKingdom));
  // seconds of current time, we'll use it as starting point in time for data:
  double now = QDateTime::currentDateTime().toTime_t();
  srand(8); // set the random seed, so we always get the same random data
  // create multiple graphs:
  for (int gi = 0; gi < 5; ++gi) {
    customPlot->addGraph();
    QColor color(20 + 200 / 4.0 * gi, 70 * (1.6 - gi / 4.0), 150, 150);
    customPlot->graph()->setLineStyle(QCPGraph::lsLine);
    customPlot->graph()->setPen(QPen(color.lighter(200)));
    customPlot->graph()->setBrush(QBrush(color));
    // generate random walk data:
    QVector<QCPGraphData> timeData(250);
    for (int i = 0; i < 250; ++i) {
      timeData[i].key = now + 24 * 3600 * i;
      if (i == 0)
        timeData[i].value = (i / 50.0 + 1) * (rand() / (double)RAND_MAX - 0.5);
      else
        timeData[i].value =
            qFabs(timeData[i - 1].value) * (1 + 0.02 / 4.0 * (4 - gi)) +
            (i / 50.0 + 1) * (rand() / (double)RAND_MAX - 0.5);
    }
    customPlot->graph()->data()->set(timeData);
  }
  // configure bottom axis to show date instead of number:
  QSharedPointer<QCPAxisTickerDateTime> dateTicker(new QCPAxisTickerDateTime);
  dateTicker->setDateTimeFormat("d. MMMM\nyyyy");
  customPlot->xAxis->setTicker(dateTicker);
  // configure left axis text labels:
  QSharedPointer<QCPAxisTickerText> textTicker(new QCPAxisTickerText);
  textTicker->addTick(10, "a bit\nlow");
  textTicker->addTick(50, "quite\nhigh");
  customPlot->yAxis->setTicker(textTicker);
  // set a more compact font size for bottom and left axis tick labels:
  customPlot->xAxis->setTickLabelFont(QFont(QFont().family(), 8));
  customPlot->yAxis->setTickLabelFont(QFont(QFont().family(), 8));
  // set axis labels:
  customPlot->xAxis->setLabel("Date");
  customPlot->yAxis->setLabel("Random wobbly lines value");
  // make top and right axes visible but without ticks and labels:
  customPlot->xAxis2->setVisible(true);
  customPlot->yAxis2->setVisible(true);
  customPlot->xAxis2->setTicks(false);
  customPlot->yAxis2->setTicks(false);
  customPlot->xAxis2->setTickLabels(false);
  customPlot->yAxis2->setTickLabels(false);
  // set axis ranges to show all data:
  customPlot->xAxis->setRange(now, now + 24 * 3600 * 249);
  customPlot->yAxis->setRange(0, 60);
  // show legend with slightly transparent background brush:
  customPlot->legend->setVisible(true);
  customPlot->legend->setBrush(QColor(255, 255, 255, 150));
}

void MainWindow::setupRealtimeDataDemo(QCustomPlot *customPlot) {
  // demoName = "Real Time Data Demo";

  // include this section to fully disable antialiasing for higher performance:
  /*
customPlot->setNotAntialiasedElements(QCP::aeAll);
QFont font;
font.setStyleStrategy(QFont::NoAntialias);
customPlot->xAxis->setTickLabelFont(font);
customPlot->yAxis->setTickLabelFont(font);
customPlot->legend->setFont(font);
*/
  customPlot->addGraph(); // blue line
  customPlot->graph(0)->setPen(QPen(QColor(40, 110, 255)));
  customPlot->addGraph(); // red line
  customPlot->graph(1)->setPen(QPen(QColor(255, 110, 40)));

  QSharedPointer<QCPAxisTickerTime> timeTicker(new QCPAxisTickerTime);
  timeTicker->setTimeFormat("%h:%m:%s");
  customPlot->xAxis->setTicker(timeTicker);
  customPlot->axisRect()->setupFullAxesBox();
  customPlot->yAxis->setRange(-1.2, 1.2);

  // make left and bottom axes transfer their ranges to right and top axes:
  connect(customPlot->xAxis, SIGNAL(rangeChanged(QCPRange)), customPlot->xAxis2,
          SLOT(setRange(QCPRange)));
  connect(customPlot->yAxis, SIGNAL(rangeChanged(QCPRange)), customPlot->yAxis2,
          SLOT(setRange(QCPRange)));

  // setup a timer that repeatedly calls MainWindow::realtimeDataSlot:
  connect(&dataTimer, SIGNAL(timeout()), this, SLOT(realtimeDataSlot()));
  dataTimer.start(0); // Interval 0 means to refresh as fast as possible
}

void MainWindow::setupBarChartDemo(QCustomPlot *customPlot) {
  // demoName = "Bar Chart Demo";
  // set dark background gradient:
  QLinearGradient gradient(0, 0, 0, 400);
  gradient.setColorAt(0, QColor(90, 90, 90));
  gradient.setColorAt(0.38, QColor(105, 105, 105));
  gradient.setColorAt(1, QColor(70, 70, 70));
  customPlot->setBackground(QBrush(gradient));

  // create empty bar chart objects:
  QCPBars *regen = new QCPBars(customPlot->xAxis, customPlot->yAxis);
  QCPBars *nuclear = new QCPBars(customPlot->xAxis, customPlot->yAxis);
  QCPBars *fossil = new QCPBars(customPlot->xAxis, customPlot->yAxis);
  regen->setAntialiased(false); // gives more crisp, pixel aligned bar borders
  nuclear->setAntialiased(false);
  fossil->setAntialiased(false);
  regen->setStackingGap(1);
  nuclear->setStackingGap(1);
  fossil->setStackingGap(1);
  // set names and colors:
  fossil->setName("Fossil fuels");
  fossil->setPen(QPen(QColor(111, 9, 176).lighter(170)));
  fossil->setBrush(QColor(111, 9, 176));
  nuclear->setName("Nuclear");
  nuclear->setPen(QPen(QColor(250, 170, 20).lighter(150)));
  nuclear->setBrush(QColor(250, 170, 20));
  regen->setName("Regenerative");
  regen->setPen(QPen(QColor(0, 168, 140).lighter(130)));
  regen->setBrush(QColor(0, 168, 140));
  // stack bars on top of each other:
  nuclear->moveAbove(fossil);
  regen->moveAbove(nuclear);

  // prepare x axis with country labels:
  QVector<double> ticks;
  QVector<QString> labels;
  ticks << 1 << 2 << 3 << 4 << 5 << 6 << 7;
  labels << "USA"
         << "Japan"
         << "Germany"
         << "France"
         << "UK"
         << "Italy"
         << "Canada";
  QSharedPointer<QCPAxisTickerText> textTicker(new QCPAxisTickerText);
  textTicker->addTicks(ticks, labels);
  customPlot->xAxis->setTicker(textTicker);
  customPlot->xAxis->setTickLabelRotation(60);
  customPlot->xAxis->setSubTicks(false);
  customPlot->xAxis->setTickLength(0, 4);
  customPlot->xAxis->setRange(0, 8);
  customPlot->xAxis->setBasePen(QPen(Qt::white));
  customPlot->xAxis->setTickPen(QPen(Qt::white));
  customPlot->xAxis->grid()->setVisible(true);
  customPlot->xAxis->grid()->setPen(
      QPen(QColor(130, 130, 130), 0, Qt::DotLine));
  customPlot->xAxis->setTickLabelColor(Qt::white);
  customPlot->xAxis->setLabelColor(Qt::white);

  // prepare y axis:
  customPlot->yAxis->setRange(0, 12.1);
  customPlot->yAxis->setPadding(5); // a bit more space to the left border
  customPlot->yAxis->setLabel(
      "Power Consumption in\nKilowatts per Capita (2007)");
  customPlot->yAxis->setBasePen(QPen(Qt::white));
  customPlot->yAxis->setTickPen(QPen(Qt::white));
  customPlot->yAxis->setSubTickPen(QPen(Qt::white));
  customPlot->yAxis->grid()->setSubGridVisible(true);
  customPlot->yAxis->setTickLabelColor(Qt::white);
  customPlot->yAxis->setLabelColor(Qt::white);
  customPlot->yAxis->grid()->setPen(
      QPen(QColor(130, 130, 130), 0, Qt::SolidLine));
  customPlot->yAxis->grid()->setSubGridPen(
      QPen(QColor(130, 130, 130), 0, Qt::DotLine));

  // Add data:
  QVector<double> fossilData, nuclearData, regenData;
  fossilData << 0.86 * 10.5 << 0.83 * 5.5 << 0.84 * 5.5 << 0.52 * 5.8
             << 0.89 * 5.2 << 0.90 * 4.2 << 0.67 * 11.2;
  nuclearData << 0.08 * 10.5 << 0.12 * 5.5 << 0.12 * 5.5 << 0.40 * 5.8
              << 0.09 * 5.2 << 0.00 * 4.2 << 0.07 * 11.2;
  regenData << 0.06 * 10.5 << 0.05 * 5.5 << 0.04 * 5.5 << 0.06 * 5.8
            << 0.02 * 5.2 << 0.07 * 4.2 << 0.25 * 11.2;
  fossil->setData(ticks, fossilData);
  nuclear->setData(ticks, nuclearData);
  regen->setData(ticks, regenData);

  // setup legend:
  customPlot->legend->setVisible(true);
  customPlot->axisRect()->insetLayout()->setInsetAlignment(
      0, Qt::AlignTop | Qt::AlignHCenter);
  customPlot->legend->setBrush(QColor(255, 255, 255, 100));
  customPlot->legend->setBorderPen(Qt::NoPen);
  QFont legendFont = font();
  legendFont.setPointSize(10);
  customPlot->legend->setFont(legendFont);
  customPlot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);
}

void MainWindow::realtimeDataSlot() {
  static QTime time(QTime::currentTime());
  // calculate two new data points:
  double key =
      time.elapsed() / 1000.0; // time elapsed since start of demo, in seconds
  static double lastPointKey = 0;
  if (key - lastPointKey > 0.002) // at most add point every 2 ms
  {
    // add data to lines:
    ui->realTimePlot->graph(0)->addData(
        key, qSin(key) + qrand() / (double)RAND_MAX * 1 * qSin(key / 0.3843));
    ui->realTimePlot->graph(1)->addData(
        key, qCos(key) + qrand() / (double)RAND_MAX * 0.5 * qSin(key / 0.4364));
    // rescale value (vertical) axis to fit the current data:
    // ui->customPlot->graph(0)->rescaleValueAxis();
    // ui->customPlot->graph(1)->rescaleValueAxis(true);
    lastPointKey = key;
  }
  // make key axis range scroll with the data (at a constant range size of 8):
  ui->realTimePlot->xAxis->setRange(key, 8, Qt::AlignRight);
  ui->realTimePlot->replot();

  // calculate frames per second:
  /*
  static double lastFpsKey;
  static int frameCount;
  ++frameCount;
  if (key - lastFpsKey > 2) // average fps over 2 seconds
  {
    ui->statusBar->showMessage(
        QString("%1 FPS, Total Data points: %2")
            .arg(frameCount / (key - lastFpsKey), 0, 'f', 0)
            .arg(ui->realTimePlot->graph(0)->data()->size() +
                 ui->realTimePlot->graph(1)->data()->size()),
        0);
    lastFpsKey = key;
    frameCount = 0;
  }
 */
}

void MainWindow::setupDemo() {}
