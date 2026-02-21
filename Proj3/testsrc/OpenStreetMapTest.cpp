#include <gtest/gtest.h>
#include "OpenStreetMap.h"
#include "StringDataSource.h"

TEST(OSMTest, SimpleFiles){
    auto OSMDataSource = std::make_shared< CStringDataSource >("<?xml version='1.0' encoding='UTF-8'?>\n"
                                                                "<osm version=\"0.6\" generator=\"osmconvert 0.8.5\">\n"
	                                                            "  <node id=\"1\" lat=\"38.5\" lon=\"-121.7\"/>\n"
	                                                            "  <node id=\"2\" lat=\"38.5\" lon=\"-121.8\"/>\n"
                                                                "  <way id=\"100\">\n"
		                                                        "    <nd ref=\"258592863\"/>\n"
		                                                        "    <nd ref=\"4399281377\"/>\n"
                                                                "  </way>\n"
                                                                "</osm>"
                                                                );
    auto OSMReader = std::make_shared< CXMLReader >(OSMDataSource);
    COpenStreetMap OpenStreetMap(OSMReader);

    EXPECT_EQ(OpenStreetMap.NodeCount(),2);
    EXPECT_EQ(OpenStreetMap.WayCount(),1);
    EXPECT_NE(OpenStreetMap.NodeByIndex(0),nullptr);
    EXPECT_NE(OpenStreetMap.NodeByIndex(1),nullptr);
    EXPECT_EQ(OpenStreetMap.NodeByIndex(2),nullptr);
    EXPECT_NE(OpenStreetMap.WayByIndex(0),nullptr);
    EXPECT_EQ(OpenStreetMap.WayByIndex(1),nullptr);
    auto TempNode = OpenStreetMap.NodeByIndex(0);
    ASSERT_NE(TempNode,nullptr);
    EXPECT_EQ(TempNode, OpenStreetMap.NodeByID(TempNode->ID()));
    EXPECT_EQ(TempNode->ID(),1);
    EXPECT_EQ(TempNode->Location(),std::make_pair(38.5,-121.7));

}
