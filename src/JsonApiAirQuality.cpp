#include "../inc/JsonApiAirQuality.hpp"

void JsonApiAirQuality::initCurl() { curl = curl_easy_init(); }
size_t JsonApiAirQuality::WriteCallback(void *contents, size_t size,
                                        size_t nmemb, void *userp) {
  ((std::string *)userp)->append((char *)contents, size * nmemb);
  return size * nmemb;
}
void JsonApiAirQuality::configureCurl() {
  curl_easy_setopt(curl, CURLOPT_URL, _apiUrl.c_str());
  curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
  curl_easy_setopt(curl, CURLOPT_WRITEDATA, &_httpData);
}
bool JsonApiAirQuality::performCurl() {
  curl_easy_perform(curl);
  curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &_httpCode);
  std::cout << _httpCode << std::endl;
  std::cout << "HTTP data was:\n" << _httpData << std::endl;
  return true;
}
bool JsonApiAirQuality::cleanupCurl() {
  curl_easy_cleanup(curl);
  return true;
}
