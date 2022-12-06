/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Dictionary.cpp
 * Author: zhangtaihong
 * 
 * Created on 2015年10月20日, 上午3:20
 */
#include <iostream>                      // cout
#include "Dictionary.h"

Dictionary::Dictionary() {
    ifstream infile("word.dat");          // 打开文件
    string line;
    while (getline(infile, line)) {       // 逐行处理
        istringstream iss(line);
        unsigned int  a;
        unsigned char  b[100];
        if (!(iss >> hex >> a >> b)) {
            break;
        }
        dict[a] = b[0];
    }
    infile.close();
}


Dictionary::~Dictionary() {
}

unsigned char Dictionary::lookUp(const string name){
    int code = utf8ToUcs4(name);
    return dict[code];
}

unsigned int Dictionary::utf8ToUcs4(const string name){
    unsigned char firstByte = name[0];
    int UCS4 = 0;              // 保存姓名第一个字的UCS4编码
    int utf8CharLen = 0;       // 第一个字的UTF-8的字节数

    // 计算第一个字的UCS4编码
    if (firstByte < 0x80){     // 首字节为0xxxxxxx，1字节编码,即ASCII
        utf8CharLen = 1;
        UCS4 = firstByte;
    }
    else if(firstByte < 0xe0){ // 首字节为110xxxxx，2字节编码
        utf8CharLen = 2;
        UCS4 = firstByte & 0x1f;
        
    }
    else if(firstByte < 0xf0){ // 首字节为1110xxxx，3字节编码
        utf8CharLen = 3;
        UCS4 = firstByte & 0x0f;
        
    }
    else if(firstByte < 0xf8){ // 首字节为11110xxx，4字节编码
        utf8CharLen = 4;
        UCS4 = firstByte & 7;
    }
    else if(firstByte < 0xfc){ // 首字节为111110xx，5字节编码
        utf8CharLen = 5;
        UCS4 = firstByte & 3;
    }
    else{                     // 首字节为1111110x，6字节编码
        utf8CharLen = 6;
        UCS4 = firstByte & 1;
    }
     
    for(int i = 1; i< utf8CharLen; i++){
        char b = name[i];
        UCS4 = (UCS4 << 6) + (b & 0x3f);
    }
    return UCS4;
}