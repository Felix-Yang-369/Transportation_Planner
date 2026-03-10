#include "DijkstraPathRouter.h"
struct CDijkstraPathRouter::SImplementation{
    struct SVertex;
    using TEdge = std::pair<double,std::shared_ptr<SVertex>>;
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
        auto NewVertex = std::make_shared<SVertex>();
        NewVertex->DTag = tag;
        TVertexID NewID = DVertices.size();
        DVertices.push_back(NewVertex);
        return NewID;
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

    double FindShortestPath(TVertexID src, TVertexID dest, std::vector<TVertexID> &path) noexcept{
        std::vector<double> Weights;
        Weights.resize(DVertices.size(),std::numeric_limits<double>::max());
        std::vector<TVertexID> Previous;
        Previous.resize(DVertices.size(),std::numeric_limits<TVertexID>::max());
        



    }
};

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
