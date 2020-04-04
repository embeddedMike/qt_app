#include "../inc/JsonParser.hpp"

std::string JsonParser::getUrlResponse() { return _urlResponse; }

void JsonParser::setUrlResponse(std::string urlResponse) {
  _urlResponse = urlResponse;
}

std::string JsonParser::getStationName() {
  json j = json::parse(_urlResponse);
  std::cout << j[0]["addressStreet"] << std::endl;
  return j[0]["addressStreet"];
}
