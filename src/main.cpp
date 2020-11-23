//#include "curl/curl.h"
#include <curl/curl.h>
#include "inc/DbManager.hpp"
#include "inc/JsonApi.hpp"
#include "inc/JsonParser.hpp"
#include "inc/mainwindow.h"
#include "sqlite3.h"
//#include "gtest/gtest.h"
#include <QApplication>
#include <algorithm>
#include <chrono>
#include <nlohmann/json.hpp>
#include <thread>
#include <vector>

static const QString path = "/Users/mike/Projects/qt_app/json_data.db";

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

void logReadings(DbManager& db, JsonApi& jsonApiInstance, JsonParser& jsonParserInstance);
void logAirQuality(DbManager& db, JsonApi& jsonApiInstance, JsonParser& jsonParserInstance);
void logWeather(DbManager& db, JsonApi& jsonApiInstance, JsonParser& jsonParserInstance);

void logData(JsonApi& jsonApiInstance, JsonParser& jsonParserInstance) {
  std::cout << "Thread - Log Data" << std::endl;
  DbManager db(path);
  for ( ; ; ) {
    logReadings(db, jsonApiInstance, jsonParserInstance);
    logAirQuality(db, jsonApiInstance, jsonParserInstance);
    logWeather(db, jsonApiInstance, jsonParserInstance);
    std::this_thread::sleep_for(std::chrono::hours(1));
  }
}

int main(int argc, char *argv[]) {

  QApplication a(argc, argv);
  MainWindow w;
  w.show();




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
/*
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
    QString fetchedTimestamp =
        QString::fromLocal8Bit(instanceJsonApi.getSensorRead().first.c_str());
    db.addReadings(id, fetchedTimestamp,
                   instanceJsonApi.getSensorRead().second);
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
    QString fetchedTimestamp = QString::fromLocal8Bit(
        instanceJsonApi.getStationAirQuality().first.c_str());
    QString fetchedValue = QString::fromLocal8Bit(
        instanceJsonApi.getStationAirQuality().second.c_str());
    db.addAirQuality(id, fetchedTimestamp, fetchedValue);
    url.clear();
  }

  QString weatherCity = "Kraków";
  instance.setUrl(urlWeather);
  instance.initCurl();
  instance.configureCurl();
  instance.performCurl();
  instance.cleanupCurl();
  instanceJsonApi.setUrlResponse(instance.getHttpData());
  instanceJsonApi.fetchCracowId();
  instanceJsonApi.printCityId();
  instanceJsonApi.fetchWeatherData();
  instanceJsonApi.printWeatherData();
  db.addLocations(instanceJsonApi.getCityId(), weatherCity);
  db.addWeather(instanceJsonApi.getCityId(),
                instanceJsonApi.getWeatherDataHandler());
*/
  //logReadings(db, instance, instanceJsonApi);
  //logAirQuality(db, instance, instanceJsonApi);
  //logWeather(db, instance, instanceJsonApi);
  std::thread t{logData, std::ref(instance), std::ref(instanceJsonApi)};
  t.detach();

  return a.exec();
}

void logReadings(DbManager& db, JsonApi& jsonApiInstance, JsonParser& jsonParserInstance)
{
    for (const auto &[id, paramCode] :
         jsonParserInstance.getSensorIdWithParamCode()) {
      std::string url("");
      url.assign(urlSensorData + std::to_string(id));
      jsonApiInstance.setUrl(url);
      jsonApiInstance.initCurl();
      jsonApiInstance.configureCurl();
      jsonApiInstance.performCurl();
      jsonApiInstance.cleanupCurl();
      jsonParserInstance.setUrlResponse(jsonApiInstance.getHttpData());
      jsonParserInstance.fetchSensorRead();
      jsonParserInstance.printSensorRead();
      QString fetchedTimestamp =
          QString::fromLocal8Bit(jsonParserInstance.getSensorRead().first.c_str());
      db.addReadings(id, fetchedTimestamp,
                     jsonParserInstance.getSensorRead().second);
      url.clear();
    }
}

void logAirQuality(DbManager& db, JsonApi& jsonApiInstance, JsonParser& jsonParserInstance)
{
    for (const auto &[id, station] : jsonParserInstance.getStationNameAndIds()) {
      std::string url("");
      url.assign(urlAir + std::to_string(id));
      jsonApiInstance.setUrl(url);
      jsonApiInstance.initCurl();
      jsonApiInstance.configureCurl();
      jsonApiInstance.performCurl();
      jsonApiInstance.cleanupCurl();
      jsonParserInstance.setUrlResponse(jsonApiInstance.getHttpData());
      jsonParserInstance.fetchStationAirQuality();
      jsonParserInstance.printStationAirQuality();
      QString fetchedTimestamp = QString::fromLocal8Bit(
          jsonParserInstance.getStationAirQuality().first.c_str());
      QString fetchedValue = QString::fromLocal8Bit(
          jsonParserInstance.getStationAirQuality().second.c_str());
      db.addAirQuality(id, fetchedTimestamp, fetchedValue);
      url.clear();
    }
}

void logWeather(DbManager& db, JsonApi& jsonApiInstance, JsonParser& jsonParserInstance)
{
    QString weatherCity = "Kraków";
    static bool isLocationAdded = false;
    jsonApiInstance.setUrl(urlWeather);
    jsonApiInstance.initCurl();
    jsonApiInstance.configureCurl();
    jsonApiInstance.performCurl();
    jsonApiInstance.cleanupCurl();
    jsonParserInstance.setUrlResponse(jsonApiInstance.getHttpData());
    jsonParserInstance.fetchCracowId();
    jsonParserInstance.printCityId();
    jsonParserInstance.fetchWeatherData();
    jsonParserInstance.printWeatherData();
    if(!isLocationAdded)
    {
        qDebug() << "first time";
        db.addLocations(jsonParserInstance.getCityId(), weatherCity);
        isLocationAdded = true;

    }
    else
    {
        qDebug() << "next time";
    }
    //db.addLocations(jsonParserInstance.getCityId(), weatherCity);
    db.addWeather(jsonParserInstance.getCityId(),
                  jsonParserInstance.getWeatherDataHandler());
}
