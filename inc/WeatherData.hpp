#pragma once
#include <string>
class WeatherData {
public:
  std::string timestamp;
  float temperature;
  float wind;
  int pressure;
  int humidity;
};
