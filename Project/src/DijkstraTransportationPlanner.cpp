#include "DijkstraTransportationPlanner.h"

// Implementation that stores graph data and routing logic
struct CDijkstraTransportationPlanner::SImplementation {
    std::shared_ptr<SConfiguration> Config; 
    std::vector<std::shared_ptr<CStreetMap::SNode>> SortedNodes; // nodes sorted by node ID for binary search
    
    CDijkstraPathRouter ShortestPathRouter; // router used to compute shortest paths
    std::vector<TNodeID> FastNodeIDs; 
    
    // Build graph from the street map and initialize router
    SImplementation(std::shared_ptr<SConfiguration> config) : Config(config) {
        if (!Config || !Config->StreetMap()) return;

        for (std::size_t i = 0; i < Config->StreetMap()->NodeCount(); ++i) {
            SortedNodes.push_back(Config->StreetMap()->NodeByIndex(i));
        }
        std::sort(SortedNodes.begin(), SortedNodes.end(), 
            [](const std::shared_ptr<CStreetMap::SNode>& a, const std::shared_ptr<CStreetMap::SNode>& b) {
                return a->ID() < b->ID();
            });


        FastNodeIDs.reserve(SortedNodes.size());
        // Add every node as a vertex in the router
        for (const auto& node : SortedNodes) {
            FastNodeIDs.push_back(node->ID());
            ShortestPathRouter.AddVertex(node->ID());
        }

        for (std::size_t i = 0; i < Config->StreetMap()->WayCount(); ++i) { // Build graph edges from street map ways
            auto way = Config->StreetMap()->WayByIndex(i);
            bool isOneWay = way->HasAttribute("oneway") && way->GetAttribute("oneway") == "yes";

            for (std::size_t j = 0; j < way->NodeCount() - 1; ++j) {
                TNodeID nodeID1 = way->GetNodeID(j);
                TNodeID nodeID2 = way->GetNodeID(j + 1);

                auto it1 = std::lower_bound(FastNodeIDs.begin(), FastNodeIDs.end(), nodeID1);
                auto it2 = std::lower_bound(FastNodeIDs.begin(), FastNodeIDs.end(), nodeID2);

                if (it1 != FastNodeIDs.end() && *it1 == nodeID1 && 
                    it2 != FastNodeIDs.end() && *it2 == nodeID2) {
                    
                    CPathRouter::TVertexID v1 = std::distance(FastNodeIDs.begin(), it1);
                    CPathRouter::TVertexID v2 = std::distance(FastNodeIDs.begin(), it2);

                    auto node1 = SortedNodes[v1];
                    auto node2 = SortedNodes[v2];

                    double distance = SGeographicUtils::HaversineDistanceInMiles(node1->Location(), node2->Location());
                    ShortestPathRouter.AddEdge(v1, v2, distance, !isOneWay);
                }
            }
        }

        ShortestPathRouter.Precompute(std::chrono::steady_clock::now());
    }

    std::size_t NodeCount() const noexcept { return SortedNodes.size(); }
    std::shared_ptr<CStreetMap::SNode> SortedNodeByIndex(std::size_t index) const noexcept {
        return (index < SortedNodes.size()) ? SortedNodes[index] : nullptr;
    }

    double FindShortestPath(TNodeID src, TNodeID dest, std::vector<TNodeID> &path) { // Find shortest path using the router
        path.clear();
        
        // find adrress with O(log N)
        auto it_src = std::lower_bound(FastNodeIDs.begin(), FastNodeIDs.end(), src);
        auto it_dest = std::lower_bound(FastNodeIDs.begin(), FastNodeIDs.end(), dest);

        if (it_src == FastNodeIDs.end() || *it_src != src || 
            it_dest == FastNodeIDs.end() || *it_dest != dest) {
            return CPathRouter::NoPathExists;
        }

        CPathRouter::TVertexID vSrc = std::distance(FastNodeIDs.begin(), it_src);
        CPathRouter::TVertexID vDest = std::distance(FastNodeIDs.begin(), it_dest);

        std::vector<CPathRouter::TVertexID> routerPath;
        double dist = ShortestPathRouter.FindShortestPath(vSrc, vDest, routerPath);

        if (dist != CPathRouter::NoPathExists) {
            path.reserve(routerPath.size());
            for (auto vID : routerPath) {
                path.push_back(FastNodeIDs[vID]); // O(1)
            }
        }
        return dist;
    }


