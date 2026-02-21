#include "DSVReader.h"

struct CDSVReader::SImplementation{
    std::shared_ptr<CDataSource> DSource;
    char DDelimiter;

    SImplementation(std::shared_ptr< CDataSource > src, char delimiter){
        DSource = src;
        DDelimiter = delimiter;
    }

    bool ParseValue(std::string &val){
        bool InQuotes = false;
        val.clear();

        while(!DSource->End()){
            char NextChar;
            DSource->Peek(NextChar);
            if(!InQuotes &&((NextChar == DDelimiter)||(NextChar == '\n'))){
                return true;
            }
            if(NextChar == '\"' && !InQuotes){
                InQuotes = true;
            }
            else{
                val += std::string(1,NextChar);
            }
        }
    }

    bool End() const{
        return false;
    }

    bool ReadRow(std::vector<std::string> &row){
        while(!DSource->End()){
            std::string NextValue;
            if(ParseValue(NextValue)){
                row.push_back(NextValue);
                char NextChar;
                DSource->Peek(NextChar);
                if(NextChar == '\n'){
                    // consume
                    return true;
                }
            }
        }
        return false;
    }

};
        
CDSVReader::CDSVReader(std::shared_ptr< CDataSource > src, char delimiter){
    DImplementation = std::make_unique<SImplementation>(src,delimiter);
}

CDSVReader::~CDSVReader(){

}

bool CDSVReader::End() const{

}

bool CDSVReader::ReadRow(std::vector<std::string> &row){

}
