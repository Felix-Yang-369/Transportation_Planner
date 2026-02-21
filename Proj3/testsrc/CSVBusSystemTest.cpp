#include <gtest/gtest.h>
#include "CSVBusSystem.h"
#include "StringDataSource.h"

TEST(CSVBusSystem, SimpleFiles){
    auto StopDataSource = std::make_shared< CStringDataSource >("stop_id,node_id\n"
                                                                "1,123\n"
                                                                "2,124");
    auto StopReader = std::make_shared< CDSVReader >(StopDataSource,',');
    auto RouteDataSource = std::make_shared< CStringDataSource >("route,stop_id\n"
                                                                "A,1\n"
                                                                "A,2");
    auto RouteReader = std::make_shared< CDSVReader >(RouteDataSource,',');
    
    CCSVBusSystem BusSystem(StopReader,RouteReader);
    
    EXPECT_EQ(BusSystem.StopCount(),2);
    auto StopObj = BusSystem.StopByIndex(0);
    EXPECT_NE(StopObj,nullptr);
    StopObj = BusSystem.StopByIndex(1);
    EXPECT_NE(StopObj,nullptr);
    StopObj = BusSystem.StopByID(1);
    ASSERT_NE(StopObj,nullptr);
    EXPECT_EQ(StopObj->ID(),1);
    EXPECT_EQ(StopObj->NodeID(),123);
    StopObj = BusSystem.StopByID(1);
    ASSERT_NE(StopObj,nullptr);
    EXPECT_EQ(StopObj->ID(),2);
    EXPECT_EQ(StopObj->NodeID(),124);

}
