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
  // query.exec("INSERT INTO people (name) VALUES ('Thad Beaumont')");
  query.bindValue(":stationName", stationName);
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

bool DbManager::removeAllLocations() {
  bool success = false;

  QSqlQuery removeQuery;
  removeQuery.prepare("DELETE FROM locations");

  if (removeQuery.exec()) {
    success = true;
  } else {
    qDebug() << "remove all persons failed: " << removeQuery.lastError();
  }

  return success;
}

DbManager::~DbManager() {
  if (m_db.isOpen()) {
    m_db.close();
  }
}
