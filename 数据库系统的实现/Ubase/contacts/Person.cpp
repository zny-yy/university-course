#include "Person.h"

//hello
// 联系人类的构造函数
Person::Person(string name, string phone)
{
    this->name = name;
    this->phone = phone;
}

ostream& operator<<(ostream& os, const Person& p)
{
    return os << p.name << ":" << p.phone; 
}

string Person::getName(){
    return name;
}
Person::~Person(){
    
}
