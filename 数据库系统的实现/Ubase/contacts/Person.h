/* 
 * File:   Person.h
 * Author: zhangtaihong
 *
 * Created on 2015年10月19日, 下午5:56P
 */

#ifndef PERSON_H
#define PERSON_H

#include <iostream>  // endl等
using namespace std;

class Person {
    friend ostream& operator<<(ostream& os, const Person& p); //重载<<操作符函数
public:
    Person(string name, string phone); // 联系人构造函数
    ~Person(); // 联系人析构函数
    string getName();
private:
    string name;
    string phone;
};

#endif /* PERSON_H */