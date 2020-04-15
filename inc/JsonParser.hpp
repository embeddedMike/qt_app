#pragma once
#include "nlohmann/json.hpp"
#include <QString>
#include <iostream>
#include <map>
#include <regex>
#include <set>
#include <string>
using json = nlohmann::json;

class JsonParser {
private:
  std::string _urlResponse;
  std::map<int, std::string> _stationNameAndIds;
  std::map<int, std::string> _sensorIdWithParamCode;
  std::pair<std::string, float> _sensorRead;

public:
  JsonParser() = delete;
  JsonParser(std::string urlResponse) : _urlResponse(urlResponse) {}
  std::string getUrlResponse();
  void setUrlResponse(std::string urlResponse);
  void printStationNamesAndIds();
  void printSensorIdAndParamCode();
  void printSensorRead();
  void getStationNamesAndIds();
  void getSensorIdAndParamCode();
  void getSensorRead();
};
