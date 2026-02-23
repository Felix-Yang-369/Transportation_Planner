#include <gtest/gtest.h>
#include "DSVWriter.h"
#include "DSVReader.h"
#include "StringDataSource.h"
#include "StringDataSink.h"

TEST(DSVWriterTest, EmptyRowTest){
    std::shared_ptr<CStringDataSink> DataSink = std::make_shared<CStringDataSink>();
    CDSVWriter Writer(DataSink,',');

    EXPECT_TRUE(DataSink->String().empty());
    EXPECT_EQ(DataSink->String(),"");
    EXPECT_TRUE(Writer.WriteRow({}));
    EXPECT_EQ(DataSink->String(),"");

}


TEST(DSVWriterTest, SingleRowTest){
    std::shared_ptr<CStringDataSink> DataSink = std::make_shared<CStringDataSink>();
    CDSVWriter Writer(DataSink,',');

    EXPECT_TRUE(DataSink->String().empty());
    EXPECT_EQ(DataSink->String(),"");
    EXPECT_TRUE(Writer.WriteRow({"A","B","C"}));
    EXPECT_EQ(DataSink->String(),"A,B,C");

}


TEST(DSVReaderTest, QuotedFieldWithDelimiter){
    std::string Input = "\"A,B\",C";
    auto DataSource = std::make_shared<CStringDataSource>(Input);
    CDSVReader Reader(DataSource, ',');

    std::vector<std::string> Row;
    EXPECT_TRUE(Reader.ReadRow(Row));
    ASSERT_EQ(Row.size(), 2);
    EXPECT_EQ(Row[0], "A,B");
    EXPECT_EQ(Row[1], "C");
}


TEST(DSVReaderTest, EmptyField){
    std::string Input = "A,,C";
    auto DataSource = std::make_shared<CStringDataSource>(Input);
    CDSVReader Reader(DataSource, ',');

    std::vector<std::string> Row;
    EXPECT_TRUE(Reader.ReadRow(Row));
    ASSERT_EQ(Row.size(), 3);
    EXPECT_EQ(Row[0], "A");
    EXPECT_EQ(Row[1], "");
    EXPECT_EQ(Row[2], "C");
}


TEST(DSVIntegrationTest, WriteThenReadSingleRow){
    auto DataSink = std::make_shared<CStringDataSink>();
    CDSVWriter Writer(DataSink, ',');

    EXPECT_TRUE(Writer.WriteRow({"A","B","C"}));
    EXPECT_EQ(DataSink->String(), "A,B,C");

    auto DataSource = std::make_shared<CStringDataSource>(DataSink->String());
    CDSVReader Reader(DataSource, ',');

    std::vector<std::string> Row;
    EXPECT_TRUE(Reader.ReadRow(Row));
    ASSERT_EQ(Row.size(), 3);
    EXPECT_EQ(Row[0], "A");
    EXPECT_EQ(Row[1], "B");
    EXPECT_EQ(Row[2], "C");
}


TEST(DSVIntegrationTest, WriteThenRead){
    auto Sink = std::make_shared<CStringDataSink>();
    CDSVWriter Writer(Sink, ',');

    Writer.WriteRow({"A","B"});

    auto Source = std::make_shared<CStringDataSource>(Sink->String());
    CDSVReader Reader(Source, ',');

    std::vector<std::string> Row;
    EXPECT_TRUE(Reader.ReadRow(Row));
    EXPECT_EQ(Row.size(), 2);
    EXPECT_EQ(Row[0], "A");
    EXPECT_EQ(Row[1], "B");
}