#include <iostream>
#include <memory>
#include "DijkstraTransportationPlanner.h"
#include "TransportationPlannerCommandLine.h"
#include "OpenStreetMap.h"
#include "CSVBusSystem.h"
#include "XMLReader.h"
#include "DSVReader.h"
#include "FileDataFactory.h"
#include "StandardDataSource.h"
#include "StandardDataSink.h"
#include "TransportationPlannerConfig.h"

int main(int argc, char *argv[]) {
    if (argc < 4) {
        std::cerr << "Usage: " << argv[0] << " <osm_file> <stops_file> <routes_file>" << std::endl;
        return 1;
    }

    // 1. Create a File Factory
    auto FileFactory = std::make_shared<CFileDataFactory>();

    // 2. Open Data File
    auto OSMDataSource = FileFactory->CreateSource(argv[1]);
    auto StopsDataSource = FileFactory->CreateSource(argv[2]);
    auto RoutesDataSource = FileFactory->CreateSource(argv[3]);

    if (!OSMDataSource || !StopsDataSource || !RoutesDataSource) {
        std::cerr << "Error: Could not open one or more data files." << std::endl;
        return 1;
    }

    // 3. Parse OpenStreetMap Data
    auto XMLReader = std::make_shared<CXMLReader>(OSMDataSource);
    auto StreetMap = std::make_shared<COpenStreetMap>(XMLReader);

    // 4. Parse Transportation Data
    auto StopsDSVReader = std::make_shared<CDSVReader>(StopsDataSource, ',');
    auto RoutesDSVReader = std::make_shared<CDSVReader>(RoutesDataSource, ',');
    auto BusSystem = std::make_shared<CCSVBusSystem>(StopsDSVReader, RoutesDSVReader);

    // 5. Initialize Planner
    auto Config = std::make_shared<STransportationPlannerConfig>(StreetMap, BusSystem);
    auto Planner = std::make_shared<CDijkstraTransportationPlanner>(Config);

    // 6. Set Input and Output
    auto CmdSrc = std::make_shared<CStandardDataSource>(std::cin);
    auto OutSink = std::make_shared<CStandardDataSink>(std::cout);
    auto ErrSink = std::make_shared<CStandardDataSink>(std::cerr);

    // 7. Start CommandLIne
    CTransportationPlannerCommandLine CommandLine(CmdSrc, OutSink, ErrSink, FileFactory, Planner);

    CommandLine.ProcessCommands();

    return 0;
}