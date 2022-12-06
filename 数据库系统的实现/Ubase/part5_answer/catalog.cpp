/* 
 * File:   catalog.cpp
 * Author: 张太红
 * 
 * Created on 2015年11月29日, 下午1:13
 */
#include "catalog.h"

RelCatalog::RelCatalog(Status &status) : HeapFile(RELCATNAME, status) {
    // nothing needed
}

const Status RelCatalog::getInfo(const string & relation, RelDesc &record) {
    if (relation.empty())
        return BADCATPARM;

    Status status;
    Record rec;
    RID rid;

    HeapFileScan* scan;

    scan = new HeapFileScan(RELCATNAME, status);
    if (status != OK) return status;

    scan->startScan(0, sizeof (record.relName), STRING, relation.c_str(), EQ);
    if (status != OK) {
        delete scan;
        return status;
    }
    status = scan->scanNext(rid);
    if (status == FILEEOF) {
        delete scan;
        return RELNOTFOUND;
    }
    if ((status != FILEEOF) && (status != OK)) {
        delete scan;
        return status;
    }
    status = scan->getRecord(rec);
    if (status != OK) {
        delete scan;
        return status;
    }
    memcpy(&record, rec.data, rec.length);

    scan->endScan();
    delete scan;
    return OK;
}

const Status RelCatalog::addInfo(RelDesc & record) {
    RID rid;
    InsertFileScan* ifs;
    Status status;
    Record Relrec;

    Relrec.data = &record;
    Relrec.length = sizeof (RelDesc);
    ifs = new InsertFileScan(RELCATNAME, status);
    if (status != OK) return status;

    status = ifs->insertRecord(Relrec, rid);
    if (status != OK) {
        delete ifs;
        return status;
    }
    delete ifs;

    return OK;
}

const Status RelCatalog::removeInfo(const string & relation) {
    Status status;
    RID rid;
    HeapFileScan* hfs;

    if (relation.empty()) return BADCATPARM;

    hfs = new HeapFileScan(RELCATNAME, status);
    if (status != OK) return status;

    hfs->startScan(0, relation.length(), STRING, relation.c_str(), EQ);
    if (status != OK) {
        delete hfs;
        return status;
    }
    status = hfs->scanNext(rid);
    if (status != OK) {
        delete hfs;
        return status;
    }
    status = hfs->deleteRecord();
    if (status != OK) {
        delete hfs;
        return status;
    }
    hfs->endScan();
    delete hfs;

    return OK;
}

RelCatalog::~RelCatalog() {
    // nothing needed
}

AttrCatalog::AttrCatalog(Status &status) : HeapFile(ATTRCATNAME, status) {
    // nothing needed
}

const Status AttrCatalog::getInfo(const string & relation,
        const string & attrName,
        AttrDesc &record) {

    Status status;
    RID rid;
    Record rec;
    HeapFileScan* hfs;


    hfs = new HeapFileScan(ATTRCATNAME, status);
    if (status != OK) return status;

    hfs->startScan(0, sizeof (record.relName), STRING, relation.c_str(), EQ);
    if (status != OK) {
        delete hfs;
        return status;
    }
    while ((status = hfs->scanNext(rid)) != FILEEOF) {
        if (status != OK) {
            delete hfs;
            return status;
        }
        status = hfs->getRecord(rec);
        if (status != OK) {
            delete hfs;
            return status;
        }
        if (strncmp((char*) rec.data + sizeof (record.relName), attrName.c_str(), sizeof (record.attrName)) == 0) {
            memcpy(&record, rec.data, rec.length);
            break;
        }
    }

    hfs->endScan();
    delete hfs;

    return OK;
}

const Status AttrCatalog::addInfo(AttrDesc & record) {
    RID rid;
    InsertFileScan* ifs;
    Status status;

    Record Attrrec;

    Attrrec.data = &record;
    Attrrec.length = sizeof (AttrDesc);
    ifs = new InsertFileScan(ATTRCATNAME, status);
    if (status != OK) {
        return status;
    }
    status = ifs->insertRecord(Attrrec, rid);
    if (status != OK) {
        delete ifs;
        return status;
    }
    delete ifs;

    return OK;

}

