#pragma once
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
  void printAllLocations();
  bool removeAllLocations();
  ~DbManager();

private:
  QSqlDatabase m_db;
};
