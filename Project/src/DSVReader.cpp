#include "DSVReader.h"

struct CDSVReader::SImplementation{
    // Where we read characters from
    std::shared_ptr<CDataSource> DSource;
    // Character used to separate values
    char DDelimiter;

    // Save the data source and delimiter
    SImplementation(std::shared_ptr< CDataSource > src, char delimiter){
        DSource = src;
        DDelimiter = delimiter;
    }
    // Read one value from the data source
    bool ParseValue(std::string &val){
        bool InQuotes = false;
        val.clear();
        // Keep reading characters until we hit a delimiter or newline
        while(!DSource->End()){
            char NextChar;
            DSource->Peek(NextChar);
            // Stop if we see delimiter or newline outside quotes
            if(!InQuotes &&((NextChar == DDelimiter)||(NextChar == '\n'))){
                return true;
            }
            // Actually consume the character
            DSource->Get(NextChar);

            // Toggle quote state when seeing a quote
            if(NextChar == '\"') {
                if (!InQuotes) {
                    InQuotes =true;
                }
                else{
                    InQuotes =false;
                }
            }
            else{
                // Add normal characters to the value
                val.push_back(NextChar);
            }

        }
        // Return true if we read something before hitting the end
        return !val.empty();
    }
    
    // Check if there is no more data to read
    bool End() const{
        return DSource->End();
    }
    // Read one full row of values
    bool ReadRow(std::vector<std::string> &row){
        row.clear();
        while(!DSource->End()){
            std::string NextValue;
            if(ParseValue(NextValue)){
                row.push_back(NextValue);
                // If we reached the end, return what we have
                if (DSource->End()){
                    return true;
                }
                char NextChar;
                DSource->Peek(NextChar);
                // Skip over the delimiter
                if (NextChar==DDelimiter) {
                    DSource->Get(NextChar);
                    continue;
                }
                // Skip newline and finish this row
                if(NextChar == '\n'){
                    DSource->Get(NextChar);
                    return true;
                }
            }
        }
        // Return true if at least one value was read
        return !row.empty();
    }

};
  
// Create a reader with a data source and delimiter
CDSVReader::CDSVReader(std::shared_ptr< CDataSource > src, char delimiter){
    DImplementation = std::make_unique<SImplementation>(src,delimiter);
}

// Destructor
CDSVReader::~CDSVReader(){

}

// Check if reader is at the end
bool CDSVReader::End() const{
    return DImplementation->End();
}
// Read a row using the internal implementation
bool CDSVReader::ReadRow(std::vector<std::string> &row){
    return DImplementation->ReadRow(row);
}

