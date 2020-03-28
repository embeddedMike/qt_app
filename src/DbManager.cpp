#include "../inc/DbManager.hpp"

DbManager::DbManager(const QString &path) {
  m_db = QSqlDatabase::addDatabase("QSQLITE");
  m_db.setDatabaseName(path);

  if (!m_db.open()) {
    qDebug() << "Error: connection with database fail";
  } else {
    qDebug() << "Database: connection ok";
  }
}

bool DbManager::addPerson(const QString &name) {
  bool success = false;
  QSqlQuery query;

  if (m_db.open()) {
    qDebug() << "tescik";
  }
  qDebug() << "addPerson success???? 1";
  query.prepare("INSERT INTO people (name) VALUES (:name)");
  // query.exec("INSERT INTO people (name) VALUES ('Thad Beaumont')");

  qDebug() << "addPerson success??? 2";
  query.bindValue(":name", name);
  qDebug() << "addPerson success??? 3";
  if (query.exec()) {
    qDebug() << "addPerson success??? ";
    success = true;
  } else {
    qDebug() << "addPerson error: " << query.lastError();
  }
  return success;
}

void DbManager::printAllPersons() {
  qDebug() << "Persons in db";
  QSqlQuery query("SELECT * FROM people");
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
