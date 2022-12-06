/* 
 * File:   contactHashTable.h
 * Author: zhangtaihong
 *
 * Created on 2015年10月19日, 下午6:53
 */

#ifndef CONTACTHASHTABLE_H
#define	CONTACTHASHTABLE_H

#include "Person.h"
#include "Dictionary.h"
// 联系人节点结构
struct contactNode {
    Person* person;          // 指向联系人对象的指针
    contactNode* next;       // 指向下一个联系人节点的指针

};

class ContactHashTable {
public:
    ContactHashTable();              // 联系人散列表类的构造函数
    virtual~ContactHashTable();      // 联系人散列表类的析构函数
    
    // 向散列表插入一个节点
    void insert(Person* &person);

    // 在散列表中查找名为name的联系人是否存在，如果存在则通过参数person返回该联系人对象指针
    void find(const string name, Person* &person);

    // 从散列表删除一个联系人
    void erase(const string name);
    
    // 输出散列表,用于测试
    void listHashTable();
      
private:
    size_t HTSIZE;             // 散列表桶数，
    contactNode** ht;          // 散列表，用指针数组实现
    size_t hash(string name);  // 散列函数，返回值在0和HTSIZE-1之间，用来计算散列表桶号
    Dictionary ucs4PinyinDict; // UCS4编码->拼音首字母字典

};

#endif	/* CONTACTHASHTABLE_H */

