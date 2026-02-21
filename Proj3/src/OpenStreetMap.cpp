#include "OpenStreetMap.h"
#include <unordered_map>

struct COpenStreetMap::SImplementation{
    const std::string DOSMTag = "osm";
    const std::string DNodeTag = "node";
    const std::string DNodeIDAttr = "id";
    const std::string DNodeLatAttr = "lat";
    const std::string DNodeLonAttr = "lon";

    struct SNode: public CStreetMap::SNode{
        TNodeID DID;
        TLocation DLocation;
        ~SNode(){};
        TNodeID ID() const noexcept override{

        }

        TLocation Location() const noexcept override{

        }

        std::size_t AttributeCount() const noexcept override{

        }

        std::string GetAttributeKey(std::size_t index) const noexcept override{

        }

        bool HasAttribute(const std::string &key) const noexcept override{

        }

        std::string GetAttribute(const std::string &key) const noexcept override{

        }
    };

    struct SWay: public CStreetMap::SWay{
        ~SWay(){};
        TWayID ID() const noexcept override{

        }

        std::size_t NodeCount() const noexcept override{

        }

        TNodeID GetNodeID(std::size_t index) const noexcept override{

        }

        std::size_t AttributeCount() const noexcept override{

        }

        std::string GetAttributeKey(std::size_t index) const noexcept override{

        }

        bool HasAttribute(const std::string &key) const noexcept override{

        }

        std::string GetAttribute(const std::string &key) const noexcept override{

        }
    };
    std::vector<std::shared_ptr<SNode>> DNodesByIndex;
    std::unordered_map<TNodeID,std::shared_ptr<SNode>> DNodesByID;

    bool ParseNodes(std::shared_ptr<CXMLReader> src, SXMLEntity &nextentity){
        SXMLEntity TempEntity;

        while(src->ReadEntity(TempEntity)){
            if(TempEntity.DType == SXMLEntity::EType::StartElement && TempEntity.DNameData == DNodeTag){
                auto NodeID = std::stoull(TempEntity.AttributeValue(DNodeIDAttr));
                auto NodeLat = std::stod(TempEntity.AttributeValue(DNodeLatAttr));
                auto NodeLon = std::stod(TempEntity.AttributeValue(DNodeLonAttr));
                auto NewNode = std::make_shared<SNode>();
                NewNode->DID = NodeID;
                NewNode->DLocation = std::make_pair(NodeLat,NodeLon);
                DNodesByIndex.push_back(NewNode);
                DNodesByID[NodeID] = NewNode;
                while(src->ReadEntity(TempEntity)){
                    if(TempEntity.DType == SXMLEntity::EType::EndElement && TempEntity.DNameData == DNodeTag){
                        break;
                    }
                }
            }
        }
        return true;
    }


    bool ParseWays(std::shared_ptr<CXMLReader> src, SXMLEntity &firstentity){

    }

    bool ParseOpenStreetMap(std::shared_ptr<CXMLReader> src){
        SXMLEntity TempEntity;
        
        if(src->ReadEntity(TempEntity)){
            if(TempEntity.DType == SXMLEntity::EType::StartElement && TempEntity.DNameData == DOSMTag){
                if(ParseNodes(src,TempEntity)){
                    if(ParseWays(src,TempEntity)){
                        return true;
                    }
                }
            }
        }
        return false;
    }

    SImplementation(std::shared_ptr<CXMLReader> src){
        if(ParseOpenStreetMap(src)){

        }
    }

    std::size_t NodeCount() const noexcept{
        return 0;
    }

    std::size_t WayCount() const noexcept{
        return 0;
    }
    
    std::shared_ptr<CStreetMap::SNode> NodeByIndex(std::size_t index) const noexcept{
        return nullptr;
    }
    
    std::shared_ptr<CStreetMap::SNode> NodeByID(TNodeID id) const noexcept{
        return nullptr;
    }
    
    std::shared_ptr<CStreetMap::SWay> WayByIndex(std::size_t index) const noexcept{
        return nullptr;
    }
    
    std::shared_ptr<CStreetMap::SWay> WayByID(TWayID id) const noexcept{
        return nullptr;
    }
};

COpenStreetMap::COpenStreetMap(std::shared_ptr<CXMLReader> src){
    DImplementation = std::make_unique<SImplementation>(src);
}

COpenStreetMap::~COpenStreetMap(){

}

std::size_t COpenStreetMap::NodeCount() const noexcept {
    return DImplementation->NodeCount();
}
std::size_t COpenStreetMap::WayCount() const noexcept{
    return DImplementation->WayCount();
}

std::shared_ptr<CStreetMap::SNode> COpenStreetMap::NodeByIndex(std::size_t index) const noexcept{
    return DImplementation->NodeByIndex(index);
}

std::shared_ptr<CStreetMap::SNode> COpenStreetMap::NodeByID(TNodeID id) const noexcept{
    return DImplementation->NodeByID(id);
}
std::shared_ptr<CStreetMap::SWay> COpenStreetMap::WayByIndex(std::size_t index) const noexcept{
    return DImplementation->WayByIndex(index);
}

std::shared_ptr<CStreetMap::SWay> COpenStreetMap::WayByID(TWayID id) const noexcept{
    return DImplementation->WayByID(id);
}
