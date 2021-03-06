#pragma once
#include "inc/WeatherData.hpp"
#include "nlohmann/json.hpp"
#include <QDebug>
#include <QString>
#include <QDateTime>
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
  std::map<int, std::string> _sensorIdWithParamCodeBuffer;
  std::pair<std::string, float> _sensorRead;
  std::pair<std::string, std::string> _stationAirQuality;

public:
  JsonParser(std::string urlResponse) : _urlResponse(urlResponse) {
    _weatherData = new WeatherData();
  }
  ~JsonParser() { delete _weatherData; }
  WeatherData *getWeatherDataHandler();
  std::string getUrlResponse();
  std::map<int, std::string> getStationNameAndIds();
  std::map<int, std::string> getSensorIdWithParamCode();
  std::map<int, std::string> getSensorIdWithParamCodeBuffer();
  std::pair<std::string, float> getSensorRead();
  std::pair<std::string, std::string> getStationAirQuality();
  int getCityId();
  void clearSensorIdWithParamCodeBuffer();
  void setUrlResponse(std::string urlResponse);
  void printStationNamesAndIds();
  void printSensorIdAndParamCode();
  void printSensorIdAndParamCodeBuffer();
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
