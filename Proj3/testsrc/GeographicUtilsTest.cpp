#include <gtest/gtest.h>
#include "GeographicUtils.h"

TEST(GeographicUtils, SimpleTest){
    EXPECT_EQ(SGeographicUtils::RadiansToDegrees(SGeographicUtils::DegreesToRadians(90.0)),90.0);
    
    EXPECT_EQ(SGeographicUtils::Normalize180180(SGeographicUtils::Normalize360(-90.0)),-90.0);
    EXPECT_EQ(SGeographicUtils::HaversineDistanceInMiles({36.5,-121.7},{36.5,-121.71}),0.5555691415393712);
    EXPECT_EQ(SGeographicUtils::BearingToDirection(SGeographicUtils::CalculateBearing({36.5,-121.0}, {36.5,-121.5})),"W");
    EXPECT_EQ(SGeographicUtils::BearingToDirection(1.0),"N");
    EXPECT_EQ(SGeographicUtils::BearingToDirection(46.0),"NE");
    EXPECT_EQ(SGeographicUtils::BearingToDirection(93.0),"E");
    EXPECT_EQ(SGeographicUtils::BearingToDirection(134.0),"SE");
    EXPECT_EQ(SGeographicUtils::BearingToDirection(177.0),"S");
    EXPECT_EQ(SGeographicUtils::BearingToDirection(182.0),"S");
    EXPECT_EQ(SGeographicUtils::BearingToDirection(228.0),"SW");
    EXPECT_EQ(SGeographicUtils::BearingToDirection(270.0),"W");
    EXPECT_EQ(SGeographicUtils::BearingToDirection(314.0),"NW");
    EXPECT_EQ(SGeographicUtils::CalcualteExternalBisectorDirection({0.0,-1.0},{0.0,0.0},{0.0,1.0}),"S");
    EXPECT_EQ(SGeographicUtils::CalcualteExternalBisectorDirection({0.0,1.0},{0.0,0.0},{0.0,-1.0}),"S");
    EXPECT_EQ(SGeographicUtils::CalcualteExternalBisectorDirection({-1.0,0.0},{0.0,0.0},{1.0,0.0}),"E");
    EXPECT_EQ(SGeographicUtils::CalcualteExternalBisectorDirection({1.0,0.0},{0.0,0.0},{-1.0,0.0}),"E");
    EXPECT_EQ(SGeographicUtils::ConvertLLToDMS({36.5,-121.7}),"36d 30' 0\" N, 121d 42' 0\" W");
    CStreetMap::SLocation LowerLeft, UpperRight;
    std::vector<CStreetMap::SLocation> Locations{{0.0,0.0},{-5.0,4.0},{7.0,-2.0}};
    EXPECT_TRUE(SGeographicUtils::CalculateExtents(Locations,LowerLeft,UpperRight));
    EXPECT_EQ(LowerLeft,CStreetMap::SLocation(-5.0,-2.0));
    EXPECT_EQ(UpperRight,CStreetMap::SLocation(7.0,4.0));
    auto Locations2 = Locations;
    Locations2.push_back({8.9,11.0});
    EXPECT_EQ(SGeographicUtils::FilterLocations(Locations2,LowerLeft,UpperRight),Locations);

    
}

