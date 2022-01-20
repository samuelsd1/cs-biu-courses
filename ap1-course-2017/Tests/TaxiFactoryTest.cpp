#include <gtest/gtest.h>
#include "../src/Util/Factories/TaxiFactory.h"

class TaxiFactoryTest : public testing::Test {
public:
    TaxiFactoryTest() {}

protected:
    TaxiFactory t;
};
/**
 * test TaxiFactory_createTaxi_test make shore that createTaxi
 * and sets works good.
 */
TEST_F(TaxiFactoryTest, TaxiFactory_createTaxi_test) {
    t = TaxiFactory();
    Taxi* taxi = NULL;
    for (short num = -5; num < 0xFF; num++) {
        //if num <0 expect flier in update taxi.
        if(num<0){
            //id less then 0.
            EXPECT_ANY_THROW(t.setID(num));
            //type is not 1/2.
            EXPECT_ANY_THROW(t.setType(num));
            //color is not found.
            EXPECT_ANY_THROW(t.setColor('O'));
            //manufacturer is not found.
            EXPECT_ANY_THROW(t.setManufacturer('l'));
            //new word is undefined.
            //taxi cant be created.
            EXPECT_ANY_THROW(taxi=t.createTaxi());
        }
        else{
            //update TaxiFactory and create taxi.
            t.setID(num);
            //set tariff 1/2.
            t.setType((short)(num%2+1));
            t.setColor('R');
            t.setManufacturer('H');
            //create taxi.
            taxi=t.createTaxi();
            //check everything update ok.
            EXPECT_TRUE(taxi!=NULL);
            EXPECT_TRUE(taxi->getID()==(unsigned short)num);
            EXPECT_TRUE(taxi->getTariff()==(unsigned short)num%2+1);
            EXPECT_TRUE(taxi->getColor()==RED);
            EXPECT_TRUE(taxi->getManufacturer()==HONDA);
        }
        if(taxi!=NULL)
            delete taxi;
    }

}