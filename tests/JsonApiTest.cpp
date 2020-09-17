#include <gtest/gtest.h>
#include "../inc/JsonApi.hpp"

std::string urlAllStations = "http://api.gios.gov.pl/pjp-api/rest/station/findAll";

class JsonApiTest : public ::testing::Test {
    public:
};

TEST_F(JsonApiTest, someTest) {
    ASSERT_STREQ("","");
    JsonApi JsonApiUnitTest(urlAllStations);
    JsonApiUnitTest.initCurl();
    //JsonApiUnitTest.configureCurl();
    //JsonApiUnitTest.performCurl();
    //JsonApiUnitTest.cleanupCurl();
    ASSERT_STREQ("","");
}