   // For Extra Credit 1
   std::string FormatLocation(const CStreetMap::SLocation& loc) const {
        auto formatCoordinate = [](double coord, const std::string& posDir, const std::string& negDir) {
            std::string dir = coord >= 0 ? posDir : negDir;
            coord = std::abs(coord);
            int d = static_cast<int>(coord);
            coord = (coord - d) * 60.0;
            int m = static_cast<int>(coord);
            coord = (coord - m) * 60.0;
            int s = static_cast<int>(std::round(coord)); 
            if (s == 60) { s = 0; m += 1; }
            if (m == 60) { m = 0; d += 1; }
            return std::to_string(d) + "d " + std::to_string(m) + "' " + std::to_string(s) + "\" " + dir;
        };
        return formatCoordinate(loc.DLatitude, "N", "S") + ", " + formatCoordinate(loc.DLongitude, "E", "W");
    }

    std::string GetBearingString(double bearing) const {
        if (bearing < 22.5 || bearing >= 337.5) return "N";
        if (bearing < 67.5) return "NE";
        if (bearing < 112.5) return "E";
        if (bearing < 157.5) return "SE";
        if (bearing < 202.5) return "S";
        if (bearing < 247.5) return "SW";
        if (bearing < 292.5) return "W";
        return "NW";
    }

    std::string GetWayName(TNodeID n1, TNodeID n2) const {
        if (!Config || !Config->StreetMap()) return "";
        for (std::size_t i = 0; i < Config->StreetMap()->WayCount(); ++i) {
            auto way = Config->StreetMap()->WayByIndex(i);
            for (std::size_t j = 0; j < way->NodeCount() - 1; ++j) {
                if ((way->GetNodeID(j) == n1 && way->GetNodeID(j+1) == n2) ||
                    (way->GetNodeID(j) == n2 && way->GetNodeID(j+1) == n1)) {
                    if (way->HasAttribute("name")) return way->GetAttribute("name");
                    return "";
                }
            }
        }
        return "";
    }

