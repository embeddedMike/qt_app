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
  bool addPerson(const QString &name);
  void printAllPersons();
  ~DbManager();

private:
  QSqlDatabase m_db;
};
