#include "DijkstraTransportationPlanner.h"

struct CDijkstraTransportationPlanner::SImplementation {
    std::shared_ptr<SConfiguration> Config;
    std::vector<std::shared_ptr<CStreetMap::SNode>> SortedNodes;
    
    std::unordered_map<TNodeID, CPathRouter::TVertexID> NodeToVertexID;
    std::unordered_map<CPathRouter::TVertexID, TNodeID> VertexToNodeID;

    SImplementation(std::shared_ptr<SConfiguration> config) : Config(config) {
        if (Config && Config->StreetMap()) {
            for (std::size_t i = 0; i < Config->StreetMap()->NodeCount(); ++i) {
                SortedNodes.push_back(Config->StreetMap()->NodeByIndex(i));
            }
            std::sort(SortedNodes.begin(), SortedNodes.end(), 
                [](const std::shared_ptr<CStreetMap::SNode>& a, const std::shared_ptr<CStreetMap::SNode>& b) {
                    return a->ID() < b->ID();
                });
        }
    }

    std::size_t NodeCount() const noexcept {
        return SortedNodes.size();
    }

    std::shared_ptr<CStreetMap::SNode> SortedNodeByIndex(std::size_t index) const noexcept {
        if (index < SortedNodes.size()) {
            return SortedNodes[index];
        }
        return nullptr;
    }

    double FindShortestPath(TNodeID src, TNodeID dest, std::vector<TNodeID> &path) {
        path.clear();
        if (!Config || !Config->StreetMap()) return CPathRouter::NoPathExists;

        CDijkstraPathRouter Router;
        NodeToVertexID.clear();
        VertexToNodeID.clear();

        for (std::size_t i = 0; i < Config->StreetMap()->NodeCount(); ++i) {
            auto node = Config->StreetMap()->NodeByIndex(i);
            CPathRouter::TVertexID vID = Router.AddVertex(node->ID());
            NodeToVertexID[node->ID()] = vID;
            VertexToNodeID[vID] = node->ID();
        }

        for (std::size_t i = 0; i < Config->StreetMap()->WayCount(); ++i) {
            auto way = Config->StreetMap()->WayByIndex(i);
            bool isOneWay = false;
            
            if (way->HasAttribute("oneway") && way->GetAttribute("oneway") == "yes") {
                isOneWay = true;
            }

            for (std::size_t j = 0; j < way->NodeCount() - 1; ++j) {
                TNodeID nodeID1 = way->GetNodeID(j);
                TNodeID nodeID2 = way->GetNodeID(j + 1);

                auto node1 = Config->StreetMap()->NodeByID(nodeID1);
                auto node2 = Config->StreetMap()->NodeByID(nodeID2);

                if (node1 && node2) {
                    double distance = SGeographicUtils::HaversineDistanceInMiles(node1->Location(), node2->Location());
                    
                    CPathRouter::TVertexID v1 = NodeToVertexID[nodeID1];
                    CPathRouter::TVertexID v2 = NodeToVertexID[nodeID2];

                    Router.AddEdge(v1, v2, distance, !isOneWay);
                }
            }
        }

        if (NodeToVertexID.find(src) == NodeToVertexID.end() || NodeToVertexID.find(dest) == NodeToVertexID.end()) {
            return CPathRouter::NoPathExists;
        }

        std::vector<CPathRouter::TVertexID> routerPath;
        double shortestDistance = Router.FindShortestPath(NodeToVertexID[src], NodeToVertexID[dest], routerPath);

        if (shortestDistance == CPathRouter::NoPathExists) {
            return CPathRouter::NoPathExists;
        }

        for (auto vID : routerPath) {
            path.push_back(VertexToNodeID[vID]);
        }

        return shortestDistance;
    }

    double FindFastestPath(TNodeID src, TNodeID dest, std::vector<TTripStep> &path) {
        path.clear();
        return CPathRouter::NoPathExists; 
    }

    bool GetPathDescription(const std::vector<TTripStep> &path, std::vector<std::string> &desc) const {
        desc.clear();
        return false;
    }
};

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