#include "OpenStreetMap.h"
#include <unordered_map>

/**
 * Implementation of OpenStreetMap using PIMPL pattern.
 * Parses OSM XML files to store and index geographical nodes and ways.
 */
struct COpenStreetMap::SImplementation{
    // String constants for XML element and attribute names
    const std::string DOSMTag = "osm";
    const std::string DNodeTag = "node";
    const std::string DNodeIDAttr = "id";
    const std::string DNodeLatAttr = "lat";
    const std::string DNodeLonAttr = "lon";

    // Internal representation of an OSM Node
    struct SNode: public CStreetMap::SNode{
        CStreetMap::TNodeID DID;
        CStreetMap::TLocation DLocation;
        std::vector<std::pair<std::string, std::string>> DAttributes; // Stores <k, v> tags
        
        ~SNode(){};
        TNodeID ID() const noexcept override { return DID; }
        TLocation Location() const noexcept override { return DLocation; }
        std::size_t AttributeCount() const noexcept override { return DAttributes.size(); }

        // Returns attribute key by index; returns empty string if out of bounds
        std::string GetAttributeKey(std::size_t index) const noexcept override{
            return (index < DAttributes.size()) ? DAttributes[index].first : "";
        }

        bool HasAttribute(const std::string &key) const noexcept override{
            for (const auto &kv : DAttributes) if (kv.first == key) return true;
            return false;
        }

        std::string GetAttribute(const std::string &key) const noexcept override{
            for (const auto &kv : DAttributes) if (kv.first == key) return kv.second;
            return "";
        }
    };

    // Internal representation of an OSM Way (a path of nodes)
    struct SWay: public CStreetMap::SWay{
        CStreetMap::TWayID DID;
        std::vector<TNodeID> DNodeIDs; // Ordered list of node references
        std::vector<std::pair<std::string, std::string>> DAttributes;
        
        ~SWay(){};
        TWayID ID() const noexcept override { return DID; }
        std::size_t NodeCount() const noexcept override { return DNodeIDs.size(); }

        TNodeID GetNodeID(std::size_t index) const noexcept override{
            return (index < DNodeIDs.size()) ? DNodeIDs[index] : CStreetMap::InvalidNodeID;
        }

        std::size_t AttributeCount() const noexcept override { return DAttributes.size(); }
        // ... (Attribute methods similar to SNode)
        std::string GetAttributeKey(std::size_t index) const noexcept override{
            return (index < DAttributes.size()) ? DAttributes[index].first : "";
        }
        bool HasAttribute(const std::string &key) const noexcept override{
            for (const auto &kv : DAttributes) if (kv.first == key) return true;
            return false;
        }
        std::string GetAttribute(const std::string &key) const noexcept override{
            for (const auto &kv : DAttributes) if (kv.first == key) return kv.second;
            return "";
        }
    };

    // Data storage for fast access
    std::vector<std::shared_ptr<SNode>> DNodesByIndex;
    std::unordered_map<TNodeID, std::shared_ptr<SNode>> DNodesByID;
    std::vector<std::shared_ptr<SWay>> DWaysByIndex; 
    std::unordered_map<TWayID, std::shared_ptr<SWay>> DWaysByID;

    /**
     * Parses the <node> section of the XML. 
     * If a <way> tag is encountered, it stops and passes the entity back via nextentity.
     */
    bool ParseNodes(std::shared_ptr<CXMLReader> src, SXMLEntity &nextentity){
        SXMLEntity TempEntity;

        while(src->ReadEntity(TempEntity)){
            // Transition: If we hit a 'way', nodes are finished
            if(TempEntity.DType == SXMLEntity::EType::StartElement && TempEntity.DNameData == "way") {
                nextentity = TempEntity;
                return true;
            }

            // Extract node attributes (ID, Latitude, Longitude)
            if(TempEntity.DType == SXMLEntity::EType::StartElement && TempEntity.DNameData == DNodeTag){
                auto NodeID = std::stoull(TempEntity.AttributeValue(DNodeIDAttr));
                auto NodeLat = std::stod(TempEntity.AttributeValue(DNodeLatAttr));
                auto NodeLon = std::stod(TempEntity.AttributeValue(DNodeLonAttr));
                
                auto NewNode = std::make_shared<SNode>();
                NewNode->DID = NodeID;
                NewNode->DLocation = std::make_pair(NodeLat, NodeLon);
                
                DNodesByIndex.push_back(NewNode);
                DNodesByID[NodeID] = NewNode;
            }
        }
        return true;
    }

