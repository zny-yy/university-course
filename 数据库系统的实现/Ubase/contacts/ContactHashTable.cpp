/* 
 * 文件: ContactHashTable.cpp
 * 班级: XXXXXXXX
 * 学号: YYYYYYYY
 * 姓名: ZZZZZZZZ
 */

 
#include <iostream>
using namespace std;
#include "ContactHashTable.h"


// 构造函数，创建并初始化联系人散列表类

ContactHashTable::ContactHashTable() {

	// 联系人姓名的首个拼音字母共26个，所以散列表的桶数为26
	HTSIZE = 26;

	// 分配散列表指针数组
	ht = new contactNode* [HTSIZE];

	// 每个指针初始化为空指针
	for (size_t i = 0; i < HTSIZE; i++) ht[i] = NULL;
}


// 析构函数，回收联系人散列表

ContactHashTable::~ContactHashTable() {
	for (size_t i = 0; i < HTSIZE; i++) {
		contactNode* tmpNode = ht[i];
		while (ht[i]) {
			tmpNode = ht[i];
			ht[i] = ht[i]->next;
			// 删除联系人对象
			if (tmpNode->person != NULL) delete tmpNode->person;
			// 删除联系人节点
			delete tmpNode;
		}
	}
	// 删除联系人散列表
	delete [] ht;
}


// 按姓名计算散列表桶号

size_t ContactHashTable::hash(const string name) {

	//姓名拼音的首字母如果为A，该函数应当返回0，首字母如果为Z则返回25

	//获取第一个字的拼音首字母
	unsigned char firstPinyin = ucs4PinyinDict.lookUp(name);

	// 返回散列表桶号
	return (size_t) (firstPinyin - 'A');
}


// 添加联系人 头插法

void ContactHashTable::insert(Person*& person) {
	size_t index = hash(person->getName());
	
	// 创建一个新的联系人节点
        
        contactNode* newNode = new contactNode();
        newNode->person=person;
        newNode->next=NULL;
        contactNode* tmpNode = ht[index];
	/*
     *  在这里编写你的代码
	 *		 
	 */
	
	// 调整指针
	/*
     *  在这里编写你的代码
	 *		 
	 */
        
        if(tmpNode!=NULL){
            newNode->next=tmpNode;  
	    ht[index]=newNode;
        }
        else{
            tmpNode=newNode;
            ht[index]=tmpNode;
        }  
}

// 查找联系人

void ContactHashTable::find(const string name, Person*& person) {
	person = NULL;
    size_t index = hash(name);
    
	/*
     *  在这里编写你的代码
	 *		 
	 */
    contactNode* tmpNode = ht[index];
    while(tmpNode!=NULL){
        if(tmpNode->person->getName()==name){
            person=tmpNode->person;
            break;
        }
        tmpNode=tmpNode->next;
    }
    cout << "∅";
}


// 删除联系人

void ContactHashTable::erase(const string name) {
    size_t index = hash(name);
    contactNode* tmpNode = ht[index];
    contactNode* prevNode = ht[index];
    while (tmpNode!=NULL) {
        if (tmpNode->person->getName() == name) {
          /*
           *  在这里编写你的代码
	       *		 
	       */
            if(tmpNode->next!=NULL){
                 prevNode->next=tmpNode->next;
                 delete tmpNode->person;
                 delete tmpNode;
            }
            else {
                //prevNode->next=NULL;
                ht[index]=tmpNode->next;
                delete tmpNode->person;
                delete tmpNode;
            }
            return;
        }
        else {
        	
          /*
           *  在这里编写你的代码
	       *		 
	       */
            if(tmpNode->next!=NULL) tmpNode=tmpNode->next;
            else cout << "∅";
        }
    }
}


void ContactHashTable::listHashTable() {
	for (size_t i = 0; i < HTSIZE; i++) {
		cout << char(i + 'A') << "  ";
		cout << "ht[" << i << "]->";
		contactNode* tmpNode = ht[i];
		if (tmpNode)
			cout << tmpNode;
		else
			cout << "∅";

		while (tmpNode) {
			cout << "->" << *tmpNode->person;
			tmpNode = tmpNode->next;
			cout << "->";
			if (tmpNode)
				cout << tmpNode;
			else
				cout << "∅";
		}

		cout << endl;
	}
}
