#include "OpenStreetMap.h"
#include <unordered_map>

struct COpenStreetMap::SImplementation{
    const std::string DOSMTag = "osm";
    const std::string DNodeTag = "node";
    const std::string DNodeIDAttr = "id";
    const std::string DNodeLatAttr = "lat";
    const std::string DNodeLonAttr = "lon";

    struct SNode: public CStreetMap::SNode{
        CStreetMap::TNodeID DID;
        CStreetMap::TLocation DLocation;
        std::vector<std::pair<std::string, std::string>> DAttributes;
        ~SNode(){};
        TNodeID ID() const noexcept override{
            return DID;
        }

        TLocation Location() const noexcept override{
            return DLocation;
        }

        std::size_t AttributeCount() const noexcept override{
            return DAttributes.size();
        }

        std::string GetAttributeKey(std::size_t index) const noexcept override{
            if (index>= DAttributes.size()) {
                return "";
            }
            return DAttributes[index].first;
        }

        bool HasAttribute(const std::string &key) const noexcept override{
            for (const auto &kv : DAttributes) {
                if (kv.first==key) {
                    return true;
                }
            }
            return false;
        }

        std::string GetAttribute(const std::string &key) const noexcept override{
            for (const auto &kv : DAttributes) {
                if (kv.first==key) {
                    return kv.second;
                }
            }
            return "";
        }
    };

    struct SWay: public CStreetMap::SWay{
        CStreetMap::TWayID DID;
        std::vector<TNodeID> DNodeIDs;
        std::vector<std::pair<std::string, std::string>> DAttributes;
        ~SWay(){};
        TWayID ID() const noexcept override{
            return DID;
        }

        std::size_t NodeCount() const noexcept override{
            return DNodeIDs.size();
        }

        TNodeID GetNodeID(std::size_t index) const noexcept override{
            if (index >= DNodeIDs.size()) {
                return CStreetMap::InvalidNodeID;
            }
            return DNodeIDs[index];
        }

        std::size_t AttributeCount() const noexcept override{
            return DAttributes.size();
        }

        std::string GetAttributeKey(std::size_t index) const noexcept override{
            if (index>=DAttributes.size()) {
                return "";
            }
            return DAttributes[index].first;
        }

        bool HasAttribute(const std::string &key) const noexcept override{
            for (const auto &kv : DAttributes) {
                if (kv.first==key) {
                    return true;
                }
            }
            return false;
        }

        std::string GetAttribute(const std::string &key) const noexcept override{
            for (const auto &kv : DAttributes) {
                if (kv.first==key) {
                    return kv.second;
                }
            }
            return "";
        }
    };
    std::vector<std::shared_ptr<SNode>> DNodesByIndex;
    std::unordered_map<TNodeID,std::shared_ptr<SNode>> DNodesByID;
    std::vector<std::shared_ptr<SWay>> DWaysByIndex; //container
    std::unordered_map<TWayID,std::shared_ptr<SWay>> DWaysByID;

    bool ParseNodes(std::shared_ptr<CXMLReader> src, SXMLEntity &nextentity){
        SXMLEntity TempEntity;

        while(src->ReadEntity(TempEntity)){
            if(TempEntity.DType == SXMLEntity::EType::StartElement && TempEntity.DNameData == "way") { //if it is a way rather than a node.
                nextentity=TempEntity;
                return true;
            }

            if(TempEntity.DType == SXMLEntity::EType::StartElement && TempEntity.DNameData == DNodeTag){
                auto NodeID = std::stoull(TempEntity.AttributeValue(DNodeIDAttr));
                auto NodeLat = std::stod(TempEntity.AttributeValue(DNodeLatAttr));
                auto NodeLon = std::stod(TempEntity.AttributeValue(DNodeLonAttr));
                auto NewNode = std::make_shared<SNode>();
                NewNode->DID = NodeID;
                NewNode->DLocation = std::make_pair(NodeLat,NodeLon);
                DNodesByIndex.push_back(NewNode);
                DNodesByID[NodeID] = NewNode;
            }
        }
        return true;
    }


    bool ParseWays(std::shared_ptr<CXMLReader> src, SXMLEntity &firstentity){
        SXMLEntity TempEntity;
        while (true){
            if(TempEntity.DType == SXMLEntity::EType::StartElement && TempEntity.DNameData == "way"){
                auto WayID=std::stoull(TempEntity.AttributeValue("id"));
                auto NewWay=std::make_shared<SWay>();
                NewWay->DID=WayID;

                while (src->ReadEntity(TempEntity)){
                    if (TempEntity.DType==SXMLEntity::EType::StartElement && TempEntity.DNameData=="nd"){
                        auto RefID=std::stoull(TempEntity.AttributeValue("ref"));
                        NewWay->DNodeIDs.push_back(RefID);
                    }
                    else if (TempEntity.DType==SXMLEntity::EType::StartElement && TempEntity.DNameData=="way"){
                        break;
                    }
                }
                DWaysByIndex.push_back(NewWay);
                DWaysByID[WayID]=NewWay;
            }
            if(!src->ReadEntity(TempEntity)){ //if done then stop
                break;
            }
        }
        return true;
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
        return DNodesByIndex.size();
    }

    std::size_t WayCount() const noexcept{
        return DWaysByIndex.size();
    }
    
    std::shared_ptr<CStreetMap::SNode> NodeByIndex(std::size_t index) const noexcept{
        if (index>=DNodesByIndex.size()){
            return nullptr;
        }
        return DNodesByIndex[index];
    }
    
    std::shared_ptr<CStreetMap::SNode> NodeByID(TNodeID id) const noexcept{
        auto iN=DNodesByID.find(id);
        if(iN==DNodesByID.end()){
            return nullptr;
        }
        return iN->second;
    }
    
    std::shared_ptr<CStreetMap::SWay> WayByIndex(std::size_t index) const noexcept{
        if (index>=DWaysByIndex.size()){
            return nullptr;
        }
        return DWaysByIndex[index];
    }
    
    std::shared_ptr<CStreetMap::SWay> WayByID(TWayID id) const noexcept{
        auto iW=DWaysByID.find(id);
        if(iW==DWaysByID.end()){
            return nullptr;
        }
        return iW->second;
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
