#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "qcustomplot.h"
#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
  Q_OBJECT

public:
  MainWindow(QWidget *parent = nullptr);
  ~MainWindow();
  void setupStatisticalDemo(QCustomPlot *customPlot);
  void setupDateDemo(QCustomPlot *customPlot);
  void setupRealtimeDataDemo(QCustomPlot *customPlot);
  void setupBarChartDemo(QCustomPlot *customPlot);
  void setupDemo();
private slots:
  void on_pushButton_clicked();
  void realtimeDataSlot();

private:
  Ui::MainWindow *ui;
  QTimer dataTimer;
};
#endif // MAINWINDOW_H
