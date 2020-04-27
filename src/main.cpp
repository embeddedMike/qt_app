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
    urlSensors("http://api.gios.gov.pl/pjp-api/rest/station/sensors/");
const std::string
    urlSensorData("http://api.gios.gov.pl/pjp-api/rest/data/getData/");
const std::string
    urlAir("http://api.gios.gov.pl/pjp-api/rest/aqindex/getIndex/");
const std::string
    urlWeather("http://api.openweathermap.org/data/2.5/"
               "weather?q=Krakow&appid=7ece3f05a22be77f9007d7513f44468a");

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

  // std::thread t(hello);
  // t.join();

  DbManager db(path);
  QSqlQuery query;

  db.createTable(locationsTable);
  db.createTable(sensorsTable);
  db.createTable(readingsTable);
  db.createTable(airQualityTable);
  db.createTable(weatherTable);

  db.removeAllSensors();
  db.removeAllLocations();

  JsonApi instance(urlAllStations);
  instance.initCurl();
  instance.configureCurl();
  instance.performCurl();
  instance.cleanupCurl();

  JsonParser instanceJsonApi(instance.getHttpData());
  instanceJsonApi.fetchStationNamesAndIds();
  instanceJsonApi.printStationNamesAndIds();

  for (const auto &[id, station] : instanceJsonApi.getStationNameAndIds()) {
    QString stationName = QString::fromLocal8Bit(station.c_str());
    db.addLocations(id, stationName);
  }

  db.printAllLocations();

  for (const auto &[id, station] : instanceJsonApi.getStationNameAndIds()) {
    std::string url("");

    url.assign(urlSensors + std::to_string(id));
    instance.setUrl(url);
    instance.initCurl();
    instance.configureCurl();
    instance.performCurl();
    instance.cleanupCurl();
    instanceJsonApi.setUrlResponse(instance.getHttpData());
    instanceJsonApi.fetchSensorIdAndParamCode();
    instanceJsonApi.printSensorIdAndParamCode();
    instanceJsonApi.printSensorIdAndParamCodeBuffer();
    for (const auto &[sensorId, paramCode] :
         instanceJsonApi.getSensorIdWithParamCodeBuffer()) {
      QString fetchedParamCode = QString::fromLocal8Bit(paramCode.c_str());
      db.addSensors(sensorId, id, fetchedParamCode);
    }
    instanceJsonApi.clearSensorIdWithParamCodeBuffer();
    url.clear();
  }

  for (const auto &[id, paramCode] :
       instanceJsonApi.getSensorIdWithParamCode()) {
    std::string url("");
    url.assign(urlSensorData + std::to_string(id));
    instance.setUrl(url);
    instance.initCurl();
    instance.configureCurl();
    instance.performCurl();
    instance.cleanupCurl();
    instanceJsonApi.setUrlResponse(instance.getHttpData());
    instanceJsonApi.fetchSensorRead();
    instanceJsonApi.printSensorRead();
    url.clear();
  }
  for (const auto &[id, station] : instanceJsonApi.getStationNameAndIds()) {
    std::string url("");
    url.assign(urlAir + std::to_string(id));
    instance.setUrl(url);
    instance.initCurl();
    instance.configureCurl();
    instance.performCurl();
    instance.cleanupCurl();
    instanceJsonApi.setUrlResponse(instance.getHttpData());
    instanceJsonApi.fetchStationAirQuality();
    instanceJsonApi.printStationAirQuality();
  }

  instance.setUrl(urlWeather);
  instance.initCurl();
  instance.configureCurl();
  instance.performCurl();
  instance.cleanupCurl();
  instanceJsonApi.setUrlResponse(instance.getHttpData());
  instanceJsonApi.fetchCracowId();
  instanceJsonApi.printCityId();
  instanceJsonApi.fetchWeatherData();
  // std::cout << instanceJsonApi.getWeatherDataHandler()->wind << std::endl;
  instanceJsonApi.printWeatherData();

  // db.removeAllLocations();

  return a.exec();
}
