#pragma once
#include "WeatherData.hpp"
#include <QDebug>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlRecord>
class DbManager {
public:
  DbManager(const QString &path);
  bool createTable(const QString &table);
  bool addStationName(QString &stationName);
  bool addLocations(int stationId, QString &stationName);
  bool addSensors(int sensorId, int stationId, QString &paramCode);
  bool addReadings(int sensorId, QString &timestampValue, float readingValue);
  bool addAirQuality(int stationId, QString &timestampValue,
                     QString &qualityIndex);
  bool addWeather(int locationId, WeatherData *weatherData);
  void printAllLocations();
  void printAllSensors();
  bool removeAllLocations();
  bool removeAllSensors();
  ~DbManager();

private:
  QSqlDatabase m_db;
};