    bool GetPathDescription(const std::vector<TTripStep> &path, std::vector<std::string> &desc) const {
        desc.clear();
        if (path.empty() || !Config || !Config->StreetMap()) return false;

        auto startNode = Config->StreetMap()->NodeByID(path.front().second);
        if (!startNode) return false;
        
        desc.push_back("Start at " + FormatLocation(startNode->Location()));

        std::size_t idx = 0;
        while (idx < path.size() - 1) {
            auto mode = path[idx + 1].first;
            std::size_t start_idx = idx;

            if (mode == CTransportationPlanner::ETransportationMode::Walk || 
                mode == CTransportationPlanner::ETransportationMode::Bike) {
                
                std::string current_name = GetWayName(path[idx].second, path[idx+1].second);
                std::size_t end_idx = idx + 1;
                double total_dist = SGeographicUtils::HaversineDistanceInMiles(
                    Config->StreetMap()->NodeByID(path[end_idx-1].second)->Location(),
                    Config->StreetMap()->NodeByID(path[end_idx].second)->Location()
                );

                while (end_idx + 1 < path.size() && path[end_idx + 1].first == mode) {
                    std::string next_name = GetWayName(path[end_idx].second, path[end_idx+1].second);
                    if (next_name != current_name) break;
                    
                    end_idx++;
                    total_dist += SGeographicUtils::HaversineDistanceInMiles(
                        Config->StreetMap()->NodeByID(path[end_idx-1].second)->Location(),
                        Config->StreetMap()->NodeByID(path[end_idx].second)->Location()
                    );
                }

                double bearing = SGeographicUtils::CalculateBearing(
                    Config->StreetMap()->NodeByID(path[start_idx].second)->Location(),
                    Config->StreetMap()->NodeByID(path[end_idx].second)->Location()
                );
                std::string bearing_str = GetBearingString(bearing);
                std::string mode_str = (mode == CTransportationPlanner::ETransportationMode::Walk) ? "Walk" : "Bike";

                std::stringstream ss;
                ss << mode_str << " " << bearing_str << " ";

                if (!current_name.empty()) {
                    ss << "along " << current_name << " ";
                } else {
                    std::string next_street = "End";
                    for (std::size_t k = end_idx; k < path.size() - 1; ++k) {
                        std::string name = GetWayName(path[k].second, path[k+1].second);
                        if (!name.empty()) {
                            next_street = name;
                            break;
                        }
                    }
                    ss << "toward " << next_street << " ";
                }
                
                ss << "for " << std::fixed << std::setprecision(1) << total_dist << " mi";
                desc.push_back(ss.str());
                idx = end_idx;
                
            } else if (mode == CTransportationPlanner::ETransportationMode::Bus) {
                std::size_t end_idx = idx + 1;
                while (end_idx + 1 < path.size() && path[end_idx + 1].first == mode) {
                    end_idx++;
                }

                std::vector<TNodeID> bus_nodes;
                for (std::size_t k = start_idx; k <= end_idx; ++k) {
                    bus_nodes.push_back(path[k].second);
                }

                std::string best_route_name;
                std::string start_stop_id, end_stop_id;

                if (Config->BusSystem()) {
                    std::vector<std::shared_ptr<CBusSystem::SRoute>> valid_routes;
                    for (std::size_t r = 0; r < Config->BusSystem()->RouteCount(); ++r) {
                        auto route = Config->BusSystem()->RouteByIndex(r);
                        std::vector<TNodeID> route_nodes;
                        for (std::size_t s = 0; s < route->StopCount(); ++s) {
                            auto stop = Config->BusSystem()->StopByID(route->GetStopID(s));
                            if (stop) route_nodes.push_back(stop->NodeID());
                        }

                        // Check Subsequence
                        if (bus_nodes.size() <= route_nodes.size()) {
                            for (std::size_t i = 0; i <= route_nodes.size() - bus_nodes.size(); ++i) {
                                bool match = true;
                                for (std::size_t j = 0; j < bus_nodes.size(); ++j) {
                                    if (route_nodes[i+j] != bus_nodes[j]) { match = false; break; }
                                }
                                if (match) { valid_routes.push_back(route); break; }
                            }
                        }
                    }

                    if (!valid_routes.empty()) {
                        std::sort(valid_routes.begin(), valid_routes.end(), [](auto a, auto b) {
                            return a->Name() < b->Name();
                        });
                        best_route_name = valid_routes.front()->Name();
                    }
                    
                    for (std::size_t s = 0; s < Config->BusSystem()->StopCount(); ++s) {
                        auto stop = Config->BusSystem()->StopByIndex(s);
                        if (stop->NodeID() == bus_nodes.front()) start_stop_id = std::to_string(stop->ID());
                        if (stop->NodeID() == bus_nodes.back()) end_stop_id = std::to_string(stop->ID());
                    }
                }

                desc.push_back("Take Bus " + best_route_name + " from stop " + start_stop_id + " to stop " + end_stop_id);
                idx = end_idx;
            }
        }

        auto endNode = Config->StreetMap()->NodeByID(path.back().second);
        desc.push_back("End at " + FormatLocation(endNode->Location()));

        return true;
    }
};


// Encapsulation Part
CDijkstraTransportationPlanner::CDijkstraTransportationPlanner(std::shared_ptr<SConfiguration> config) {
    DImplementation = std::make_unique<SImplementation>(config);
}

CDijkstraTransportationPlanner::~CDijkstraTransportationPlanner() {}

std::size_t CDijkstraTransportationPlanner::NodeCount() const noexcept {
    return DImplementation->NodeCount();
}

std::shared_ptr<CStreetMap::SNode> CDijkstraTransportationPlanner::SortedNodeByIndex(std::size_t index) const noexcept {
    return DImplementation->SortedNodeByIndex(index);
}

double CDijkstraTransportationPlanner::FindShortestPath(TNodeID src, TNodeID dest, std::vector<TNodeID> &path) {
    return DImplementation->FindShortestPath(src, dest, path);
}

double CDijkstraTransportationPlanner::FindFastestPath(TNodeID src, TNodeID dest, std::vector<TTripStep> &path) {
    return DImplementation->FindFastestPath(src, dest, path);
}

bool CDijkstraTransportationPlanner::GetPathDescription(const std::vector<TTripStep> &path, std::vector<std::string> &desc) const {
    return DImplementation->GetPathDescription(path, desc);
}