const Status AttrCatalog::removeInfo(const string & relation,
        const string & attrName) {
    Status status;
    Record rec;
    RID rid;
    AttrDesc record;
    HeapFileScan* hfs;

    if (relation.empty() || attrName.empty()) return BADCATPARM;

    hfs = new HeapFileScan(ATTRCATNAME, status);
    if (status != OK) return status;

    hfs->startScan(0, sizeof (record.relName), STRING, relation.c_str(), EQ);
    if (status != OK) {
        delete hfs;
        return status;
    }
    while ((status = hfs->scanNext(rid)) != FILEEOF) {
        if (status != OK) {
            delete hfs;
            return status;
        }

        status = hfs->getRecord(rec);
        if (status != OK) {
            delete hfs;
            return status;
        }
        if (strncmp((char*) rec.data + sizeof (record.relName), attrName.c_str(), sizeof (record.attrName)) == 0) {
            status = hfs->deleteRecord();
            if (status != OK) {
                delete hfs;
                return status;
            }
            break;
        }
    }

    hfs->endScan();
    delete hfs;

    return OK;
}

const Status AttrCatalog::getRelInfo(const string & relation,
        int &attrCnt,
        AttrDesc *&attrs) {
    Status status;
    RID rid;
    Record rec;
    HeapFileScan* hfs;
    RelDesc rel;
    if (relation.empty()) return BADCATPARM;

    status = relCat->getInfo(relation, rel);
    //if (status == FILEEOF) return  RELNOTFOUND;
    //if ((status != FILEEOF) && (status != OK)) return status;

    if (status != OK) return status;

    hfs = new HeapFileScan(ATTRCATNAME, status);

    hfs->startScan(0, sizeof (attrs->relName), STRING, relation.c_str(), EQ);

    if (status != OK) {
        delete hfs;
        return status;
    }
    attrCnt = 0;
    attrs = new AttrDesc[10];
    while ((status = hfs->scanNext(rid)) != FILEEOF) {
        if (status != OK) {
            delete hfs;
            return status;
        }
        status = hfs->getRecord(rec);
        if (status != OK) {
            delete hfs;
            return status;
        }

        memcpy(&attrs[attrCnt], rec.data, rec.length);
        attrCnt++;
    }
    hfs->endScan();
    delete hfs;

    return OK;

}

AttrCatalog::~AttrCatalog() {
    // 无需任何处理
}

const Status RelCatalog::createRel(const string & relation,
        const int attrCnt,
        const attrInfo attrList[]) {
    Status status;
    RelDesc rd;
    AttrDesc ad;


    if (relation.empty() || attrCnt < 1)
        return BADCATPARM;

    if (relation.length() >= sizeof rd.relName)
        return NAMETOOLONG;

    status = getInfo(relation, rd);
    if (status == OK) return RELEXISTS;
    if ((status != OK) && (status != RELNOTFOUND)) return status;

    //检查是否有同名字段
    bool duplicated = false;
    for (int i = 0; i < attrCnt - 1; i++) {
        for (int j = i + 1; j < attrCnt; j++) {
            if (strncmp(attrList[i].attrName, attrList[j].attrName, MAXNAME) == 0) {
                duplicated = true;
                break;
            }
        }
    }
    if (duplicated == true) return DUPLATTR;

    // 检查字段长度之和（记录长度）是否超过限制，4096字节的页能容纳的最长记录为4076字节
    int maxRecordLen = 0;
    for (int i = 0; i < attrCnt; i++) {
        maxRecordLen += attrList[i].attrLen;
    }
    if (maxRecordLen > 4076) return ATTRTOOLONG;

    // 向字典表relcat添加记录
    strcpy(rd.relName, relation.c_str());
    rd.attrCnt = attrCnt;
    status = addInfo(rd);
    if (status != OK) return status;

    // 向字典表attrcat添加记录
    int Offset = 0;
    for (int i = 0; i < attrCnt; i++) {
        strcpy(ad.relName, attrList[i].relName); // relation name
        strcpy(ad.attrName, attrList[i].attrName); // attribute name
        ad.attrOffset = Offset; // attribute offset
        ad.attrType = attrList[i].attrType; // attribute type
        ad.attrLen = attrList[i].attrLen; // attribute lengt
        status = attrCat->addInfo(ad);
        Offset += attrList[i].attrLen;
        if (status != OK) return status;
    }

    // 创建堆文件
    status = createHeapFile(relation);
    if (status != OK) return status;
    if (relation != "Tmp_Ubase_Result") {
        cout << endl;
        cout << relation << "表创建成功!" << endl;
    }
    return OK;
}

//
// Destroys a relation. It performs the following steps:
//
// 	removes the catalog entry for the relation
// 	destroys the heap file containing the tuples in the relation
//
// Returns:
// 	OK on success
// 	error code otherwise
//

