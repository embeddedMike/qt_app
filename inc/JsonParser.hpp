#pragma once
#include "inc/WeatherData.hpp"
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
  int _cityId;
  WeatherData *_weatherData;
  std::map<int, std::string> _stationNameAndIds;
  std::map<int, std::string> _sensorIdWithParamCode;
  std::pair<std::string, float> _sensorRead;
  std::pair<std::string, std::string> _stationAirQuality;

public:
  JsonParser(std::string urlResponse) : _urlResponse(urlResponse) {
    _weatherData = new WeatherData();
  }
  ~JsonParser() { delete _weatherData; }
  WeatherData *getWeatherDataHandler();
  std::string getUrlResponse();
  void setUrlResponse(std::string urlResponse);
  void printStationNamesAndIds();
  void printSensorIdAndParamCode();
  void printSensorRead();
  void printStationAirQuality();
  void printCityId();
  void printWeatherData();
  void fetchStationNamesAndIds();
  void fetchSensorIdAndParamCode();
  void fetchSensorRead();
  void fetchStationAirQuality();
  void fetchCracowId();
  void fetchWeatherData();
};
