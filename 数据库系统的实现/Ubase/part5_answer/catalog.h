/* 
 * File:   catalog.h
 * Author: 张太红
 * 
 * Created on 2015年11月29日, 下午1:13
 */

#ifndef CATALOG_H
#define CATALOG_H
#include <stdlib.h>  // atoi()、qsort()等
#include <fcntl.h>   // open()、O_RDONLY等
#include "heapfile.h"
#include "error.h"


//
// 实用工具（utility）层函数原型声明
//
const Status UT_Load(const string & relation, const string & fileName);

const Status UT_Print(string relation);

void UT_Quit(void);



#define RELCATNAME   "relcat"           // 表（关系）字典名称
#define ATTRCATNAME  "attrcat"          // 字段（属性）字典名称
#define MAXNAME      32                 // 表名称、字段名称长度
#define MAXSTRINGLEN 255                // 字符型字段的最大长度


// 表（关系）字典的结构:
//   表（关系）名称 　: char(32)           <-- 查找键（主键）
//   字段（属性）数量 : integer(4)

typedef struct {
    char relName[MAXNAME]; // 表（关系）名称
    int attrCnt; // 字段（属性）数量
} RelDesc;

typedef struct {
    char relName[MAXNAME]; // 表（关系）名称
    char attrName[MAXNAME]; // 字段（属性）名称
    int attrType; // 字段类型：INTEGER、FLOAT或STRING
    int attrLen; // 字段长度（以字节为单位）
    void *attrValue; // 指向二进制值的指针
} attrInfo;

class RelCatalog : public HeapFile {
public:
    // 打开表（关系）字典
    RelCatalog(Status &status);

    // 获取一个表（关系）的描述信息
    const Status getInfo(const string & relation, RelDesc& record);

    // 向表（关系）字典添加记录
    const Status addInfo(RelDesc & record);

    // 从表（关系）字典删除记录
    const Status removeInfo(const string & relation);

    // 创建一个新表（关系）
    const Status createRel(const string & relation,
            const int attrCnt,
            const attrInfo attrList[]);

    // 删除一个表（关系）
    const Status destroyRel(const string & relation);

    // 输出表（关系）的结构信息
    const Status help(const string & relation); // relation可以空

    // 关闭表（关系）字典
    ~RelCatalog();
};


// 字段（属性）字典的结构:
//   表（关系）名称 　: char(32)           <-- 查找键（主键）
//   字段（属性）名称 : char(32)         　<--
//   字段（属性）偏移 : integer(4)
//   字段（属性）类型 : integer(4)(就是Datatype枚举类型的一个值，在heapfile.h文件中定义过)
//   字段（属性）长度 : integer(4)

typedef struct {
    char relName[MAXNAME]; // 表（关系）名称
    char attrName[MAXNAME]; // 字段（属性）名称
    int attrOffset; // 字段（属性）偏移
    int attrType; // 字段（属性）类型
    int attrLen; // 字段（属性）长度
} AttrDesc;

class AttrCatalog : public HeapFile {
    friend class RelCatalog;

public:
    // 打开字段（属性）字典
    AttrCatalog(Status &status);

    // 从字段（属性）字典获取一条记录
    const Status getInfo(const string & relation,
            const string & attrName,
            AttrDesc &record);

    // 向字段（属性）字典添加一条记录
    const Status addInfo(AttrDesc & record);

    // 从字段（属性）字典删除一条记录
    const Status removeInfo(const string & relation, const string & attrName);

    // 从字段（属性）字典获取一个表的所用字段信息
    const Status getRelInfo(const string & relation,
            int &attrCnt,
            AttrDesc *&attrs);

    // 从字段（属性）字典删除一个表的所用字段信息
    const Status dropRelation(const string & relation);

    // 关闭字段（属性）字典
    ~AttrCatalog();
};


extern RelCatalog *relCat;
extern AttrCatalog *attrCat;
extern Error error;
extern Status createHeapFile(const string filename);
extern Status destroyHeapFile(const string filename);

#endif
