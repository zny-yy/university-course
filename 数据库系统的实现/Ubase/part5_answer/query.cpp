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

    Status status;
    Record rec,irec;
    RID rid;
    char buffer[reclen];

    int itemp;
    float ftemp;
    HeapFileScan * scan;
    scan = new HeapFileScan(projNames[0].relName,status);
    if (status != OK) return status;

    InsertFileScan * iscan;
    iscan = new InsertFileScan(result,status);
    irec.length=reclen;

    // 当where子句中的条件不为NULL
    if (filter) 
    {
        switch(attrDesc->attrType)
        {
        case INTEGER:
            itemp = atoi(filter);
            scan->startScan(attrDesc->attrOffset,
                            attrDesc->attrLen,
                            (Datatype)attrDesc->attrType,
                            (char*)&itemp,
                             op);
            break;
        case FLOAT:
            ftemp = atof(filter);
            scan->startScan(attrDesc->attrOffset,attrDesc->attrLen,(Datatype)attrDesc->attrType,(char*)&ftemp,op);
            break;
        case STRING:
            scan->startScan(attrDesc->attrOffset,attrDesc->attrLen,(Datatype)attrDesc->attrType,filter,op);
            break;
        }
    }
    else
    {
        scan->startScan(0,0,STRING,NULL,EQ);
    }
    if (status != OK) return status;

    while ((status = scan->scanNext(rid)) != FILEEOF)
    {
        if (status != OK) return status;
        status = scan->getRecord(rec);
        if (status != OK) return status;
        // 投影操作
        int offset = 0; //需投影属性的偏移量
        for (int i=0; i< projCnt; i++)
        {
            memcpy(buffer+offset,(char*)rec.data+projNames[i].attrOffset,projNames[i].attrLen);
            //memcpy((char*)irec.data+offset,(char*)rec.data+projNames[i].attrOffset,projNames[i].attrLen);
            offset += projNames[i].attrLen;   
        }
        //将记录插入到结果表
        irec.data = &buffer;
        status = iscan->insertRecord(irec,rid);
        if (status != OK) return status;
    }

    scan->endScan();
    delete scan;
    delete iscan;

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

    Status status;
    AttrDesc sprojNames[projCnt];
    AttrDesc sattrDesc;
    int reclen=0;
    
    //利用 attrCat->getInfo,将projNames[] 转换为sprojNames[projCnt] 
    //即，从attrInfo转换为AttrDesc
    for (int i=0; i<projCnt; i++)
    {
        status = attrCat->getInfo(projNames[i].relName,
                                  projNames[i].attrName,
                                  sprojNames[i]);
        if (status != OK) return status;
        reclen += sprojNames[i].attrLen;
    }
    // 当有WHERE子句时，attrValue 不为空
    // 利用attrCat->getInfo,将*attr转换为attrDesc
    // 即，从attrInfo转换为AttrDesc
    if (attrValue)
    {
        status = attrCat->getInfo(attr->relName,attr->attrName,sattrDesc);
        if (status != OK) return status;
    }

    status =  ScanSelect(result,
                         projCnt,
                         sprojNames,
                         &sattrDesc,
                         op,
                         attrValue,
                         reclen);
    if (status != OK) return status;

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

    Status status;
    AttrDesc pNames[projCnt];
    AttrDesc lattr;    // where子句中的左属性
    AttrDesc rattr;    // where子句中的右属性
    int reclen=0;      // 结果记录的长度

    // 利用attrCat->getInfo，将projNames[]转换为pNames[projCnt]
    // 即，将attrInfo转换为AttrDesc
    for (int i=0; i<projCnt; i++)
    {
        status = attrCat->getInfo(projNames[i].relName,projNames[i].attrName,pNames[i]);
        if (status != OK) return status;
        reclen += pNames[i].attrLen;
    }

    // 保存结果记录数据(newRec.data)的临时数组
    char buffer[reclen];

    // 利用attrCat->getInfo,将 *attr转换为sattrDesc
    // 即，将attrInfo转换为AttrDesc
    status = attrCat->getInfo(attr1->relName,attr1->attrName,lattr);
    if (status != OK) return status;
    status = attrCat->getInfo(attr2->relName,attr2->attrName,rattr);
    if (status != OK) return status;

    char filter[lattr.attrLen];
    Record outerrec,innerrec,newrec;
    RID rid;

    HeapFileScan * outerscan;
    outerscan = new HeapFileScan(lattr.relName,status);
    if (status != OK) return status;

    HeapFileScan * innerscan;

    InsertFileScan * iscan;
    iscan = new InsertFileScan(result,status);
    newrec.length=reclen;

    // 外关系无条件扫描（即，遍历整个关系）
    outerscan->startScan(0,0,STRING,NULL,EQ);
    if (status != OK) return status;
    int i;
    int j = 0; // 计算被选择的记录数
    int tmpInt=0;
    float tmpFloat=0;

    while ((status = outerscan->scanNext(rid)) != FILEEOF)
    {
        if (status != OK) return status;
        status = outerscan->getRecord(outerrec);
        if (status != OK) return status;

        memcpy(filter,(char*)outerrec.data+lattr.attrOffset,lattr.attrLen);

        innerscan = new HeapFileScan(rattr.relName,status);
        if (status != OK) return status;

        innerscan->startScan(rattr.attrOffset,rattr.attrLen,(Datatype)rattr.attrType,filter,op);
        while ((status = innerscan->scanNext(rid)) != FILEEOF)
        {
            if (status != OK) return status;
            status = innerscan->getRecord(innerrec);
            if (status != OK) return status;
            // 投影操作
            int offset = 0; // 被投影属性的偏移量
            for (i=0; i< projCnt; i++)
            {
                if (strcmp(pNames[i].relName,lattr.relName)==0)
                {
                    memcpy(buffer+offset,(char*)outerrec.data+pNames[i].attrOffset,pNames[i].attrLen);
                    //memcpy((char*)newrec.data+offset,(char*)outerrec.data+pNames[i].attrOffset,pNames[i].attrLen);
                }
                else
                {
                    memcpy(buffer+offset,(char*)innerrec.data+pNames[i].attrOffset,pNames[i].attrLen);
                    //memcpy((char*)newrec.data+offset,(char*)innerrec.data+pNames[i].attrOffset,pNames[i].attrLen);
                }
                offset += pNames[i].attrLen;
            }
            //insert the record into result table;
            newrec.data=&buffer;
            status = iscan->insertRecord(newrec,rid);
            if (status != OK) return status;
            j++;
        }
        innerscan->endScan();
        delete innerscan;

    }
    outerscan->endScan();
    delete outerscan;

    //innerscan->endScan();
    //delete innerscan;
 
    delete iscan;
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

