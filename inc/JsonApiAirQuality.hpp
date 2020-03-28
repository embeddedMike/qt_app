#pragma once
#include "curl/curl.h"
#include <iostream>
#include <string>

class JsonApiAirQuality {
private:
  long _httpCode;
  const std::string _apiUrl;
  std::string _httpData;
  CURL *curl;

public:
  JsonApiAirQuality() = delete;
  JsonApiAirQuality(const std::string apiUrl) : _apiUrl(apiUrl) {}
  void initCurl();
  static size_t WriteCallback(void *contents, size_t size, size_t nmemb,
                              void *userp);
  void configureCurl();
  bool performCurl();
  bool cleanupCurl();
};