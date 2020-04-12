#include "curl/curl.h"
#include "inc/DbManager.hpp"
#include "inc/JsonApi.hpp"
#include "inc/JsonParser.hpp"
#include "inc/mainwindow.h"
#include "sqlite3.h"
#include "gtest/gtest.h"
#include <QApplication>
#include <algorithm>
#include <nlohmann/json.hpp>
#include <thread>
#include <vector>

static const QString path = "/Users/mike/qt_app/json_data.db";

const std::string
    urlAllStations("http://api.gios.gov.pl/pjp-api/rest/station/findAll");
const std::string
    urlSensorData("http://api.gios.gov.pl/pjp-api/rest/data/getData/401");
const std::string
    urlAir("http://api.gios.gov.pl/pjp-api/rest/aqindex/getIndex/401");
const std::string
    urlWeather("http://api.openweathermap.org/data/2.5/"
               "weather?q=London,uk&APPID=7ece3f05a22be77f9007d7513f44468a");

std::array<const std::string, 8> stationNames{
    "Kraków, Aleja Krasińskiego", "Kraków, ul. Bujaka",
    "Kraków, ul. Bulwarowa",      "Kraków, ul. Dietla",
    "Kraków, ul. Złoty Róg",      "Kraków, os. Piastów",
    "Kraków, os. Wadów",          "Kraków, os. Swoszowice"};
// std::array<const int,8> {400,401,402,10121,10123,10139,10447,11303}

// http://api.openweathermap.org/data/2.5/weather?q=Krakow&appid=7ece3f05a22be77f9007d7513f44468a

const QString locationsTable =
    "CREATE TABLE IF NOT EXISTS locations(station_id INT PRIMARY KEY NOT "
    "NULL,name TEXT);";
const QString sensorsTable = "CREATE TABLE IF NOT EXISTS sensors(sensor_id INT "
                             "PRIMARY KEY NOT NULL,station_id INT REFERENCES "
                             "locations(station_id), param_code TEXT);";

const QString readingsTable =
    "CREATE TABLE IF NOT EXISTS readings(sensor_id INT REFERENCES "
    "sensors(sensor_id), timestamp TEXT, value FLOAT);";

const QString airQualityTable =
    "CREATE TABLE IF NOT EXISTS airquality(station_id INT REFERENCES "
    "locations(station_id), timestamp TEXT, qualityIndex TEXT);";

const QString weatherTable =
    "CREATE TABLE IF NOT EXISTS weather(location_id INT NOT NULL REFERENCES "
    "locations(station_id),timestamp TEXT, temp FLOAT, pressure INT, humidity "
    "INT, wind FLOAT);";

void hello() { std::cout << "thread test" << std::endl; }

int main(int argc, char *argv[]) {

  QApplication a(argc, argv);
  MainWindow w;
  w.show();

  std::thread t(hello);
  t.join();
  JsonApi instance(urlAllStations);
  instance.initCurl();
  instance.configureCurl();
  instance.performCurl();
  instance.cleanupCurl();
  // std::cout << instance.getHttpData() << std::endl;
  /*
  using json = nlohmann::json;
  json j_complete = instance.getHttpData();
  json j = json::parse(instance.getHttpData());
  if (j.is_array()) {
    std::cout << "to tablica" << std::endl;
  }
  if (j.is_string()) {
    std::cout << "to string" << std::endl;
  }
  std::cout << j[0]["addressStreet"] << std::endl;
  for (const auto &item : j) {
    if (item["stationName"] == "Kraków, Aleja Krasińskiego") {
      std::cout << item["id"] << std::endl;
    }
  }
  */
  // std::cout << j_complete["Wrocław - Wiśniowa"] << std::endl;
  // std::cout << j_complete[0] << std::endl;
  // JsonParser instanceJsonApi(instance.getHttpData());
  // std::cout << instanceJsonApi.getUrlResponse() << std::endl;

  sqlite3 *connection = nullptr;
  int result = sqlite3_open("/Users/mike/qt_app/json_data.db", &connection);
  if (SQLITE_OK != result) {
    std::cout << "Error" << std::endl;
    sqlite3_close(connection);
  }
  std::cout << "SQLITE_OK" << result << std::endl;

  JsonParser instanceJsonApi(instance.getHttpData());
  instanceJsonApi.getStationNamesAndIds();
  instanceJsonApi.printStationNamesAndIds();

  DbManager db(path);
  QSqlQuery query;
  QString stationName = "testaas";
  db.createTable(locationsTable);
  db.createTable(sensorsTable);
  db.createTable(readingsTable);
  db.createTable(airQualityTable);
  db.createTable(weatherTable);
  db.addStationName(stationName);
  db.printAllLocations();
  // db.removeAllLocations();
  return a.exec();
}