    /**
     * Parses the <way> section of the XML.
     * Processes nested <nd> elements to build the path of Node IDs.
     */
    bool ParseWays(std::shared_ptr<CXMLReader> src, SXMLEntity &firstentity){
        SXMLEntity TempEntity;
        // Use the entity passed from ParseNodes if it's already a 'way'
        if(firstentity.DType == SXMLEntity::EType::StartElement && firstentity.DNameData == "way"){
            TempEntity = firstentity;
        }
        else{
            if(!src->ReadEntity(TempEntity)) return true;
        }

        while (true){
            if(TempEntity.DType == SXMLEntity::EType::StartElement && TempEntity.DNameData == "way"){
                auto WayID = std::stoull(TempEntity.AttributeValue("id"));
                auto NewWay = std::make_shared<SWay>();
                NewWay->DID = WayID;

                // Inner loop to find <nd> (node references) inside the way
                while (src->ReadEntity(TempEntity)){
                    if (TempEntity.DType == SXMLEntity::EType::StartElement && TempEntity.DNameData == "nd"){
                        auto RefID = std::stoull(TempEntity.AttributeValue("ref"));
                        NewWay->DNodeIDs.push_back(RefID);
                    }
                    else if (TempEntity.DType == SXMLEntity::EType::EndElement && TempEntity.DNameData == "way"){
                        break; // End of this way's definition
                    }
                }
                DWaysByIndex.push_back(NewWay);
                DWaysByID[WayID] = NewWay;
            }
            if(!src->ReadEntity(TempEntity)) break; // End of file
        }
        return true;
    }

    // Main entry point for XML parsing logic
    bool ParseOpenStreetMap(std::shared_ptr<CXMLReader> src){
        SXMLEntity TempEntity;
        if(src->ReadEntity(TempEntity)){
            // Validate root OSM tag
            if(TempEntity.DType == SXMLEntity::EType::StartElement && TempEntity.DNameData == DOSMTag){
                // Sequential parsing: Nodes come before Ways in OSM files
                if(ParseNodes(src, TempEntity)){
                    if(ParseWays(src, TempEntity)){
                        return true;
                    }
                }
            }
        }
        return false;
    }

    SImplementation(std::shared_ptr<CXMLReader> src){
        ParseOpenStreetMap(src);
    }

    // Getter methods providing O(1) access to geographical data
    std::size_t NodeCount() const noexcept { return DNodesByIndex.size(); }
    std::size_t WayCount() const noexcept { return DWaysByIndex.size(); }
    
    std::shared_ptr<CStreetMap::SNode> NodeByIndex(std::size_t index) const noexcept {
        return (index < DNodesByIndex.size()) ? DNodesByIndex[index] : nullptr;
    }
    
    std::shared_ptr<CStreetMap::SNode> NodeByID(TNodeID id) const noexcept {
        auto it = DNodesByID.find(id);
        return (it == DNodesByID.end()) ? nullptr : it->second;
    }
    
    std::shared_ptr<CStreetMap::SWay> WayByIndex(std::size_t index) const noexcept {
        return (index < DWaysByIndex.size()) ? DWaysByIndex[index] : nullptr;
    }
    
    std::shared_ptr<CStreetMap::SWay> WayByID(TWayID id) const noexcept {
        auto it = DWaysByID.find(id);
        return (it == DWaysByID.end()) ? nullptr : it->second;
    }
};

// Bridge implementations for public COpenStreetMap class
COpenStreetMap::COpenStreetMap(std::shared_ptr<CXMLReader> src){
    DImplementation = std::make_unique<SImplementation>(src);
}

COpenStreetMap::~COpenStreetMap(){}

std::size_t COpenStreetMap::NodeCount() const noexcept { return DImplementation->NodeCount(); }
std::size_t COpenStreetMap::WayCount() const noexcept { return DImplementation->WayCount(); }
std::shared_ptr<CStreetMap::SNode> COpenStreetMap::NodeByIndex(std::size_t index) const noexcept { return DImplementation->NodeByIndex(index); }
std::shared_ptr<CStreetMap::SNode> COpenStreetMap::NodeByID(TNodeID id) const noexcept { return DImplementation->NodeByID(id); }
std::shared_ptr<CStreetMap::SWay> COpenStreetMap::WayByIndex(std::size_t index) const noexcept { return DImplementation->WayByIndex(index); }
std::shared_ptr<CStreetMap::SWay> COpenStreetMap::WayByID(TWayID id) const noexcept { return DImplementation->WayByID(id); }