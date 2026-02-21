#include <gtest/gtest.h>
#include "XMLReader.h"
#include "StringDataSource.h"

TEST(XMLReader, SimpleTest){
    std::string XMLString = "<tag></tag>";
    std::shared_ptr<CStringDataSource> DataSource = std::make_shared<CStringDataSource>(XMLString);
    CXMLReader Reader(DataSource);
    SXMLEntity TempEntity;
    EXPECT_TRUE(Reader.ReadEntity(TempEntity));
    EXPECT_EQ(TempEntity.DType,SXMLEntity::EType::StartElement);
    EXPECT_EQ(TempEntity.DNameData,"tag");
    EXPECT_TRUE(TempEntity.DAttributes.empty());
    EXPECT_TRUE(Reader.ReadEntity(TempEntity));
    EXPECT_EQ(TempEntity.DType,SXMLEntity::EType::EndElement);
    EXPECT_EQ(TempEntity.DNameData,"tag");
    EXPECT_TRUE(TempEntity.DAttributes.empty());
}


TEST(XMLReader, CharDataTest){
    std::string XMLString = "<tag attr1=\"val1\">Some Data</tag>";
    std::shared_ptr<CStringDataSource> DataSource = std::make_shared<CStringDataSource>(XMLString);
    CXMLReader Reader(DataSource);
    SXMLEntity TempEntity;
    EXPECT_TRUE(Reader.ReadEntity(TempEntity));
    EXPECT_EQ(TempEntity.DType,SXMLEntity::EType::StartElement);
    EXPECT_EQ(TempEntity.DNameData,"tag");
    ASSERT_EQ(TempEntity.DAttributes.size(),1);
    EXPECT_EQ(TempEntity.AttributeValue("attr1"),"val1");
    EXPECT_TRUE(Reader.ReadEntity(TempEntity));
    EXPECT_EQ(TempEntity.DType,SXMLEntity::EType::CharData);
    EXPECT_EQ(TempEntity.DNameData,"Some Data");
    EXPECT_TRUE(Reader.ReadEntity(TempEntity));
    EXPECT_EQ(TempEntity.DType,SXMLEntity::EType::EndElement);
    EXPECT_EQ(TempEntity.DNameData,"tag");
    EXPECT_TRUE(TempEntity.DAttributes.empty());
}

