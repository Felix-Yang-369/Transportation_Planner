#include "DSVWriter.h"

struct CDSVWriter::SImplementation{
    // Where output characters are written to
    std::shared_ptr< CDataSink > DSink;
    // Character used to separate values
    char DDelimiter;
    // Whether all values should be quoted (not used yet)
    bool DQuoteAll;

    // Save sink, delimiter, and quote option
    SImplementation(std::shared_ptr< CDataSink > sink, char delimiter, bool quoteall){
        DSink = sink;
        DDelimiter = delimiter;
        DQuoteAll = quoteall;
    }

    // Destructor
    ~SImplementation(){

    }

    // Write a single row of values to the data sink
    bool WriteRow(const std::vector<std::string> &row){
        bool First = true;
        // Go through each value in the row
        for(auto &Column : row){
            // Convert string to a character buffer
            std::vector<char> Buffer(Column.begin(),Column.end());
            // Write delimiter before every value except the first
            if(!First){
                DSink->Put(DDelimiter);
            }
            First = false;
            // Write the value characters to the sink
            DSink->Write(Buffer);
        }
        return true;
    }

};
// Create a writer with a data sink and delimiter
CDSVWriter::CDSVWriter(std::shared_ptr< CDataSink > sink, char delimiter, bool quoteall){
    DImplementation = std::make_unique<SImplementation>(sink,delimiter,quoteall);
}
// Destructor
CDSVWriter::~CDSVWriter(){

}
// Write a row using the internal implementation
bool CDSVWriter::WriteRow(const std::vector<std::string> &row){
    return DImplementation->WriteRow(row);
}
