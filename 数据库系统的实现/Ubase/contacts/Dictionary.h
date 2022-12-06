/* 
 * File:   Dictionary.h
 * Author: zhangtaihong
 *
 * Created on 2015年10月20日, 上午3:20
 */

#ifndef DICTIONARY_H
#define DICTIONARY_H


#include <sstream>
#include <string>
#include <fstream>
#include <map>

using namespace std;

typedef map<unsigned int, unsigned char> Dict;

class Dictionary {
    friend class ContactHashTable;
public:
    Dictionary();
    virtual ~Dictionary();
    unsigned char lookUp(const string name);

private:
    Dict dict;
    unsigned int utf8ToUcs4(const string name);
};

#endif /* DICTIONARY_H */

