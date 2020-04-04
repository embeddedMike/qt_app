#pragma once
#include "curl/curl.h"
#include <iostream>
#include <string>

class JsonApi {
private:
  long _httpCode;
  const std::string _apiUrl;
  std::string _httpData;
  CURL *curl;

public:
  JsonApi() = delete;
  JsonApi(const std::string apiUrl) : _apiUrl(apiUrl) {}
  void initCurl();
  std::string getHttpData() { return _httpData; }
  static size_t WriteCallback(void *contents, size_t size, size_t nmemb,
                              void *userp);
  void configureCurl();
  bool performCurl();
  bool cleanupCurl();
};
