#include <gtest/gtest.h>
#include "../src/Core/Entities/Driver.h"
#include "../src/Util/Factories/DriverFactory.h"

class DriverFactoryTest : public testing::Test {
public:
    DriverFactoryTest() {}

protected:
    DriverFactory d;
};
/**
 * test DriverFactory_createDriver_test make shore that createDriver
 * and sets works good.
 */
TEST_F(DriverFactoryTest, DriverFactory_createDriver_test) {
    for (short num = -5; num < 0xFF; num++) {
        d = DriverFactory();
        //if num <0 expect failure in update driver.
        if (num < 0) {
            //expect exception num<0
            EXPECT_ANY_THROW(d.setID(num));
            //expect exception num+1<=0
            EXPECT_ANY_THROW(d.setAge(num + 1));
            EXPECT_ANY_THROW(d.setExperienceYears(num));
            //expect exception unknown char.
            EXPECT_ANY_THROW(d.setMaritalStatus('f'));
        } else {
            //create driver and check updates.
            d.setID(num);
            d.setAge((short) (num + 1));
            d.setExperienceYears((short)num+2);
            d.setMaritalStatus('M');
            Driver *driver = d.createDriver();
            EXPECT_TRUE(driver->getMaritalStatus() == MARRIED);
            EXPECT_TRUE(driver != NULL);
            EXPECT_TRUE(driver->getId() == (unsigned short)num);
            EXPECT_TRUE(driver->getAge() == (unsigned short)num + 1);
            EXPECT_TRUE(driver->getExperienceYears() == (unsigned short)num+2);
            EXPECT_TRUE(driver->getAvgSatisfaction() == 0);
            delete driver;
        }
    }
}