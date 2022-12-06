/* 
 * File:   ubase.cpp
 * Author: 张太红
 * 
 * Created on 2015年11月29日, 下午1:13
 */

#include <stdio.h>   // perror()
#include <stdlib.h>   // exit())
#include "catalog.h"
#include "query.h"
#include "unistd.h"
#include "sys/types.h"
using namespace std;

DB db;
Error error;

BufferManager *bufMgr;
RelCatalog *relCat;
AttrCatalog *attrCat;

JoinType JoinMethod;

int main(int argc, char **argv) {
    if (argc < 2) {
        cerr << "用法: " << argv[0] << " 数据库名称 [连接方法：NL|SM|HJ]" << endl;
        return 1;
    }

    if (chdir(argv[1]) < 0) {
        perror("chdir");
        exit(1);
    }

    JoinMethod = NLJoin; // 缺省的连接方法
    if (argc == 3) // 指定了其它连接方法
    {
        if (strcmp(argv[2], "SM") == 0) JoinMethod = SMJoin;
        else if (strcmp(argv[2], "HJ") == 0) JoinMethod = HashJoin;
    }

    // 创建缓存管理器

    bufMgr = new BufferManager(100);

    // 创建关系和属性数据字典
    Status status;
    relCat = new RelCatalog(status);
    if (status == OK)
        attrCat = new AttrCatalog(status);
    if (status != OK) {
        error.print(status);
        exit(1);
    }

    cout << endl << "欢迎使用UBASE数据库管理系统！" << endl;
    cout << "2015修订，版本号：v1.0" << endl;
    cout << "开发小组联系信息：" << endl;
    cout << "新疆农业大学计算机与信息工程学院" << endl;
    cout << "张太红，zth@xjau.edu.cn，13325538255" << endl;

    if (JoinMethod == NLJoin) {
        cout << "使用嵌套循环实现连接（Join）操作" << endl;
    } else
        if (JoinMethod == HashJoin) {
        cout << "使用散列技术实现连接（Join）操作" << endl;
    } else {
        cout << "使用归并排序实现连接（Join）操作" << endl;
    }

    extern void parse();
    parse();

    return 0;
}
