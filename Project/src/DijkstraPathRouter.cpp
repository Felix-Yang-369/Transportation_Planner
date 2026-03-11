#include "DijkstraPathRouter.h"

struct CDijkstraPathRouter::SImplementation {
    using TEdge = std::pair<double, CPathRouter::TVertexID>; 
    
    // Represents a single node in the graph.
    struct SVertex {
        std::vector<TEdge> DEdges;
        std::any DTag;
    };
    
    std::vector<SVertex> DVertices;
    std::vector<double> Weights;
    std::vector<CPathRouter::TVertexID> Previous;

    SImplementation() {}
    ~SImplementation() {}

    // Returns the total number of vertices in the graph.
    std::size_t VertexCount() const noexcept { return DVertices.size(); }

    CPathRouter::TVertexID AddVertex(std::any tag) noexcept {
        CPathRouter::TVertexID NewID = DVertices.size();
        SVertex NewVertex;
        NewVertex.DTag = std::move(tag);
        DVertices.push_back(std::move(NewVertex));
        return NewID;
    }

    std::any GetVertexTag(CPathRouter::TVertexID id) const noexcept {
        if (id < DVertices.size()) return DVertices[id].DTag;
        return std::any();
    }

    // Creates a directed (or bidirectional) edge between two existing vertices.
    bool AddEdge(CPathRouter::TVertexID src, CPathRouter::TVertexID dest, double weight, bool bidir) noexcept {
        if (weight < 0.0 || src >= DVertices.size() || dest >= DVertices.size()) return false; 
        DVertices[src].DEdges.push_back(std::make_pair(weight, dest));
        if (bidir) {
            DVertices[dest].DEdges.push_back(std::make_pair(weight, src));
        }
        return true;
    }

    // Pre-allocates memory for the Dijkstra query arrays before the speed test begins.
    bool Precompute(std::chrono::steady_clock::time_point deadline) noexcept {
        Weights.resize(DVertices.size());
        Previous.resize(DVertices.size());
        return true;
    }

    Core Algorithm: Min-Heap Dijkstra's Shortest Path, Time Complexity: O(E log V)
    double FindShortestPath(CPathRouter::TVertexID src, CPathRouter::TVertexID dest, std::vector<CPathRouter::TVertexID> &path) noexcept {
        path.clear();
        if (src >= DVertices.size() || dest >= DVertices.size()) return CPathRouter::NoPathExists;

        std::fill(Weights.begin(), Weights.end(), std::numeric_limits<double>::infinity());
        std::fill(Previous.begin(), Previous.end(), std::numeric_limits<CPathRouter::TVertexID>::max());
        
        using NodeDist = std::pair<double, CPathRouter::TVertexID>;
        std::priority_queue<NodeDist, std::vector<NodeDist>, std::greater<NodeDist>> pq;

        Weights[src] = 0.0;
        pq.push({0.0, src});

        while (!pq.empty()) {
            auto [current_dist, current_u] = pq.top();
            pq.pop();

            if (current_u == dest) break;
            if (current_dist > Weights[current_u]) continue;

            for (const auto& edge : DVertices[current_u].DEdges) {
                double edge_weight = edge.first;
                CPathRouter::TVertexID neighbor_v = edge.second;

                if (Weights[current_u] + edge_weight < Weights[neighbor_v]) {
                    Weights[neighbor_v] = Weights[current_u] + edge_weight;
                    Previous[neighbor_v] = current_u;
                    pq.push({Weights[neighbor_v], neighbor_v});
                }
            }
        }

        // If the destination weight is still infinity, no valid path exists
        if (Weights[dest] == std::numeric_limits<double>::infinity()) return CPathRouter::NoPathExists;

        CPathRouter::TVertexID curr = dest;
        while (curr != std::numeric_limits<CPathRouter::TVertexID>::max()) {
            path.push_back(curr);
            if (curr == src) break;
            curr = Previous[curr];
        }
        std::reverse(path.begin(), path.end());
        return Weights[dest];
    }
};

// Encapsulation Part(Pimpl Idiom)

CDijkstraPathRouter::CDijkstraPathRouter(){
    DImplementation = std::make_unique<SImplementation>();
}

CDijkstraPathRouter::~CDijkstraPathRouter(){

}

std::size_t CDijkstraPathRouter::VertexCount() const noexcept{
    return DImplementation->VertexCount();
}

CPathRouter::TVertexID CDijkstraPathRouter::AddVertex(std::any tag) noexcept{
    return DImplementation->AddVertex(tag);
}

std::any CDijkstraPathRouter::GetVertexTag(TVertexID id) const noexcept{
    return DImplementation->GetVertexTag(id);
}

bool CDijkstraPathRouter::AddEdge(TVertexID src, TVertexID dest, double weight, bool bidir) noexcept{
    return DImplementation->AddEdge(src,dest,weight,bidir);
}

bool CDijkstraPathRouter::Precompute(std::chrono::steady_clock::time_point deadline) noexcept{
    return DImplementation->Precompute(deadline);
}

double CDijkstraPathRouter::FindShortestPath(TVertexID src, TVertexID dest, std::vector<TVertexID> &path) noexcept{
    return DImplementation->FindShortestPath(src,dest,path);
}
