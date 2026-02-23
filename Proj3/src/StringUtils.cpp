#include "StringUtils.h"
#include <cctype>

namespace StringUtils{

std::string Slice(const std::string &str, ssize_t start, ssize_t end) noexcept{
    // Replace code here
    int len=str.length();
    std::string Temp=str;
    if (end<=0) {
        end=end+len;
    }
    if (start<0) {
        start=start+len;
    }
    if (end>len) {
        end=len;
    }
    if (start>len) {
        start=len;
    }
    if (start>=end) {
        Temp="";
    }
    else {
        Temp=str.substr(start, end-start);
    }
    return Temp;
}

std::string Capitalize(const std::string &str) noexcept{
    // Replace code here
    std::string Temp = str;
    if(!Temp.empty()){
        Temp[0] = toupper(Temp[0]);
        for(size_t Index = 1; Index < Temp.length(); Index++){
            Temp[Index] = tolower(Temp[Index]);
        }
    }
    return Temp;
}

std::string Upper(const std::string &str) noexcept{
    // Replace code here
    std::string Temp=str;
    if(!Temp.empty()) {
        for(size_t Index=0; Index<Temp.length(); Index++) {
            Temp[Index]=toupper(Temp[Index]);
        }
    }
    return Temp;
}

std::string Lower(const std::string &str) noexcept{
    // Replace code here
    std::string Temp=str;
    if(!Temp.empty()) {
        for(size_t Index=0; Index<Temp.length(); Index++) {
            Temp[Index]=tolower(Temp[Index]);
        }
    }
    return Temp;
}

std::string LStrip(const std::string &str) noexcept{
    // Replace code here
    std::string Temp = str;
    size_t Index = 0;
    size_t Length = Temp.length();
    while((Index < Length) && isspace(Temp[Index])){
        Index++;
    }

    return Temp.substr(Index);
}

std::string RStrip(const std::string &str) noexcept{
    // Replace code here
    std::string Temp = str;
    if(!Temp.empty()){
        size_t Index = Temp.length()-1;
        while(isspace(Temp[Index])){
            if(!Index){
                return "";
            }
            Index--;
        }
        return Temp.substr(0,Index+1);
    }
    return "";
}

std::string Strip(const std::string &str) noexcept{
    // Replace code here
    return LStrip(RStrip(str));
}

std::string Center(const std::string &str, int width, char fill) noexcept{
    // Replace code here
    std::string Temp=str;
    int len=str.length();
    if (width<=len) {
        Temp=str;
    }
    else {
        if ((width-len)%2==0) {
            Temp=std::string((width-len)/2, fill)+str+std::string((width-len)/2, fill);
        }
        else {
            Temp=std::string((width-len)/2, fill)+str+std::string((width-len)/2+1, fill);
        }
    }

    return Temp;
}

std::string LJust(const std::string &str, int width, char fill) noexcept{
    // Replace code here
    std::string Temp=str;
    int len=str.length();
    if (width<=len) {
        Temp=str;
    }
    else {
        Temp=str+std::string(width-len, fill);
    }
    return Temp;
}

std::string RJust(const std::string &str, int width, char fill) noexcept{
    // Replace code here
    std::string Temp=str;
    int len=str.length();
    if (width<=len) {
        Temp=str;
    }
    else {
        Temp=std::string(width-len, fill)+str;
    }
    return Temp;
}

std::string Replace(const std::string &str, const std::string &old, const std::string &rep) noexcept{
    // Replace code here
    std::string Temp="";
    int len=str.length();
    int index=0;
    if (str!="") {
        while(index<str.length()){
            if (str.compare(index, old.length(), old)!=0) {
                Temp+=str[index];
                index+=1;
            }
            else {
                Temp+=rep;
                index+=old.length();
            }
        }
    }
        
    return Temp;
}

std::vector< std::string > Split(const std::string &str, const std::string &splt) noexcept{
    // Replace code here
    int len=str.length();
    if (splt=="") {
        std::vector<std::string> Temp;
        int index=0;
        if (str=="") {
            Temp.push_back("");
            return Temp;
        }
        while (index<len) {
            while (index<len && isspace((unsigned char)str[index])) {
                index+=1;
            }
            if (index>=len) {
                break;
            }
            int start=index;
            while (index<len && !isspace((unsigned char)str[index])) {
                index+=1;
            }
            Temp.push_back(str.substr(start, index-start));
        }
        return Temp;

    }
    std::vector<std::string> Temp(1);
    int index =0;
    int num=0;
    if (str!="") {
        while(index<len){
            if (str.compare(index, splt.length(),splt)==0) {
                Temp.push_back("");
                num+=1;
                index+=splt.length();
            }
            else {
                Temp[num]+=str[index];
                index+=1;
            }
        }
    }
    return Temp;
}

std::string Join(const std::string &str, const std::vector< std::string > &vect) noexcept{
    // Replace code here
    std::string Temp;
    for (int i=0; i<vect.size(); i++) {
        if (i>0) {
            Temp+=str;
        }
        Temp +=vect[i];
    }
    return Temp;
}

std::string ExpandTabs(const std::string &str, int tabsize) noexcept{
    // Replace code here
    std::string Temp;
    int len=str.length();
    int index=0;
    for (int i=0; i<len; i++) {
        if (str[i]=='\t') {
            int spa=tabsize-(index%tabsize);
            Temp.append(spa,' ');
            index+=spa;
        }
        else {
            Temp+=str[i];
            index+=1;
        }
    }


    return Temp;
}

int EditDistance(const std::string &left, const std::string &right, bool ignorecase) noexcept{
    // Replace code here
    int a=left.length();
    int b=right.length();
    std::vector<int> prev(b+1);
    std::vector<int> cur(b+1);
    for(int j=0; j<=b; j++) {
        prev[j]=j;
    }
    for (int i=1; i<=a; i++) {
        cur[0]=i;
        for (int j=1; j<=b; j++) {
            char ltem=left[i-1];
            char rtem=right[j-1];
            if (ignorecase==true) {
                ltem=(char)tolower((unsigned char)ltem);
                rtem=(char)tolower((unsigned char)rtem);
            }
            int cost;
            if (ltem==rtem) {
                cost = 0;
            } else {
                cost = 1;
            }
            int del=prev[j]+1;
            int ins=cur[j-1]+1;
            int rep=prev[j-1]+cost;
            cur[j]=std::min(std::min(del, ins), rep);

        }
        prev=cur;
    }

    return prev[b];
}

};