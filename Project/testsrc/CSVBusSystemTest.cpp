#include <gtest/gtest.h>
#include "CSVBusSystem.h"
#include "StringDataSource.h"
#include "DSVReader.h"

TEST(CSVBusSystem, SimpleFiles){
    auto StopDataSource = std::make_shared< CStringDataSource >("stop_id,node_id\n"
                                                                "1,123\n"
                                                                "2,124");
    auto StopReader = std::make_shared< CDSVReader >(StopDataSource, ',');


    auto RouteDataSource = std::make_shared< CStringDataSource >("route,stop_id\n"
                                                                 "A,1\n"
                                                                 "A,2");
    auto RouteReader = std::make_shared< CDSVReader >(RouteDataSource, ',');
    

    CCSVBusSystem BusSystem(StopReader, RouteReader);
    

    EXPECT_EQ(BusSystem.StopCount(), 2);
    

    auto StopObj = BusSystem.StopByIndex(0);
    EXPECT_NE(StopObj, nullptr);
    StopObj = BusSystem.StopByIndex(1);
    EXPECT_NE(StopObj, nullptr);
    EXPECT_EQ(BusSystem.StopByIndex(2), nullptr);


    StopObj = BusSystem.StopByID(1);
    ASSERT_NE(StopObj, nullptr);
    EXPECT_EQ(StopObj->ID(), 1);
    EXPECT_EQ(StopObj->NodeID(), 123);


    StopObj = BusSystem.StopByID(2); 
    ASSERT_NE(StopObj, nullptr);
    EXPECT_EQ(StopObj->ID(), 2);
    EXPECT_EQ(StopObj->NodeID(), 124);

    EXPECT_EQ(BusSystem.RouteCount(), 1);


    auto RouteObj = BusSystem.RouteByName("A");
    ASSERT_NE(RouteObj, nullptr);
    EXPECT_EQ(RouteObj->Name(), "A");
    EXPECT_EQ(RouteObj->StopCount(), 2);

    EXPECT_EQ(RouteObj->GetStopID(0), 1);
    EXPECT_EQ(RouteObj->GetStopID(1), 2);
    

    EXPECT_EQ(BusSystem.RouteByName("B"), nullptr);

    EXPECT_EQ(RouteObj->GetStopID(2), CBusSystem::InvalidStopID);
}

TEST(CSVBusSystem, EmptyFiles){
    auto EmptyStopDS = std::make_shared< CStringDataSource >("stop_id,node_id\n");
    auto EmptyRouteDS = std::make_shared< CStringDataSource >("route,stop_id\n");
    
    CCSVBusSystem EmptySystem(std::make_shared<CDSVReader>(EmptyStopDS, ','), 
                              std::make_shared<CDSVReader>(EmptyRouteDS, ','));
    
    EXPECT_EQ(EmptySystem.StopCount(), 0);
    EXPECT_EQ(EmptySystem.RouteCount(), 0);
}