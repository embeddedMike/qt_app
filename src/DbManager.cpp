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
bool DbManager::addPerson(const QString &name) {
  bool success = false;
  QSqlQuery query;

  if (m_db.open()) {
    qDebug() << "Database is open";
  }
  query.prepare("INSERT INTO people (name) VALUES (:name)");
  // query.exec("INSERT INTO people (name) VALUES ('Thad Beaumont')");
  query.bindValue(":name", name);
  if (query.exec()) {
    success = true;
  } else {
    qDebug() << "error: " << query.lastError();
  }
  return success;
}

void DbManager::printAllPersons() {
  qDebug() << "Values in db";
  QSqlQuery query("SELECT * FROM locations");
  int idName = query.record().indexOf("name");
  while (query.next()) {
    QString name = query.value(idName).toString();
    qDebug() << "===" << name;
  }
}

DbManager::~DbManager() {
  if (m_db.isOpen()) {
    m_db.close();
  }
}
