#include TransportationPlannerCommandLine.h

struct CBusSystemIndexer::SImplementation {
    std::shared_ptr<CBusSystem> BusSystem;
    std::vector<std::shared_ptr<SStop>> SortedStops;
    std::vector<std::shared_ptr<SRoute>> SortedRoutes;
    std::unordered_map<TNodeID, std::shared_ptr<SStop>> NodeIDToStop;
    std::unordered_map<TNodeID, std::unordered_map<TNodeID, std::unordered_set<std::shared_ptr<SRoute>>>> RouteSegments;

    // Constructor
    SImplementation(std::shared_ptr<CBusSystem> bussystem) : BusSystem(bussystem) {
        if (!BusSystem) return;

        for (std::size_t i = 0; i < BusSystem->StopCount(); ++i) {
            auto stop = BusSystem->StopByIndex(i);
            if (stop) {
                SortedStops.push_back(stop);
                NodeIDToStop[stop->NodeID()] = stop;
            }
        }
        
        // Sort stops primarily by their unique Stop ID
        std::sort(SortedStops.begin(), SortedStops.end(), [](const std::shared_ptr<SStop>& a, const std::shared_ptr<SStop>& b) {
            return a->ID() < b->ID();
        });

        for (std::size_t i = 0; i < BusSystem->RouteCount(); ++i) {
            auto route = BusSystem->RouteByIndex(i);
            if (route) {
                SortedRoutes.push_back(route);
                
                for (std::size_t j = 1; j < route->StopCount(); ++j) {
                    auto stop1 = BusSystem->StopByID(route->GetStopID(j - 1));
                    auto stop2 = BusSystem->StopByID(route->GetStopID(j));
                    if (stop1 && stop2) {
                        RouteSegments[stop1->NodeID()][stop2->NodeID()].insert(route);
                    }
                }
            }
        }
        
        // Sort routes alphabetically by their Name
        std::sort(SortedRoutes.begin(), SortedRoutes.end(), [](const std::shared_ptr<SRoute>& a, const std::shared_ptr<SRoute>& b) {
            return a->Name() < b->Name();
        });
    }

    std::size_t StopCount() const noexcept {
        return SortedStops.size();
    }

    std::size_t RouteCount() const noexcept {
        return SortedRoutes.size();
    }

    std::shared_ptr<SStop> SortedStopByIndex(std::size_t index) const noexcept {
        if (index < SortedStops.size()) {
            return SortedStops[index];
        }
        return nullptr;
    }

    std::shared_ptr<SRoute> SortedRouteByIndex(std::size_t index) const noexcept {
        if (index < SortedRoutes.size()) {
            return SortedRoutes[index];
        }
        return nullptr;
    }

    std::shared_ptr<SStop> StopByNodeID(TNodeID id) const noexcept {
        auto it = NodeIDToStop.find(id);
        if (it != NodeIDToStop.end()) {
            return it->second;
        }
        return nullptr;
    }

    // O(1) lookup to find all bus routes that travel directly from 'src' node to 'dest' node
    bool RoutesByNodeIDs(TNodeID src, TNodeID dest, std::unordered_set<std::shared_ptr<SRoute>> &routes) const noexcept {
        auto srcIt = RouteSegments.find(src);
        if (srcIt != RouteSegments.end()) {
            auto destIt = srcIt->second.find(dest);
            if (destIt != srcIt->second.end()) {
                routes = destIt->second;
                return !routes.empty();
            }
        }
        return false;
    }

    // O(1) lookup to check if ANY bus route connects 'src' node to 'dest' node
    bool RouteBetweenNodeIDs(TNodeID src, TNodeID dest) const noexcept {
        auto srcIt = RouteSegments.find(src);
        if (srcIt != RouteSegments.end()) {
            auto destIt = srcIt->second.find(dest);
            if (destIt != srcIt->second.end()) {
                return !destIt->second.empty();
            }
        }
        return false;
    }
};

// Encapsulation Part (Pimpl Idiom)
CBusSystemIndexer::CBusSystemIndexer(std::shared_ptr<CBusSystem> bussystem) {
    DImplementation = std::make_unique<SImplementation>(bussystem);
}

CBusSystemIndexer::~CBusSystemIndexer() {}

std::size_t CBusSystemIndexer::StopCount() const noexcept {
    return DImplementation->StopCount();
}

std::size_t CBusSystemIndexer::RouteCount() const noexcept {
    return DImplementation->RouteCount();
}

std::shared_ptr<SStop> CBusSystemIndexer::SortedStopByIndex(std::size_t index) const noexcept {
    return DImplementation->SortedStopByIndex(index);
}

std::shared_ptr<SRoute> CBusSystemIndexer::SortedRouteByIndex(std::size_t index) const noexcept {
    return DImplementation->SortedRouteByIndex(index);
}

std::shared_ptr<SStop> CBusSystemIndexer::StopByNodeID(TNodeID id) const noexcept {
    return DImplementation->StopByNodeID(id);
}

bool CBusSystemIndexer::RoutesByNodeIDs(TNodeID src, TNodeID dest, std::unordered_set<std::shared_ptr<SRoute>> &routes) const noexcept {
    return DImplementation->RoutesByNodeIDs(src, dest, routes);
}

bool CBusSystemIndexer::RouteBetweenNodeIDs(TNodeID src, TNodeID dest) const noexcept {
    return DImplementation->RouteBetweenNodeIDs(src, dest);
}