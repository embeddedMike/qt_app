#pragma once
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlRecord>

class DbManager {
public:
  DbManager(const QString &path);
  bool addPerson(const QString &name);
  void printAllPersons();
  ~DbManager();

private:
  QSqlDatabase m_db;
};
