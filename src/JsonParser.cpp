#include "../inc/JsonParser.hpp"

std::string JsonParser::getUrlResponse() { return _urlResponse; }

WeatherData *JsonParser::getWeatherDataHandler() { return _weatherData; }

std::map<int, std::string> JsonParser::getStationNameAndIds() {
  return _stationNameAndIds;
}

std::map<int, std::string> JsonParser::getSensorIdWithParamCode() {
  return _sensorIdWithParamCode;
}

std::map<int, std::string> JsonParser::getSensorIdWithParamCodeBuffer() {
  return _sensorIdWithParamCodeBuffer;
}

std::pair<std::string, float> JsonParser::getSensorRead() {
  return _sensorRead;
}

std::pair<std::string, std::string> JsonParser::getStationAirQuality() {
  return _stationAirQuality;
}

int JsonParser::getCityId() { return _cityId; }

void JsonParser::clearSensorIdWithParamCodeBuffer() {
  _sensorIdWithParamCodeBuffer.clear();
}

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

void JsonParser::printSensorIdAndParamCodeBuffer() {
  for (const auto &[sensorId, paramCode] : _sensorIdWithParamCodeBuffer) {
    std::cout << "Buffer:" << sensorId << " | " << paramCode << std::endl;
  }
}

void JsonParser::printSensorRead() {
  std::cout << _sensorRead.first << " | " << _sensorRead.second << std::endl;
}

void JsonParser::printStationAirQuality() {
  std::cout << _stationAirQuality.first << " | " << _stationAirQuality.second
            << std::endl;
}

void JsonParser::printCityId() { std::cout << _cityId << std::endl; }

void JsonParser::printWeatherData() {
  std::cout << _weatherData->temperature << " | " << _weatherData->pressure
            << " | " << _weatherData->humidity << " | " << _weatherData->wind
            << std::endl;
}

void JsonParser::fetchStationNamesAndIds() {
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

void JsonParser::fetchSensorIdAndParamCode() {
  json j = json::parse(_urlResponse);
  for (json::iterator it = j.begin(); it != j.end(); ++it) {
    std::pair sensorIdAndParamCode =
        std::make_pair((*it)["id"], (*it)["param"]["paramCode"]);
    _sensorIdWithParamCode.insert(sensorIdAndParamCode);
    _sensorIdWithParamCodeBuffer.insert(sensorIdAndParamCode);
  }
}

void JsonParser::fetchSensorRead() {
  json j = json::parse(_urlResponse);
  if (j["values"][0]["value"].is_null()) {
    _sensorRead = std::make_pair(j["values"][0]["date"], 0);
  } else {
    _sensorRead =
        std::make_pair(j["values"][0]["date"], j["values"][0]["value"]);
  }
}

void JsonParser::fetchStationAirQuality() {
  json j = json::parse(_urlResponse);
  _stationAirQuality =
      std::make_pair(j["stCalcDate"], j["stIndexLevel"]["indexLevelName"]);
}

void JsonParser::fetchCracowId() {
  json j = json::parse(_urlResponse);
  _cityId = j["id"];
}

void JsonParser::fetchWeatherData() {
  json j = json::parse(_urlResponse);
  _weatherData->timestamp = "6:00";
  _weatherData->temperature = j["main"]["temp"];
  _weatherData->pressure = j["main"]["pressure"];
  _weatherData->humidity = j["main"]["humidity"];
  _weatherData->wind = j["wind"]["speed"];
}
