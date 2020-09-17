#include <gtest/gtest.h>
#include "../inc/DbManager.hpp"

static const QString path = "/Users/mike/Projects/qt_app/json_data.db";

class DbManagerTest : public ::testing::Test {

    virtual void SetUp() {
        DbManager db(path);
    }

    virtual void TearDown() {

    }
};

TEST_F(DbManagerTest, someTest) {
    //DbManager db(path);
    QSqlQuery query;
    ASSERT_STREQ("","");
}
