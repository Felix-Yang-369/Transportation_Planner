# Define the tools in use
AR=ar
CC=gcc
CXX=g++

# Define the directories
INC_DIR			= ./include
SRC_DIR			= ./src
BIN_DIR			= ./bin
OBJ_DIR			= ./obj
LIB_DIR			= ./lib
TESTSRC_DIR		= ./testsrc
TESTOBJ_DIR		= ./testobj
TESTBIN_DIR		= ./testbin
TESTCOVER_DIR 	= ./htmlcov
TESTTMP_DIR		= ./testtmp

# Define the flags
PKGS			= expat
DEFINES			= 
INCLUDE			+= -I $(INC_DIR) 
CFLAGS			+= `pkg-config --cflags $(PKGS)`
CPPFLAGS		+= -std=c++20
LDFLAGS			= `pkg-config --libs $(PKGS)`

TEST_CFLAGS		= $(CFLAGS) -O0 -g --coverage
TEST_CPPFLAGS	= $(CPPFLAGS) -fno-inline
TEST_LDFLAGS	= $(LDFLAGS) -lgtest -lgtest_main -lpthread

# Define the test object files
TEST_STR_OBJ_FILES	= $(TESTOBJ_DIR)/StringUtilsTest.o $(TESTOBJ_DIR)/StringUtils.o
TEST_STRSRC_OBJ_FILES = $(TESTOBJ_DIR)/StringDataSource.o $(TESTOBJ_DIR)/StringDataSourceTest.o
TEST_STRSINK_OBJ_FILES = $(TESTOBJ_DIR)/StringDataSink.o $(TESTOBJ_DIR)/StringDataSinkTest.o
TEST_DSV_OBJ_FILES = $(TESTOBJ_DIR)/StringDataSink.o $(TESTOBJ_DIR)/DSVWriter.o $(TESTOBJ_DIR)/DSVTest.o
TEST_XML_OBJ_FILES = $(TESTOBJ_DIR)/StringDataSource.o $(TESTOBJ_DIR)/XMLReader.o $(TESTOBJ_DIR)/XMLTest.o
TEST_CSVBS_OBJ_FILES = $(TESTOBJ_DIR)/StringDataSource.o $(TESTOBJ_DIR)/DSVReader.o $(TESTOBJ_DIR)/CSVBusSystem.o $(TESTOBJ_DIR)/CSVBusSystemTest.o
TEST_OSM_OBJ_FILES = $(TESTOBJ_DIR)/StringDataSource.o $(TESTOBJ_DIR)/XMLReader.o $(TESTOBJ_DIR)/OpenStreetMap.o $(TESTOBJ_DIR)/OpenStreetMapTest.o
# Define the test target
TEST_STR_TARGET	= $(TESTBIN_DIR)/teststrutils
TEST_STRSRC_TARGET	= $(TESTBIN_DIR)/teststrdatasource 
TEST_STRSINK_TARGET	= $(TESTBIN_DIR)/teststrdatasink
TEST_DSV_TARGET = $(TESTBIN_DIR)/testdsv
TEST_XML_TARGET = $(TESTBIN_DIR)/testxml
TEST_CSVBS_TARGET = $(TESTBIN_DIR)/testcsvbs
TEST_OSM_TARGET	= $(TESTBIN_DIR)/testosm


all: directories run_strtest run_strsrctest run_strsinktest run_dsvtest run_xmltest run_osmtest run_csvbstest  gencoverage

run_strtest: $(TEST_STR_TARGET)
	$(TEST_STR_TARGET) --gtest_output=xml:$(TESTTMP_DIR)/$@
	mv $(TESTTMP_DIR)/$@ $@

run_strsrctest: $(TEST_STRSRC_TARGET)
	$(TEST_STRSRC_TARGET) --gtest_output=xml:$(TESTTMP_DIR)/$@
	mv $(TESTTMP_DIR)/$@ $@

run_strsinktest: $(TEST_STRSINK_TARGET)
	$(TEST_STRSINK_TARGET) --gtest_output=xml:$(TESTTMP_DIR)/$@
	mv $(TESTTMP_DIR)/$@ $@

run_dsvtest: $(TEST_DSV_TARGET)
	$(TEST_DSV_TARGET) --gtest_output=xml:$(TESTTMP_DIR)/$@
	mv $(TESTTMP_DIR)/$@ $@

