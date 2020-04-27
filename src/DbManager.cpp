#include "../inc/DbManager.hpp"

DbManager::DbManager(const QString &path) {
  m_db = QSqlDatabase::addDatabase("QSQLITE");
  m_db.setDatabaseName(path);
  QSqlQuery query(m_db);
  query.exec("PRAGMA foreign_keys = ON;");
  if (!m_db.open()) {
    qDebug() << "Error: connection with database fail";
  } else {
    qDebug() << "Database: connection ok";
  }
}
bool DbManager::createTable(const QString &table) {
  bool success = false;

  QSqlQuery query;
  query.prepare(table);

  if (!query.exec()) {
    qDebug() << "Couldn't create the table";
    success = false;
  }

  return success;
}
bool DbManager::addStationName(QString &stationName) {
  bool success = false;
  QSqlQuery query;

  if (m_db.open()) {
    qDebug() << "Database is open";
  }
  query.prepare("INSERT INTO locations (name) VALUES (:stationName)");
  query.bindValue(":stationName", stationName);
  if (query.exec()) {
    success = true;
  } else {
    qDebug() << "error: " << query.lastError();
  }
  return success;
}

bool DbManager::addLocations(int stationId, QString &stationName) {
  bool success = false;
  QSqlQuery query;

  if (m_db.open()) {
    qDebug() << "Database is open";
  }
  query.prepare("INSERT INTO locations (station_id, name) VALUES (?, ?);");
  query.addBindValue(stationId);
  query.addBindValue(stationName);
  if (query.exec()) {
    success = true;
  } else {
    qDebug() << "error: " << query.lastError();
  }
  return success;
}
bool DbManager::addSensors(int sensorId, int stationId, QString &paramCode) {
  bool success = false;
  QSqlQuery query;

  if (m_db.open()) {
    qDebug() << "Database is open";
  }
  query.prepare("INSERT INTO sensors (sensor_id, station_id, param_code) "
                "VALUES (?, ?, ?);");
  query.addBindValue(sensorId);
  query.addBindValue(stationId);
  query.addBindValue(paramCode);
  if (query.exec()) {
    success = true;
  } else {
    qDebug() << "error: " << query.lastError();
  }
  return success;
}
bool DbManager::addReadings(int sensorId, QString &timestampValue,
                            float readingValue) {
  bool success = false;
  QSqlQuery query;

  if (m_db.open()) {
    qDebug() << "Database is open";
  }
  query.prepare("INSERT INTO readings (sensor_id, timestamp, value) "
                "VALUES (?, ?, ?);");
  query.addBindValue(sensorId);
  query.addBindValue(timestampValue);
  query.addBindValue(readingValue);
  if (query.exec()) {
    success = true;
  } else {
    qDebug() << "error: " << query.lastError();
  }
  return success;
}
bool DbManager::addAirQuality(int stationId, QString &timestampValue,
                              QString &qualityIndex) {
  bool success = false;
  QSqlQuery query;

  if (m_db.open()) {
    qDebug() << "Database is open";
  }
  query.prepare("INSERT INTO airquality (station_id, timestamp, qualityIndex) "
                "VALUES (?, ?, ?);");
  query.addBindValue(stationId);
  query.addBindValue(timestampValue);
  query.addBindValue(qualityIndex);
  if (query.exec()) {
    success = true;
  } else {
    qDebug() << "error: " << query.lastError();
  }
  return success;
}
bool DbManager::addWeather(int locationId, WeatherData *weatherData) {
  bool success = false;
  QSqlQuery query;

  if (m_db.open()) {
    qDebug() << "Database is open";
  }
  query.prepare("INSERT INTO locations (location_id, timestamp, temp, "
                "pressure, humidity, wind) "
                "VALUES (?, ?, ?, ?, ?, ?);");
  query.addBindValue(locationId);
  query.addBindValue(weatherData->timestamp);
  query.addBindValue(weatherData->temperature);
  query.addBindValue(weatherData->pressure);
  query.addBindValue(weatherData->humidity);
  query.addBindValue(weatherData->wind);
  if (query.exec()) {
    success = true;
  } else {
    qDebug() << "error: " << query.lastError();
  }
  return success;
}

void DbManager::printAllLocations() {
  qDebug() << "Values in db";
  QSqlQuery query("SELECT * FROM locations");
  int idName = query.record().indexOf("name");
  while (query.next()) {
    QString name = query.value(idName).toString();
    qDebug() << "===" << name;
  }
}

void DbManager::printAllSensors() {
  qDebug() << "Values in db";
  QSqlQuery query("SELECT * FROM sensors");
  int idName = query.record().indexOf("name");
  while (query.next()) {
    QString name = query.value(idName).toString();
    qDebug() << "===" << name;
  }
}

bool DbManager::removeAllLocations() {
  bool success = false;

  QSqlQuery removeQuery;
  removeQuery.prepare("DELETE FROM locations");

  if (removeQuery.exec()) {
    success = true;
  } else {
    qDebug() << "remove all locations failed: " << removeQuery.lastError();
  }

  return success;
}

bool DbManager::removeAllSensors() {
  bool success = false;

  QSqlQuery removeQuery;
  removeQuery.prepare("DELETE FROM sensors");

  if (removeQuery.exec()) {
    success = true;
  } else {
    qDebug() << "remove all sensors failed: " << removeQuery.lastError();
  }
  return success;
}

DbManager::~DbManager() {
  if (m_db.isOpen()) {
    m_db.close();
  }
}
