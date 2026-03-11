#include <gtest/gtest.h>
#include "StringUtils.h"

TEST(StringUtilsTest, SliceTest){
    EXPECT_EQ(StringUtils::Slice("abcdef", 0, 2),std::string("ab"));
    EXPECT_EQ(StringUtils::Slice("abcdef", 3),std::string("def"));
    EXPECT_EQ(StringUtils::Slice("abcdef", 0),std::string("abcdef"));
    EXPECT_EQ(StringUtils::Slice("abcdef", 0, 10),std::string("abcdef"));
    EXPECT_EQ(StringUtils::Slice("abcdef", 0, -1),std::string("abcde"));
    EXPECT_EQ(StringUtils::Slice("abcdef", 1, 1),std::string(""));
    EXPECT_EQ(StringUtils::Slice("abcdef", 10),std::string(""));
}

TEST(StringUtilsTest, Capitalize){
    EXPECT_EQ(StringUtils::Capitalize(""),std::string(""));
    EXPECT_EQ(StringUtils::Capitalize("hello"),std::string("Hello"));
    EXPECT_EQ(StringUtils::Capitalize(" hello world"),std::string(" hello world"));
    EXPECT_EQ(StringUtils::Capitalize("hello world"),std::string("Hello world"));
    EXPECT_EQ(StringUtils::Capitalize("HELLO WORLD"),std::string("Hello world"));
}

TEST(StringUtilsTest, Upper){
    EXPECT_EQ(StringUtils::Upper(""),std::string(""));
    EXPECT_EQ(StringUtils::Upper("abc"),std::string("ABC"));
    EXPECT_EQ(StringUtils::Upper("123abc"),std::string("123ABC"));
    EXPECT_EQ(StringUtils::Upper(" a b c "),std::string(" A B C "));
    EXPECT_EQ(StringUtils::Upper("ABC"),std::string("ABC"));
    EXPECT_EQ(StringUtils::Upper("Abc"),std::string("ABC"));
}

TEST(StringUtilsTest, Lower){
    EXPECT_EQ(StringUtils::Lower(""),std::string(""));
    EXPECT_EQ(StringUtils::Lower("ABC"),std::string("abc"));
    EXPECT_EQ(StringUtils::Lower("123ABC"),std::string("123abc"));
    EXPECT_EQ(StringUtils::Lower(" A B C "),std::string(" a b c "));
    EXPECT_EQ(StringUtils::Lower("Abc"),std::string("abc"));
    EXPECT_EQ(StringUtils::Lower("abc"),std::string("abc"));
}

TEST(StringUtilsTest, LStrip){
    EXPECT_EQ(StringUtils::LStrip("Hello"),std::string("Hello"));
    EXPECT_EQ(StringUtils::LStrip(" Hello"),std::string("Hello"));
    EXPECT_EQ(StringUtils::LStrip("Hello "),std::string("Hello "));
    EXPECT_EQ(StringUtils::LStrip("   Hello"),std::string("Hello"));
    EXPECT_EQ(StringUtils::LStrip(" \r \t \nHello"),std::string("Hello"));
}

TEST(StringUtilsTest, RStrip){
    EXPECT_EQ(StringUtils::RStrip("      "),std::string(""));
    EXPECT_EQ(StringUtils::RStrip(""),std::string(""));
    EXPECT_EQ(StringUtils::RStrip("Hello"),std::string("Hello"));
    EXPECT_EQ(StringUtils::RStrip("Hello "),std::string("Hello"));
    EXPECT_EQ(StringUtils::RStrip(" Hello"),std::string(" Hello"));
    EXPECT_EQ(StringUtils::RStrip("Hello   "),std::string("Hello"));
    EXPECT_EQ(StringUtils::RStrip("Hello \r \t \n"),std::string("Hello"));
}

TEST(StringUtilsTest, Strip){
    EXPECT_EQ(StringUtils::Strip(" Hello"),std::string("Hello"));
    EXPECT_EQ(StringUtils::Strip("Hello "),std::string("Hello"));
    EXPECT_EQ(StringUtils::Strip("   Hello"),std::string("Hello"));
    EXPECT_EQ(StringUtils::Strip(" \r \t \nHello"),std::string("Hello"));
    EXPECT_EQ(StringUtils::Strip("      "),std::string(""));
    EXPECT_EQ(StringUtils::Strip(""),std::string(""));
    EXPECT_EQ(StringUtils::Strip("Hello   "),std::string("Hello"));
    EXPECT_EQ(StringUtils::Strip("Hello \r \t \n"),std::string("Hello"));
    EXPECT_EQ(StringUtils::Strip(" \r \t \nHello \r \t \n"),std::string("Hello"));
}

TEST(StringUtilsTest, Center){
    EXPECT_EQ(StringUtils::Center("abc", 3),std::string("abc"));
    EXPECT_EQ(StringUtils::Center("abc", 1),std::string("abc"));
    EXPECT_EQ(StringUtils::Center("abc", 5),std::string(" abc "));
    EXPECT_EQ(StringUtils::Center("abc", 6),std::string(" abc  "));
    EXPECT_EQ(StringUtils::Center("abc", 5, '-'),std::string("-abc-"));
    EXPECT_EQ(StringUtils::Center("", 3),std::string("   "));
}

