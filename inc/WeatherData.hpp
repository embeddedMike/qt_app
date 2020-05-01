#pragma once
#include <QString>
class WeatherData {
public:
  QString timestamp;
  float temperature;
  float wind;
  int pressure;
  int humidity;
};
