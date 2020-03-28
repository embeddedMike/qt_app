#include "curl/curl.h"
#include "inc/DbManager.hpp"
#include "inc/JsonApiAirQuality.hpp"
#include "inc/mainwindow.h"
#include "sqlite3.h"
#include "gtest/gtest.h"
#include <QApplication>
#include <QDebug>
#include <algorithm>
#include <nlohmann/json.hpp>
#include <vector>

static const QString path = "/Users/mike/qt_app/people.db";
const std::string
    url("http://api.gios.gov.pl/pjp-api/rest/aqindex/getIndex/52");

int main(int argc, char *argv[]) {

  QApplication a(argc, argv);
  MainWindow w;
  w.show();

  JsonApiAirQuality instance(url);
  instance.initCurl();
  instance.configureCurl();
  instance.performCurl();
  instance.cleanupCurl();
  /*
  using json = nlohmann::json;
  json j_complete = json::parse(httpData);

  std::cout << j_complete["stCalcDate"] << std::endl;
  */
  sqlite3 *connection = nullptr;
  int result = sqlite3_open("/Users/mike/qt_app/people.db", &connection);
  if (SQLITE_OK != result) {
    std::cout << "Error" << std::endl;
    sqlite3_close(connection);
  }
  std::cout << "SQLITE_OK" << result << std::endl;

  DbManager db(path);
  QSqlQuery query;
  QString name = "abc";
  query.prepare("INSERT INTO people (name) VALUES (:name)");
  query.bindValue(":name", name);
  qDebug() << "addPersonn error: " << query.lastError();
  if (query.exec()) {
    qDebug() << "addPersonn errorrrr: " << query.lastError();
  }
  db.printAllPersons();

  return a.exec();
}