run_xmltest: $(TEST_XML_TARGET)
	$(TEST_XML_TARGET) --gtest_output=xml:$(TESTTMP_DIR)/$@
	mv $(TESTTMP_DIR)/$@ $@

run_csvbstest: $(TEST_CSVBS_TARGET)
	$(TEST_CSVBS_TARGET) --gtest_output=xml:$(TESTTMP_DIR)/$@
	mv $(TESTTMP_DIR)/$@ $@

run_osmtest: $(TEST_OSM_TARGET)
	$(TEST_OSM_TARGET) --gtest_output=xml:$(TESTTMP_DIR)/$@
	mv $(TESTTMP_DIR)/$@ $@

gencoverage:
	lcov --capture --directory . --output-file $(TESTCOVER_DIR)/coverage.info --ignore-errors inconsistent,source
	lcov --remove $(TESTCOVER_DIR)/coverage.info '/usr/*' '*/testsrc/*' --output-file $(TESTCOVER_DIR)/coverage.info
	genhtml $(TESTCOVER_DIR)/coverage.info --output-directory $(TESTCOVER_DIR)

$(TEST_STR_TARGET): $(TEST_STR_OBJ_FILES)
	$(CXX) $(TEST_CFLAGS) $(TEST_CPPFLAGS) $(TEST_STR_OBJ_FILES) $(TEST_LDFLAGS) -o $(TEST_STR_TARGET)

$(TEST_STRSRC_TARGET): $(TEST_STRSRC_OBJ_FILES)
	$(CXX) $(TEST_CFLAGS) $(TEST_CPPFLAGS) $(TEST_STRSRC_OBJ_FILES) $(TEST_LDFLAGS) -o $(TEST_STRSRC_TARGET)

$(TEST_STRSINK_TARGET): $(TEST_STRSINK_OBJ_FILES)
	$(CXX) $(TEST_CFLAGS) $(TEST_CPPFLAGS) $(TEST_STRSINK_OBJ_FILES) $(TEST_LDFLAGS) -o $(TEST_STRSINK_TARGET)

$(TEST_DSV_TARGET): $(TEST_DSV_OBJ_FILES)
	$(CXX) $(TEST_CFLAGS) $(TEST_CPPFLAGS) $(TEST_DSV_OBJ_FILES) $(TEST_LDFLAGS) -o $(TEST_DSV_TARGET)

$(TEST_XML_TARGET): $(TEST_XML_OBJ_FILES)
	$(CXX) $(TEST_CFLAGS) $(TEST_CPPFLAGS) $(TEST_XML_OBJ_FILES) $(TEST_LDFLAGS) -o $(TEST_XML_TARGET)

$(TEST_CSVBS_TARGET): $(TEST_CSVBS_OBJ_FILES)
	$(CXX) $(TEST_CFLAGS) $(TEST_CPPFLAGS) $(TEST_CSVBS_OBJ_FILES) $(TEST_LDFLAGS) -o $(TEST_CSVBS_TARGET)

$(TEST_OSM_TARGET): $(TEST_OSM_OBJ_FILES)
	$(CXX) $(TEST_CFLAGS) $(TEST_CPPFLAGS) $(TEST_OSM_OBJ_FILES) $(TEST_LDFLAGS) -o $(TEST_OSM_TARGET)

$(TESTOBJ_DIR)/%.o: $(TESTSRC_DIR)/%.cpp
	$(CXX) $(TEST_CFLAGS) $(TEST_CPPFLAGS) $(DEFINES) $(INCLUDE) -c $< -o $@

$(TESTOBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CXX) $(TEST_CFLAGS) $(TEST_CPPFLAGS) $(DEFINES) $(INCLUDE) -c $< -o $@

.PHONY: directories
directories:
	mkdir -p $(BIN_DIR)
	mkdir -p $(OBJ_DIR)
	mkdir -p $(LIB_DIR)
	mkdir -p $(TESTBIN_DIR)
	mkdir -p $(TESTOBJ_DIR)
	mkdir -p $(TESTCOVER_DIR)
	mkdir -p $(TESTTMP_DIR)

clean::
	rm -rf $(BIN_DIR)
	rm -rf $(OBJ_DIR)
	rm -rf $(LIB_DIR)
	rm -rf $(TESTBIN_DIR)
	rm -rf $(TESTOBJ_DIR)
	rm -rf $(TESTCOVER_DIR)
	rm -rf $(TESTTMP_DIR)

.PHONY: clean