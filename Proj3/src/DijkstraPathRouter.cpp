#include "DijkstraPathRouter.h"

struct CDijkstraPathRouter::SImplementation{
    struct SVertex;
    using TEdge = std::pair<double, CPathRouter::TVertexID>;

    struct SVertex{
        std::vector<TEdge> DEdges;
        std::any DTag;
    };
    std::vector<std::shared_ptr<SVertex>> DVertices;

    SImplementation(){

    }
    ~SImplementation(){

    }

    std::size_t VertexCount() const noexcept{
        return DVertices.size();
    }

    TVertexID AddVertex(std::any tag) noexcept{
        if (weight < 0.0 || src >= DVertices.size() || dest >= DVertices.size()) {
            return false; 
        }
        
        DVertices[src]->DEdges.push_back(std::make_pair(weight, dest));
        
        if (bidir) {
            DVertices[dest]->DEdges.push_back(std::make_pair(weight, src));
        }
        return true;
    }

    std::any GetVertexTag(TVertexID id) const noexcept{
        if(id < DVertices.size()){
            return DVertices[id]->DTag;
        }
        return std::any();
    }

    bool AddEdge(TVertexID src, TVertexID dest, double weight, bool bidir = false) noexcept{
        if(src < DVertices.size() && dest < DVertices.size()){
            DVertices[src]->DEdges.push_back(std::make_pair(weight,DVertices[dest]));
            if(bidir){
                DVertices[dest]->DEdges.push_back(std::make_pair(weight,DVertices[src]));
            }
            return true;
        }
        return false;
    }

    bool Precompute(std::chrono::steady_clock::time_point deadline) noexcept{
        return true;
    }

    // Core Function to Find the Shortest Path
    double FindShortestPath(TVertexID src, TVertexID dest, std::vector<TVertexID> &path) noexcept{
        path.clear();
        
        if (src >= DVertices.size() || dest >= DVertices.size()) {
            return CPathRouter::NoPathExists;
        }

        std::vector<double> Weights(DVertices.size(), std::numeric_limits<double>::infinity());
        std::vector<CPathRouter::TVertexID> Previous(DVertices.size(), std::numeric_limits<CPathRouter::TVertexID>::max());
        
        using NodeDist = std::pair<double, CPathRouter::TVertexID>;
        std::priority_queue<NodeDist, std::vector<NodeDist>, std::greater<NodeDist>> pq;

        Weights[src] = 0.0;
        pq.push({0.0, src});

        while (!pq.empty()) {
            auto [current_dist, current_u] = pq.top();
            pq.pop();

            if (current_u == dest) break;

            if (current_dist > Weights[current_u]) continue;

            for (const auto& edge : DVertices[current_u]->DEdges) {
                double edge_weight = edge.first;
                CPathRouter::TVertexID neighbor_v = edge.second;

                if (Weights[current_u] + edge_weight < Weights[neighbor_v]) {
                    Weights[neighbor_v] = Weights[current_u] + edge_weight;
                    Previous[neighbor_v] = current_u;
                    pq.push({Weights[neighbor_v], neighbor_v});
                }
            }
        }

        if (Weights[dest] == std::numeric_limits<double>::infinity()) {
            return CPathRouter::NoPathExists;
        }

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

// Encapsulation Part

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
