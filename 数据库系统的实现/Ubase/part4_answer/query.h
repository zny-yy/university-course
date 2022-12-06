/* 
 * File:   query.h
 * Author: 张太红
 * 
 * Created on 2015年11月29日, 下午1:13
 */

#ifndef QUERY_H
#define QUERY_H

#include "heapfile.h"
#include "catalog.h"

enum JoinType {
    NLJoin, SMJoin, HashJoin
};

//
// 查询层函数原型声明
//

// SELECT：支持简单查询的SQL语句（单表查询）
const Status QU_Select(const string & result,
        const int projCnt,
        const attrInfo projNames[],
        const attrInfo *attr,
        const Operator op,
        const char *attrValue);

// SELECT：支持连接查询的SQL语句（多表查询）
const Status QU_Join(const string & result,
        const int projCnt,
        const attrInfo projNames[],
        const attrInfo *attr1,
        const Operator op,
        const attrInfo *attr2);

// INSERT：支持插入SQL语句
const Status QU_Insert(const string & relation,
        const int attrCnt,
        const attrInfo attrList[]);

// DELETE：支持删除SQL语句
const Status QU_Delete(const string & relation,
        const string & attrName,
        const Operator op,
        const Datatype type,
        const char *attrValue);

// UPDATE：支持更新SQL语句
const Status QU_Update(const string & relation,
        const int attrCnt,
        const attrInfo attrList[],
        const string & attrName,
        const Operator op,
        const Datatype type,
        const char *attrValue);
#endif
