#include <gtest/gtest.h>
#include "../src/Core/Point.h"
#include "../src/Core/Trip/TripInfo.h"
#include "../src/Core/Entities/Driver.h"
#include "../src/Server/TaxiCenter/TaxiCenter.h"

using namespace std;

class TaxiCenterTest : public testing::Test {
public:
    TaxiCenterTest() {}

protected:
    TripInfo t;
};

TEST_F(TaxiCenterTest, TaxiCenter_addDriver_Test) {
    TaxiCenter center(new Map(1,1));
    for(unsigned int i=0;i<10;i++){
        Driver *d = new Driver(i, 0, SINGLE, 0, 0);
        EXPECT_FALSE(center.addDriver(NULL));
        EXPECT_TRUE(center.addDriver(d));
        EXPECT_FALSE(center.addDriver(d));
        EXPECT_TRUE(center.getEmployee(0));
    }
}

TEST_F(TaxiCenterTest, TaxiCenter_addTaxi_Test) {
    // TODO: update the test so it will support the new change of the taxiCenter.
    //check that addTaxi is working as expected
    /*
    TaxiCenter center(Map(1,1));
    for(unsigned int i=0;i<10;i++){
        //create taxi.
        Taxi *t = new Taxi(Point(), i, 0, HONDA, WHITE);
        //check that cant add NULL taxi.
        EXPECT_FALSE(center.addTaxi(NULL));
        //check that can add new taxi.
        EXPECT_TRUE(center.addTaxi(t));
        //check that cant add the same taxi.
        EXPECT_FALSE(center.addTaxi(t));
        //check that its found in the center.
        EXPECT_TRUE(center.getTaxi(i) != NULL) << "center.getTaxi(i) at i = " << i;
    }
     */
}