const Status RelCatalog::destroyRel(const string & relation) {
    Status status;

    if (relation.empty() ||
            relation == string(RELCATNAME) ||
            relation == string(ATTRCATNAME))
        return BADCATPARM;

    //destroys the heap file containing the tuples in the relation
    status = destroyHeapFile(relation);
    if (status != OK) return status;

    //get relation name(also file name) from catalog(relcat)
    RelDesc record;

    status = getInfo(relation, record);
    if (status == FILEEOF) return RELNOTFOUND;
    if ((status != OK) && (status != FILEEOF)) return status;

    //remove the catalog entry of the relation from attrcat
    status = attrCat->dropRelation(relation);
    if (status != OK) return status;

    //remove the catalog entry of the relation from relcat
    status = removeInfo(relation);
    if (status != OK) return status;
    if (relation != "Tmp_Ubase_Result") {
        cout << endl;
        cout << "成功删除" << relation << "表!" << endl;
    }

    return OK;
}


//
// Drops a relation. It performs the following steps:
//
// 	removes the catalog entries for the relation
//
// Returns:
// 	OK on success
// 	error code otherwise
//

const Status AttrCatalog::dropRelation(const string & relation) {
    Status status;
    AttrDesc *attrs;
    int attrCnt;

    if (relation.empty())
        return BADCATPARM;

    //get attributes infomation of the relation
    status = getRelInfo(relation, attrCnt, attrs);
    if (status != OK) return status;

    //removes the catalog entries in attrcat of the relation
    for (int i = 0; i < attrCnt; i++) {
        status = removeInfo(relation, attrs[i].attrName);
        if (status != OK) return status;
    }
    delete [] attrs;
    return OK;
}


//
// Retrieves and prints information from the catalogs about the for the
// user. If no relation is given (relation is NULL), then it lists all
// the relations in the database, along with the width in bytes of the
// relation, the number of attributes in the relation, and the number of
// attributes that are indexed.  If a relation is given, then it lists
// all of the attributes of the relation, as well as its type, length,
// and offset, whether it's indexed or not, and its index number.
//
// Returns:
// 	OK on success
// 	error code otherwise
//

const Status RelCatalog::help(const string & relation) {
    Status status;
    RelDesc rd;
    AttrDesc *attrs;
    int attrCnt;

    if (relation.empty()) //no relation specified
    {
        status = UT_Print(RELCATNAME);
        if (status != OK) return status;
    } else // relation specified
    {
        //check relcat to make sure the relation exists
        status = relCat->getInfo(relation, rd);
        if (status == FILEEOF) return RELNOTFOUND;
        if ((status != OK) && (status != FILEEOF)) return status;

        //check attrcat to make sure the relation attributes entry exists
        status = attrCat->getRelInfo(relation, attrCnt, attrs);
        if (status != OK) return status;

        //print header of the relation
        printf("%-*.*s  ", 20, 20, "Table Name");
        printf("%-*.*s  ", 20, 20, "Attribute Name");
        printf("%-*.*s  ", 6, 6, "Offset");
        printf("%-*.*s  ", 6, 6, "Type");
        printf("%-*.*s  ", 6, 6, "Length");
        printf("\n");
        printf("%-*.*s  ", 20, 20, "--------------------");
        printf("%-*.*s  ", 20, 20, "--------------------");
        printf("%-*.*s  ", 6, 6, "------");
        printf("%-*.*s  ", 6, 6, "------");
        printf("%-*.*s  ", 6, 6, "------");
        printf("\n");

        //print tuples
        char *att_type;
        char stringType[] = "string";
        char intType[] = "int";
        char floatType[] = "float";
        for (int i = 0; i < attrCnt; i++) {
            printf("%-*.*s  ", 20, 20, attrs[i].relName);
            printf("%-*.*s  ", 20, 20, attrs[i].attrName);
            printf("%-*d  ", 6, attrs[i].attrOffset);
            switch (attrs[i].attrType) {
                case 0:
                    att_type = stringType;
                    break;
                case 1:
                    att_type = intType;
                    break;
                case 2:
                    att_type = floatType;
                    break;
            }
            printf("%-*.*s  ", 6, 6, att_type);
            printf("%-*d  ", 6, attrs[i].attrLen);
            printf("\n");
            /*
                  cout << setw(32) << attrs[i].relName;
                  cout << attrs[i].attrName;
                  cout << attrs[i].attrOffset;
                  cout << attrs[i].attrType;
                  cout << attrs[i].attrLen;
                  cout << endl;
             */
        }
        delete [] attrs;
    }

    return OK;
}


