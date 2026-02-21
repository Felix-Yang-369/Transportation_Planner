#include "DSVWriter.h"

struct CDSVWriter::SImplementation{
    std::shared_ptr< CDataSink > DSink;
    char DDelimiter;
    bool DQuoteAll;

    SImplementation(std::shared_ptr< CDataSink > sink, char delimiter, bool quoteall){
        DSink = sink;
        DDelimiter = delimiter;
        DQuoteAll = quoteall;
    }

    ~SImplementation(){

    }

    bool WriteRow(const std::vector<std::string> &row){
        bool First = true;
        for(auto &Column : row){
            std::vector<char> Buffer(Column.begin(),Column.end());
            if(!First){
                DSink->Put(DDelimiter);
            }
            First = false;
            DSink->Write(Buffer);
        }
        return true;
    }

};

CDSVWriter::CDSVWriter(std::shared_ptr< CDataSink > sink, char delimiter, bool quoteall){
    DImplementation = std::make_unique<SImplementation>(sink,delimiter,quoteall);
}

CDSVWriter::~CDSVWriter(){

}

bool CDSVWriter::WriteRow(const std::vector<std::string> &row){
    return DImplementation->WriteRow(row);
}
