/* 
 * File:   query.cpp
 * Author: 张太红
 * 
 * Created on 2015年12月12日, 下午1:13
 */

#include "query.h"
/*
 * **************************************************************************
 * 查询操作的实现
 * **************************************************************************
 */
// 扫描选择操作函数的声明，QU_Select将使用此函数完成查询操作

const Status ScanSelect(const string & result,
        const int projCnt,
        const AttrDesc projNames[],
        const AttrDesc *attrDesc,
        const Operator op,
        const char *filter,
        const int reclen) {

    // 在这里开始编写你的代码

    return OK;
}

/*
 * 从指定关系（表）查询记录
 *
 * 返回:
 * 	成功：OK
 * 	否则：错误代码
 */
const Status QU_Select(const string & result,
        const int projCnt,
        const attrInfo projNames[],
        const attrInfo *attr,
        const Operator op,
        const char *attrValue) {

    // 在这里开始编写你的代码

    return OK;
}



/*
 * **************************************************************************
 * 连接操作的实现
 * **************************************************************************
 */
extern JoinType JoinMethod;

/* 记录匹配函数 */
const int matchRec(const Record & outerRec,
        const Record & innerRec,
        const AttrDesc & attrDesc1,
        const AttrDesc & attrDesc2) {
    int tmpInt1, tmpInt2;
    float tmpFloat1, tmpFloat2;

    switch (attrDesc1.attrType) {
        case INTEGER:
            memcpy(&tmpInt1, (char *) outerRec.data + attrDesc1.attrOffset, sizeof (int));
            memcpy(&tmpInt2, (char *) innerRec.data + attrDesc2.attrOffset, sizeof (int));
            return tmpInt1 - tmpInt2;

        case FLOAT:
            memcpy(&tmpFloat1, (char *) outerRec.data + attrDesc1.attrOffset, sizeof (float));
            memcpy(&tmpFloat2, (char *) innerRec.data + attrDesc2.attrOffset, sizeof (float));
            return int(tmpFloat1 - tmpFloat2);

        case STRING:
            return strcmp((char *) outerRec.data + attrDesc1.attrOffset,
                    (char *) innerRec.data + attrDesc2.attrOffset);
    }
    return 0;
}

/* 用嵌套循环算法实现两个表的连接操作 */
const Status QU_NL_Join(const string & result,
        const int projCnt,
        const attrInfo projNames[],
        const attrInfo *attr1,
        const Operator op,
        const attrInfo *attr2) {
    
    cout << "进行嵌套循环连接" << endl;

    // 在这里开始编写你的代码
    // Status status;
    return OK;
}

/* 用排序归并算法实现两个表的连接操作 */
const Status QU_SM_Join(const string & result,
        const int projCnt,
        const attrInfo projNames[],
        const attrInfo *attr1,
        const Operator op,
        const attrInfo *attr2) {
    
    cout << "进行排序归并连接" << endl;

    // 在这里开始编写你的代码
    // Status status;
    return OK;
}

/* 用散列分区算法实现两个表的连接操作 */
const Status QU_Hash_Join(const string & result,
        const int projCnt,
        const attrInfo projNames[],
        const attrInfo *attr1,
        const Operator op,
        const attrInfo *attr2) {
    cout << "进行散列分区连接" << endl;

    // 在这里开始编写你的代码
    // Status status;
    return OK;
}

/* 两个表的连接操作 */
const Status QU_Join(const string & result,
        const int projCnt,
        const attrInfo projNames[],
        const attrInfo *attr1,
        const Operator op,
        const attrInfo *attr2) {
    switch (JoinMethod) {
        case NLJoin:
            return QU_NL_Join(result, projCnt, projNames, attr1, op, attr2);
            break;
        case SMJoin:
            QU_SM_Join(result, projCnt, projNames, attr1, op, attr2);
            break;
        case HashJoin:
            QU_Hash_Join(result, projCnt, projNames, attr1, op, attr2);
            break;
    }
    return OK;
}


/*
 * **************************************************************************
 * 插入操作的实现
 * **************************************************************************
 */

/*
 * 向指定关系（表）插入一条记录
 *
 * 返回:
 * 	成功：OK
 * 	否则：错误代码
 */

const Status QU_Insert(const string &relation, const int attrCnt, const attrInfo attrList[]) {
    if (relation.empty()) return BADCATPARM;

    // 在这里开始编写你的代码
    // Status status;
    return OK;
}


/*
 * **************************************************************************
 * 删除操作的实现
 * **************************************************************************
 */

/*
 * 删除指定关系（表）中的一条记录
 *
 * 返回:
 * 	成功：OK
 * 	否则：错误代码
 */

const Status QU_Delete(const string &relation,
        const string &attrName,
        const Operator op,
        const Datatype type,
        const char *attrValue) {
    
    if (relation.empty()) return BADCATPARM;

    // 在这里开始编写你的代码
    // Status status;
    return OK;
}

/*
 * **************************************************************************
 * 更新操作的实现
 * **************************************************************************
 */

/*
 * 更新指定关系（表）中的一条记录
 *
 * 返回:
 * 	成功：OK
 * 	否则：错误代码
 */

const Status QU_Update(const string & relation,
        const int attrCnt,
        const attrInfo attrList[],
        const string & attrName,
        const Operator op,
        const Datatype type,
        const char *attrValue) {

    if (relation.empty()) return BADCATPARM;

    // 在这里开始编写你的代码
    // Status status;
    return OK;
}