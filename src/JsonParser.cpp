#include "../inc/JsonParser.hpp"

std::string JsonParser::getUrlResponse() { return _urlResponse; }

void JsonParser::setUrlResponse(std::string urlResponse) {
  _urlResponse = urlResponse;
}

void JsonParser::printStationNamesAndIds() {
  for (const auto &[id, station] : _stationNameAndIds) {
    std::cout << id << " | " << station << std::endl;
  }
}
void JsonParser::printSensorIdAndParamCode() {
  for (const auto &[sensorId, paramCode] : _sensorIdWithParamCode) {
    std::cout << sensorId << " | " << paramCode << std::endl;
  }
}

void JsonParser::getStationNamesAndIds() {
  json j = json::parse(_urlResponse);
  std::regex e("(Kraków)(.*)");
  for (json::iterator it = j.begin(); it != j.end(); ++it) {
    std::string s = (*it)["stationName"];
    if (std::regex_match(s, e)) {
      std::pair<int, std::string> stationNameAndId =
          std::make_pair((*it)["id"], (*it)["stationName"]);
      _stationNameAndIds.insert(stationNameAndId);
    }
  }
}

void JsonParser::getSensorIdAndParamCode() {
  json j = json::parse(_urlResponse);
  for (json::iterator it = j.begin(); it != j.end(); ++it) {
    std::pair sensorIdAndParamCode =
        std::make_pair((*it)["id"], (*it)["param"]["paramCode"]);
    _sensorIdWithParamCode.insert(sensorIdAndParamCode);
  }
}
