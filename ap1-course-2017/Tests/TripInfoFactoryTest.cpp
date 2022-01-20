#include <gtest/gtest.h>
#include "../src/Util/Factories/TripInfoFactory.h"

class TripInfoFactoryTest : public testing::Test {
public:
    TripInfoFactoryTest() {}

protected:
    TripInfoFactory t;
};
/**
 * test DriverFactory_createDriver_test make shore that createTrip
 * and sets works good.
 */
TEST_F(TripInfoFactoryTest, TripInfoFactory_createTrip_test) {
    t=TripInfoFactory();
    for (int num = -5; num < 0xFF; num++) {
        //if number smaller then 0 expect failre.
        if(num<0) {
            //cant set id<0
            EXPECT_ANY_THROW(t.setID(num));
            t.setEndX(num+1);
            t.setEndY(num);
            t.setStartX(num+2);
            t.setStartY(num-10);
            //cant set Passengers<0
            EXPECT_ANY_THROW(t.setNumOfPassengers(num));
            //cant set Tariff<0
            EXPECT_ANY_THROW(t.setTariff(num));
            //cant create atrip with end(x/y)/start(x/y)<0.
            EXPECT_TRUE(t.createTrip()==NULL);
        }
        else{
            //update TripInfoFactory.
            t.setID(num);
            t.setEndX(num+1);
            t.setEndY(num+5);
            t.setStartX(num+3);
            t.setStartY(num+2);
            t.setNumOfPassengers(num+7);
            t.setTariff(num);
            //create trip by factory.
            TripInfo* trip = t.createTrip();
            EXPECT_TRUE(trip!=NULL);
            EXPECT_TRUE(trip->getEndPoint().getX()==num+1);
            EXPECT_TRUE(trip->getEndPoint().getY()==num+5);
            EXPECT_TRUE(trip->getStartPoint().getX()==num+3);
            EXPECT_TRUE(trip->getStartPoint().getY()==num+2);
            EXPECT_TRUE(trip->getNumOfPassengers()==(unsigned int)num+7);
            EXPECT_TRUE(trip->getTotalMeters()==0);
            EXPECT_TRUE(trip->getTariff()==num);
            delete trip;
        }
    }
}