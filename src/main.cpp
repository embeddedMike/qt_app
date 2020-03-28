#include "curl/curl.h"
#include "inc/JsonApiAirQuality.hpp"
#include "inc/mainwindow.h"
#include "sqlite3.h"
#include "gtest/gtest.h"
#include <QApplication>
#include <QDebug>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlRecord>
#include <algorithm>
#include <nlohmann/json.hpp>
#include <vector>

class DbManager {
public:
  DbManager(const QString &path);
  bool addPerson(const QString &name);
  void printAllPersons();
  ~DbManager();

private:
  QSqlDatabase m_db;
};

static const QString path = "/Users/mike/qt_app/people.db";
const std::string
    url("http://api.gios.gov.pl/pjp-api/rest/aqindex/getIndex/52");
static size_t WriteCallback(void *contents, size_t size, size_t nmemb,
                            void *userp) {
  ((std::string *)userp)->append((char *)contents, size * nmemb);
  return size * nmemb;
}
int main(int argc, char *argv[]) {

  QApplication a(argc, argv);
  MainWindow w;
  w.show();

  JsonApiAirQuality instance(url);
  instance.initCurl();
  instance.configureCurl();
  instance.performCurl();
  instance.cleanupCurl();

  /*
    CURL *curl;
    long httpCode(0);
    // std::unique_ptr<std::string> httpData(new std::string());
    std::string httpData;
    curl = curl_easy_init();
    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &httpData);
    // curl_easy_setopt(curl, CURLOPT_IPRESOLVE, CURL_IPRESOLVE_V4);
    // curl_easy_setopt(curl, CURLOPT_TIMEOUT, 10);
    // curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
    // curl_easy_setopt(curl, CURLOPT_WRITEDATA, httpData.get());
    curl_easy_perform(curl);
    // curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &httpCode);
    // std::cout << httpCode << std::endl;
    std::cout << "HTTP data was:\n" << httpData << std::endl;
    curl_easy_cleanup(curl);
    // std::cout << "HTTP data was:\n" << *httpData.get() << std::endl;

    using json = nlohmann::json;
    // std::string s = *httpData.get();
    // std::cout << s << std::endl;
    json j_complete = json::parse(httpData);

    std::cout << j_complete["stCalcDate"] << std::endl;
  */
  sqlite3 *connection = nullptr;
  int result = sqlite3_open("/Users/mike/qt_app/people.db", &connection);
  if (SQLITE_OK != result) {
    std::cout << "Error" << std::endl;
    sqlite3_close(connection);
  }
  std::cout << "SQLITE_OK" << result << std::endl;

  DbManager db(path);
  QSqlQuery query;
  QString name = "abc";
  query.prepare("INSERT INTO people (name) VALUES (:name)");
  query.bindValue(":name", name);
  qDebug() << "addPersonn error: " << query.lastError();
  if (query.exec()) {
    qDebug() << "addPersonn errorrrr: " << query.lastError();
  }
  // db.addPerson("Aasd");
  // db.addPerson("Bas");
  // db.addPerson("Casd");
  db.printAllPersons();

  return a.exec();
}

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
