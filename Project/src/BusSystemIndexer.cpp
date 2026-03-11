#include "BusSystemIndexer.h"
#include <vector>
#include <unordered_map>
#include <algorithm>

// Implementation struct that stores all indexing data
struct CBusSystemIndexer::SImplementation{
    std::shared_ptr<CBusSystem> DBusSystem; 
    std::vector<std::shared_ptr<SStop>> DSortedStopsByIndex; // Stops sorted by Stop ID
    std::unordered_map<TNodeID,std::shared_ptr<SStop>> DStopsByNodeID; // Fast lookup: nodeID -> stop
    std::vector<std::shared_ptr<SRoute>> DSortedRoutesByIndex; // Routes sorted by route name
    
    // Custom hash for pair<NodeID, NodeID>
    struct SNodeIDPairHash{
        std::size_t operator()(const std::pair<TNodeID,TNodeID> &pair) const{
            std::size_t First = pair.first;
            std::size_t Second = pair.second;
            return First ^ (Second<<1); 
        }
    };
    
    std::unordered_map<std::pair<TNodeID,TNodeID>,std::unordered_set<std::shared_ptr<SRoute>>,SNodeIDPairHash> DRoutesByNodeIDs;
    // Build all indices from the bus system
    SImplementation(std::shared_ptr<CBusSystem> bussystem){
        DBusSystem = bussystem;
        for(size_t Index = 0; Index < DBusSystem->StopCount(); Index++){
            auto Stop = DBusSystem->StopByIndex(Index);
            DSortedStopsByIndex.push_back(Stop);
            DStopsByNodeID[Stop->NodeID()] = Stop;
        }
        std::sort(DSortedStopsByIndex.begin(), DSortedStopsByIndex.end(), [](std::shared_ptr<SStop> l, std::shared_ptr<SStop> r) -> bool{
            return l->ID() < r->ID();
        });
        for(size_t Index = 0; Index < DBusSystem->RouteCount(); Index++){
            auto Route = DBusSystem->RouteByIndex(Index);
            DSortedRoutesByIndex.push_back(Route);
        }
        std::sort(DSortedRoutesByIndex.begin(), DSortedRoutesByIndex.end(), [](std::shared_ptr<SRoute> l, std::shared_ptr<SRoute> r) -> bool{
            return l->Name() < r->Name();
        });
        // Build a map of routes between consecutive node pairs
        for(auto Route: DSortedRoutesByIndex){
            for(size_t Index = 1; Index < Route->StopCount(); Index++){
                auto Previous = Route->GetStopID(Index-1);
                auto Current = Route->GetStopID(Index);
                auto FirstNodeID = DBusSystem->StopByID(Previous)->NodeID();
                auto SecondNodeID = DBusSystem->StopByID(Current)->NodeID();
                auto Key = std::make_pair(FirstNodeID,SecondNodeID);
                auto Search = DRoutesByNodeIDs.find(Key);
                if(Search == DRoutesByNodeIDs.end()){
                    DRoutesByNodeIDs[Key] = {Route};
                }
                else{
                    Search->second.insert(Route);
                }
            }
        }

    }

    ~SImplementation(){

    }

    std::size_t StopCount() const noexcept{
        return DBusSystem->StopCount();
    }

    std::size_t RouteCount() const noexcept{
        return DBusSystem->RouteCount();
    }

    std::shared_ptr<SStop> SortedStopByIndex(std::size_t index) const noexcept{
        if(index >= DSortedStopsByIndex.size()){
            return nullptr;
        }
        return DSortedStopsByIndex[index];  
    }

    std::shared_ptr<SRoute> SortedRouteByIndex(std::size_t index) const noexcept{
        if(index >= DSortedRoutesByIndex.size()){
            return nullptr;
        }
        return DSortedRoutesByIndex[index]; 
    }

    std::shared_ptr<SStop> StopByNodeID(TNodeID id) const noexcept{
        auto Search = DStopsByNodeID.find(id);

        if(Search != DStopsByNodeID.end()){
            return Search->second;
        }

        return nullptr;
    }

    bool RoutesByNodeIDs(TNodeID src, TNodeID dest, std::unordered_set<std::shared_ptr<SRoute> > &routes) const noexcept{
        auto Search = DRoutesByNodeIDs.find(std::make_pair(src,dest));
        if(Search != DRoutesByNodeIDs.end()){
            routes = Search->second;
            return true;
        }
        return false;
    }

    bool RouteBetweenNodeIDs(TNodeID src, TNodeID dest) const noexcept{
        auto Search = DRoutesByNodeIDs.find(std::make_pair(src,dest));
        return Search != DRoutesByNodeIDs.end();
    }

};
// Wrapper functions that forward calls to the implementation
CBusSystemIndexer::CBusSystemIndexer(std::shared_ptr<CBusSystem> bussystem){
    DImplementation = std::make_unique<SImplementation>(bussystem);
}

CBusSystemIndexer::~CBusSystemIndexer(){

}

std::size_t CBusSystemIndexer::StopCount() const noexcept{
    return DImplementation->StopCount();
}

std::size_t CBusSystemIndexer::RouteCount() const noexcept{
    return DImplementation->RouteCount();
}

std::shared_ptr<CBusSystemIndexer::SStop> CBusSystemIndexer::SortedStopByIndex(std::size_t index) const noexcept{
    return DImplementation->SortedStopByIndex(index);
}

std::shared_ptr<CBusSystemIndexer::SRoute> CBusSystemIndexer::SortedRouteByIndex(std::size_t index) const noexcept{
    return DImplementation->SortedRouteByIndex(index);
}

std::shared_ptr<CBusSystemIndexer::SStop> CBusSystemIndexer::StopByNodeID(TNodeID id) const noexcept{
    return DImplementation->StopByNodeID(id);
}

bool CBusSystemIndexer::RoutesByNodeIDs(TNodeID src, TNodeID dest, std::unordered_set<std::shared_ptr<SRoute> > &routes) const noexcept{
    return DImplementation->RoutesByNodeIDs(src,dest,routes);
}

bool CBusSystemIndexer::RouteBetweenNodeIDs(TNodeID src, TNodeID dest) const noexcept{
    return DImplementation->RouteBetweenNodeIDs(src,dest);
}