TEST(StringUtilsTest, LJust){
    EXPECT_EQ(StringUtils::LJust("abc", 3),std::string("abc"));
    EXPECT_EQ(StringUtils::LJust("abc", 5),std::string("abc  "));
    EXPECT_EQ(StringUtils::LJust("abc", 2),std::string("abc"));
    EXPECT_EQ(StringUtils::LJust("abc", 5, '-'),std::string("abc--"));
    EXPECT_EQ(StringUtils::LJust("", 3),std::string("   "));
}

TEST(StringUtilsTest, RJust){
    EXPECT_EQ(StringUtils::RJust("abc", 3),std::string("abc"));
    EXPECT_EQ(StringUtils::RJust("abc", 5),std::string("  abc"));
    EXPECT_EQ(StringUtils::RJust("abc", 2),std::string("abc"));
    EXPECT_EQ(StringUtils::RJust("abc", 5, '-'),std::string("--abc"));
    EXPECT_EQ(StringUtils::RJust("", 3),std::string("   "));
}

TEST(StringUtilsTest, Replace){
    EXPECT_EQ(StringUtils::Replace("hello","l","a"),std::string("heaao"));
    EXPECT_EQ(StringUtils::Replace("hello","b","a"),std::string("hello"));
    EXPECT_EQ(StringUtils::Replace("hello","l","l"),std::string("hello"));
    EXPECT_EQ(StringUtils::Replace("","l","a"),std::string(""));
    EXPECT_EQ(StringUtils::Replace("abcabc","abc","x"),std::string("xx"));
    EXPECT_EQ(StringUtils::Replace("abcabc","abc","xxxx"),std::string("xxxxxxxx"));
    EXPECT_EQ(StringUtils::Replace("abcabc","abc","xxx"),std::string("xxxxxx"));
}

TEST(StringUtilsTest, Split){
    EXPECT_EQ(StringUtils::Split("a,b,c", ","), std::vector<std::string>({"a","b","c"}));
    EXPECT_EQ(StringUtils::Split("a,,c", ","), std::vector<std::string>({"a","","c"}));
    EXPECT_EQ(StringUtils::Split(",a,b,c,", ","), std::vector<std::string>({"","a","b","c",""}));
    EXPECT_EQ(StringUtils::Split("abc", ","), std::vector<std::string>({"abc"}));
    EXPECT_EQ(StringUtils::Split("", ","), std::vector<std::string>({""}));
    EXPECT_EQ(StringUtils::Split("a b c"), std::vector<std::string>({"a","b","c"}));
    EXPECT_EQ(StringUtils::Split(" a b\tc "), std::vector<std::string>({"a","b","c"}));
    EXPECT_EQ(StringUtils::Split(""), std::vector<std::string>({""}));
    EXPECT_EQ(StringUtils::Split("a,,b,,c", ",,"), std::vector<std::string>({"a","b","c"}));
}

TEST(StringUtilsTest, Join){
    EXPECT_EQ(StringUtils::Join(",", {"a","b","c"}), std::string("a,b,c"));
    EXPECT_EQ(StringUtils::Join(",", {"abc"}), std::string("abc"));
    EXPECT_EQ(StringUtils::Join(",", {"ab","c"}), std::string("ab,c"));
    EXPECT_EQ(StringUtils::Join(",", {}), std::string(""));
}

TEST(StringUtilsTest, ExpandTabs){
    EXPECT_EQ(StringUtils::ExpandTabs("a\tb",4), std::string("a   b"));
    EXPECT_EQ(StringUtils::ExpandTabs("a\tb",5), std::string("a    b"));
    EXPECT_EQ(StringUtils::ExpandTabs("ab\tc",4), std::string("ab  c"));
    EXPECT_EQ(StringUtils::ExpandTabs("abc\td",4), std::string("abc d"));
    EXPECT_EQ(StringUtils::ExpandTabs("abcd\te",4), std::string("abcd    e"));
    EXPECT_EQ(StringUtils::ExpandTabs("a\t\tb",4), std::string("a       b"));
    EXPECT_EQ(StringUtils::ExpandTabs("\t",4), std::string("    "));
}

TEST(StringUtilsTest, EditDistance){
    EXPECT_EQ(StringUtils::EditDistance("a","a"), 0);
    EXPECT_EQ(StringUtils::EditDistance("a","b"), 1);
    EXPECT_EQ(StringUtils::EditDistance("a","ab"), 1);
    EXPECT_EQ(StringUtils::EditDistance("ab","a"), 1);
    EXPECT_EQ(StringUtils::EditDistance("","abc"), 3);
    EXPECT_EQ(StringUtils::EditDistance("abc",""), 3);
    EXPECT_EQ(StringUtils::EditDistance("iloveyou","ilikeyou"), 2);
    EXPECT_EQ(StringUtils::EditDistance("A","a",true), 0);
    EXPECT_EQ(StringUtils::EditDistance("A","a",false), 1);
}