const Status QU_Insert(const string &relation, 
                       const int attrCnt, 
                       const attrInfo attrList[]) {
    if (relation.empty()) return BADCATPARM;

    Status status;
    RelDesc rd;
    AttrDesc *ad;
    int attrno;

    //检查表是否存在
    status = relCat->getInfo(relation,rd);
    if(status!=OK) return status;

    //检查属性是否存在
    status = attrCat->getRelInfo(relation,attrno,ad);
    if(status!=OK) return status;

    if (attrno!=attrCnt) return BADCATPARM;

    //获得记录长度
    int reclen = 0;
    for(int i=0; i<attrno;i++)
      reclen += ad[i].attrLen;

    char buffer[reclen];
    Record dbrec;
    dbrec.length=reclen;
    int offset = 0;
    int j;

    // 按照AttrCat表中的属性顺序进行排列属性值
    for (int i=0; i < attrno; i++)
    {
        for (j=0; j < attrCnt; j++)
        {
            if (strncmp(ad[i].attrName, attrList[j].attrName, MAXNAME)==0) break;
        }

        switch(ad[i].attrType) {
        case INTEGER:
            //将void* 转换为 Integer
            int itemp;
            itemp = atoi((char*)attrList[j].attrValue);
            memcpy(buffer+offset,&itemp,ad[i].attrLen);
            break;
        case FLOAT:
            //将void* 转换为 Float
            float ftemp;
            ftemp = atof((char*)attrList[j].attrValue);
            memcpy(buffer+offset,&ftemp,ad[i].attrLen);
            break;
        case STRING:
            memcpy(buffer+offset,attrList[j].attrValue,ad[i].attrLen);
            //memcpy((char*)dbrec.data+offset,attrList[j].attrValue,ad[i].attrLen);
            break;
        }

        offset += ad[i].attrLen; 
    }
    //插入记录
    RID rid;
    InsertFileScan *iScan;
    iScan = new InsertFileScan(relation,status);
    dbrec.data=&buffer;
    status = iScan->insertRecord(dbrec,rid);
    if (status!=OK)
    {
        cout << endl << "Record insert failer!" << endl;
        return status;    
    }
    cout << endl << "1 record inserted!" << endl;
    delete iScan;
    if(status!=OK) return status;
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

    Status status;
    int itemp;
    float ftemp;
    AttrDesc ad;
    status = attrCat->getInfo(relation,attrName,ad);
    if (status != OK) return status;

    //Record rec;
    RID rid;
    HeapFileScan * scan;

    scan = new HeapFileScan(relation,status);
    if (status != OK) return status;

    switch(ad.attrType) {
        case INTEGER:
            itemp = atoi(attrValue);
            scan->startScan(ad.attrOffset,ad.attrLen,(Datatype)ad.attrType,(char*)&itemp,op);
            break;
        case FLOAT:
            ftemp = atof(attrValue);
            scan->startScan(ad.attrOffset,ad.attrLen,(Datatype)ad.attrType,(char*)&ftemp,op);
            break;
        case STRING:
            scan->startScan(ad.attrOffset,ad.attrLen,(Datatype)ad.attrType,attrValue,op);
            break;
    }
    
    if (status != OK) return status;
    int i = 0;
    while ((status = scan->scanNext(rid)) != FILEEOF)
    {
        if (status != OK) return status;
        status = scan->deleteRecord();
        if (status != OK) return status;
        i++; 
    }

    scan->endScan();
    delete scan;
    // scan = NULL;
    cout << endl;
    cout << i << " record(s) deleted!" << endl;
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
    Status status;
    int itemp,iitemp;
    float ftemp,fftemp;
    RelDesc rd;
    AttrDesc ad;

    //Check the relation
    status = relCat->getInfo(relation,rd);
    if (status != OK) return status;

    if (attrValue){
        status = attrCat->getInfo(relation,attrName,ad);
        if (status != OK) return status;
    }

    AttrDesc updateattrs[attrCnt];
    for (int i=0; i<attrCnt; i++)
    {
        status = attrCat->getInfo(attrList[i].relName,attrList[i].attrName,updateattrs[i]);
        if (status != OK) return status;
    }

    RID rid;

    Record rec;

    HeapFileScan * scan;

    scan = new HeapFileScan(relation,status);
    if (status != OK) return status;

    if (attrValue)
    {
        switch(ad.attrType) {
        case INTEGER:
            //Convert char* to Integer
            itemp = atoi(attrValue);
            scan->startScan(ad.attrOffset,ad.attrLen,(Datatype)ad.attrType,(char*)&itemp,op);
            break;
        case FLOAT:
            //Convert char* to Float
            ftemp = atof(attrValue);
            scan->startScan(ad.attrOffset,ad.attrLen,(Datatype)ad.attrType,(char*)&ftemp,op);
            break;
        case STRING:
            scan->startScan(ad.attrOffset,ad.attrLen,(Datatype)ad.attrType,attrValue,op);
            break;
        }
       if (status != OK) return status;
    }
    else
    {
        scan->startScan(0,0,STRING,NULL,EQ);
    }
    
    int j = 0;
    while ((status = scan->scanNext(rid)) != FILEEOF)
    {
        if (status != OK) return status;

        status = scan->getRecord(rec);
        //memcpy(&buffer,(char*)rec.data,reclen);
        if (status != OK) return status;
        for (int i=0; i<attrCnt; i++)
        {
            switch(updateattrs[i].attrType) {
            case INTEGER:
                //Convert char* to Integer
                iitemp = atoi((char*)attrList[i].attrValue);
                memcpy((char*)rec.data+updateattrs[i].attrOffset,&iitemp,updateattrs[i].attrLen);
                break;
            case FLOAT:
                //Convert char* to Float
                fftemp = atof((char*)attrList[i].attrValue);
                memcpy((char*)rec.data+updateattrs[i].attrOffset,&fftemp,updateattrs[i].attrLen);
                break;
            case STRING:
                memcpy((char*)rec.data+updateattrs[i].attrOffset,attrList[i].attrValue,updateattrs[i].attrLen);
                break;
            }
        }
        status = scan->updateRecord(rec);
        if (status != OK) return status;
        j++; 
    }

    scan->endScan();
    delete scan;
    scan = NULL;
    cout << endl;
    cout << j << " record(s) updated!" << endl;
    return OK;
}