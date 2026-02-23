#include "XMLWriter.h"
#include <vector>
#include <string>

struct CXMLWriter::SImplementation{
    // Where XML output is written to
    std::shared_ptr<CDataSink> DSink;

    // Save the data sink
    SImplementation(std::shared_ptr<CDataSink> sink){
        DSink = sink;
    }

    // Write a string to the sink
    void WriteString(const std::string &str){
        std::vector<char> Buffer(str.begin(), str.end());
        DSink->Write(Buffer);
    }

    // Write a single XML entity
    bool WriteEntity(const SXMLEntity &entity){
        // Handle start tag
        if(entity.DType == SXMLEntity::EType::StartElement){
            DSink->Put('<');
            WriteString(entity.DNameData);

            // Write all attributes
            for(const auto &Attr : entity.DAttributes){
                DSink->Put(' ');
                WriteString(Attr.first);
                DSink->Put('=');
                DSink->Put('\"');
                WriteString(Attr.second);
                DSink->Put('\"');
            }

            DSink->Put('>');
            return true;
        }
        // Handle end tag
        else if(entity.DType == SXMLEntity::EType::EndElement){
            DSink->Put('<');
            DSink->Put('/');
            WriteString(entity.DNameData);
            DSink->Put('>');
            return true;
        }
        // Handle character data
        else if(entity.DType == SXMLEntity::EType::CharData){
            WriteString(entity.DNameData);
            return true;
        }

        return false;
    }
};

// Create a writer with a data sink
CXMLWriter::CXMLWriter(std::shared_ptr<CDataSink> sink){
    DImplementation = std::make_unique<SImplementation>(sink);
}

// Destructor
CXMLWriter::~CXMLWriter() = default;

// Write an XML entity using the implementation
bool CXMLWriter::WriteEntity(const SXMLEntity &entity){
    return DImplementation->WriteEntity(entity);
}