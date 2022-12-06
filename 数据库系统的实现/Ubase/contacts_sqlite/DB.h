/* 
 * File:   DB.h
 * Author: 张太红
 *
 * Created on 2015年10月23日, 下午5:13
 */

#ifndef DB_H
#define	DB_H
#include "sqlite3.h"
#include <istream>     // endl
#include <iostream>    // cout

//using namespace std;

class DB {
public:
    // 构造函数
    DB(const std::string dbName);
    // 析构函数
    virtual ~DB();
    // 创建表
    void createTable(const std::string ddlSQL);
    // 添加联系人
    void add(const std::string name, const std::string phone);
    // 修改联系人
    void update(const std::string name, const std::string newName, const std::string newPhone);
    // 删除联系人
    void remove(const std::string name);
    // 查找联系人
    void find(const std::string name);
    // 显示全部联系人
    void listAll();

private:
    // 数据库对象指针
    sqlite3* database;
    // 执行SQL语句
    int exeSQL(const std::string sql);
    // 判断表是否已经存在
    bool tableExist(const std::string tableName);
};

#endif	/* DB_H */