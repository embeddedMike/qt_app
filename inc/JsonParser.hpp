#pragma once
#include "nlohmann/json.hpp"
#include <QString>
#include <iostream>
#include <map>
#include <regex>
#include <string>
using json = nlohmann::json;

class JsonParser {
private:
  std::string _urlResponse;
  std::map<int, std::string> _stationNameAndIds;

public:
  JsonParser() = delete;
  JsonParser(std::string urlResponse) : _urlResponse(urlResponse) {}
  std::string getUrlResponse();
  void setUrlResponse(std::string urlResponse);
  void printStationNamesAndIds();
  void getStationNamesAndIds();
};
