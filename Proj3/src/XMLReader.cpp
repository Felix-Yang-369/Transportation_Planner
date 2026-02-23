#include "XMLReader.h"
#include <expat.h>
#include <queue>

// <tag attr="val">CharData</tag>

struct CXMLReader::SImplementation{
    // Data source used to read XML text
    std::shared_ptr< CDataSource > DSource;
    // Expat XML parser
    XML_Parser DParser;
    // Queue to store parsed XML entities
    std::queue<SXMLEntity> DQueue;
    
    // Called when a start tag is encountered
    static void StartElementHandler(void *userData, const XML_Char *name, const XML_Char **atts){
        SImplementation *This = (SImplementation *)userData;
        SXMLEntity NewEntity;
        
        NewEntity.DType = SXMLEntity::EType::StartElement;
        NewEntity.DNameData = name;

        // Read attribute name/value pairs
        for(int i = 0; atts[i]; i += 2){
            NewEntity.DAttributes.push_back({atts[i],atts[i+1]});
        }
        // Store the entity for later reading
        This->DQueue.push(NewEntity);
    }

    // Called when an end tag is encountered
    static void EndElementHandler(void *userData, const XML_Char *name){
        SImplementation *This = (SImplementation *)userData;
        SXMLEntity NewEntity;
        
        NewEntity.DType = SXMLEntity::EType::EndElement;
        NewEntity.DNameData = name;
        // Store the entity
        This->DQueue.push(NewEntity);
    }

    // Called when character data is encountered
    static void CharacterDataHandler(void *userData, const XML_Char *s, int len){
        SImplementation *This = (SImplementation *)userData;
        SXMLEntity NewEntity;
        
        NewEntity.DType = SXMLEntity::EType::CharData;
        NewEntity.DNameData = std::string(s,len);
        // Store character data as an entity
        This->DQueue.push(NewEntity);
    }

    // Set up the XML parser and handlers
    SImplementation(std::shared_ptr< CDataSource > src){
        DSource = src;
        DParser = XML_ParserCreate(nullptr);
        XML_SetUserData(DParser,(void *)this);
        XML_SetStartElementHandler(DParser,StartElementHandler);
        XML_SetEndElementHandler(DParser,EndElementHandler);
        XML_SetCharacterDataHandler(DParser,CharacterDataHandler);

    }

    // Check if reader has reached the end
    bool End() const{
        return true;
    }

    // Read the next XML entity
    bool ReadEntity(SXMLEntity &entity, bool skipcdata){

        // If no parsed entities are available, read more data
        if(DQueue.empty()){
            std::vector<char> Buffer(512);
            DSource->Read(Buffer,Buffer.size());
            XML_Parse(DParser,Buffer.data(),Buffer.size(),DSource->End());
        }
        // Return the next available entity
        if(!DQueue.empty()){
            entity = DQueue.front();
            DQueue.pop();
            return true;
        }

        return false;
    }

};

        
// Create an XML reader using a data source
CXMLReader::CXMLReader(std::shared_ptr< CDataSource > src){
    DImplementation = std::make_unique<SImplementation>(src);
}
// Destructor
CXMLReader::~CXMLReader(){

}
// Check if reader is at the end
bool CXMLReader::End() const{
    return DImplementation->End();
}
// Read the next XML entity
bool CXMLReader::ReadEntity(SXMLEntity &entity, bool skipcdata){
    return DImplementation->ReadEntity(entity,skipcdata);
}
