/* 
 * File:   qiut.cpp
 * Author: 张太红
 * 
 * Created on 2015年11月29日, 下午1:13
 */

#include "catalog.h"

extern BufferManager *bufMgr;
extern RelCatalog *relCat;
extern AttrCatalog *attrCat;

/*
 * 关闭数据字典、销毁缓存管理器，然后终止UBASE程序
 * 没有返回值
 */

void UT_Quit(void) {
    // 关闭数据字典
    delete relCat;
    delete attrCat;

    // 销毁缓存管理器
    delete bufMgr;

    // 终止UBASE程序
    exit(1);
}
