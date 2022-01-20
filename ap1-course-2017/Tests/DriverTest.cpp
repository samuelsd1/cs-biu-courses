#include <gtest/gtest.h>
#include "../src/Core/Entities/Driver.h"

class DriverTest : public testing::Test {
public:
    DriverTest() {}

protected:
    Driver d;
};

TEST_F(DriverTest, Driver_getId_test) {
    for (int id = 0; id < 0xFFFF; id++) {
        d = Driver(id, 0, SINGLE, 0, 0);
        ASSERT_EQ(d.getId(), id) << "Driver getId differs at id = " << id;
    }
}

TEST_F(DriverTest, Driver_getAge_test) {
    for (unsigned short i = 0; i < 0xFF; i++) {
        d = Driver(0, i, SINGLE, 0, 0);
        ASSERT_EQ(d.getAge(), i) << "Driver getAge differs at i = " << i;
    }
}

TEST_F(DriverTest, Driver_setAge_test) {
    d = Driver();
    for (unsigned short i = 0; i < 0xFF; i++) {
        d.setAge(i);
        ASSERT_EQ(d.getAge(), i) << "Driver setAge differs at i = " << i;
    }
}

TEST_F(DriverTest, Driver_getMaritalStatus_test) {
    d = Driver(0, 0, MARRIED, 0, 0);
    ASSERT_EQ(d.getMaritalStatus(), MARRIED)
                                << "Driver getMaritalStatus differs at MARRIED status";

    d = Driver(0, 0, DIVORCED, 0, 0);
    ASSERT_EQ(d.getMaritalStatus(), DIVORCED)
                                << "Driver getMaritalStatus differs at DIVORCED status";

    d = Driver(0, 0, SINGLE, 0, 0);
    ASSERT_EQ(d.getMaritalStatus(), SINGLE)
                                << "Driver getMaritalStatus differs at SINGLE status";

    d = Driver(0, 0, WIDOWED, 0, 0);
    ASSERT_EQ(d.getMaritalStatus(), WIDOWED)
                                << "Driver getMaritalStatus differs at WIDOWED status";
}

TEST_F(DriverTest, Driver_setMaritalStatus_Test) {
    d = Driver(0, 0, SINGLE, 0, 0);

    d.setMaritalStatus(MARRIED);
    ASSERT_EQ(d.getMaritalStatus(), MARRIED)
                                << "Driver setMaritalStatus differs at MARRIED = " << MARRIED;

    d.setMaritalStatus(DIVORCED);
    ASSERT_EQ(d.getMaritalStatus(), DIVORCED)
                                << "Driver setMaritalStatus differs at DIVORCED = " << DIVORCED;

    d.setMaritalStatus(SINGLE);
    ASSERT_EQ(d.getMaritalStatus(), SINGLE)
                                << "Driver setMaritalStatus differs at SINGLE = " << SINGLE;

    d.setMaritalStatus(WIDOWED);
    ASSERT_EQ(d.getMaritalStatus(), WIDOWED)
                                << "Driver setMaritalStatus differs at WIDOWED = " << WIDOWED;

}

TEST_F(DriverTest, Driver_getExperieanceYears_Test) {
    for (unsigned short i = 0; i < 0xFF; i++) {
        d = Driver(0, 0, SINGLE, i, 0);
        ASSERT_EQ(d.getExperienceYears(), i) << "Driver getExperieanceYears differs at i = " << i;
    }
}

TEST_F(DriverTest, Driver_setExperieanceYears_test) {
    d = Driver(0, 0, SINGLE, 0, 0);
    for (unsigned short i = 0; i < 0xFF; i++) {
        d.setExperienceYears(i);
        ASSERT_EQ(d.getExperienceYears(), i) << "Driver setExperieanceYears differs at i = " << i;
    }
}

TEST_F(DriverTest, Driver_getAvgStatisFaction_test) {
    for (double i = 0; i <= 5; i += 0.01) {
        Driver d = Driver(0, 0, SINGLE, 0, i);
        ASSERT_EQ(d.getAvgSatisfaction(), i)
                                    << "Driver getAvgStatisFaction differs at value = " << i;
    }
}

TEST_F(DriverTest, Driver_Vehicle_test) {
    d = Driver();
    ASSERT_TRUE(NULL == d.getVehicle());
    Taxi *v = new Taxi(Point(), 0, 0, 0, HONDA, WHITE);
    EXPECT_TRUE(d.setVehicle(v));
    EXPECT_EQ(d.getVehicle(), v);
    EXPECT_FALSE(d.setVehicle(v));
}

TEST_F(DriverTest, Driver_Trip_Test) {
    d = Driver();
    ASSERT_FALSE(d.isOccupied());
    ASSERT_FALSE(d.endTrip());
    Trip *t = new Trip();
    EXPECT_TRUE(d.assignTrip(t));
    EXPECT_TRUE(d.isOccupied());
    EXPECT_FALSE(d.assignTrip(t));
    EXPECT_TRUE(d.endTrip());
    ASSERT_FALSE(d.isOccupied());
}