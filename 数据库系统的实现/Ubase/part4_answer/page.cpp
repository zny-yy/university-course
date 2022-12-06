/* 
 * File:   page.cpp
 * Author: 张太红
 *
 * Created on 2015年11月4日, 下午4:54
 */
#include <iostream>
#include "page.h"

const Status Page::insertRecord(const Record &rec, RID &rid) {
    rid = NULLRID; //出错返回时，保证返回的rid为{-1，-1}
    int i;
    //寻找空插槽
    if (rec.length > 4076) return INVALIDRECLEN; // 超长记录
    for (i = 0; ((i > slotCnt) && (slot[i].length != -1)); i--);
    //循环结束后，i要么是第一个可用的空插槽，要么是slotCnt（新插槽）。
    //不管是那中情况，我们都可以直接用i充当要插入记录的插槽数组下标。

    if (i == slotCnt) //如果是新插槽
    {
        if ((rec.length + (int) sizeof (slot_t)) > freeSpace) return NOSPACE; //检查可用空间
        freeSpace -= (rec.length + sizeof (slot_t)); //调整可用空间
        slotCnt--; //调整新插槽标志
    } else //如果是删除记录留下的空插槽
    {
        if (rec.length > freeSpace) return NOSPACE; //检查可用空间
        freeSpace -= rec.length; //调整可用空间
    }
    //设置该插槽的属性
    slot[i].offset = freePtr;
    slot[i].length = rec.length;
    memcpy(&data[freePtr], rec.data, rec.length); //把记录数据拷贝到页的合适位置
    freePtr += rec.length; //调整data数组中可用空间的偏移量
    //设置返回参数的属性
    rid.pageNo = curPage;
    rid.slotNo = -i;
    return OK;
}

// 用rec更新记录rid

const Status Page::updateRecord(const Record & rec, const RID& rid) {
    int slotNo = -rid.slotNo; //convert to negative format

    //first check the record validation
    if ((slot[slotNo].length == -1) || (slotNo <= slotCnt)) return INVALIDSLOTNO;

    int offset = slot[slotNo].offset;

    memcpy(&data[offset], rec.data, rec.length); // copy data on to the data page

    return OK;
}

const Status Page::deleteRecord(const RID &rid) {
    int slotNo = -rid.slotNo; //转换为负数形式
    //首先检查要删除的记录是否有效
    if ((slot[slotNo].length == -1) || (!(slotNo > slotCnt))) return INVALIDSLOTNO;

    //如果要删除记录的数据不在页的尾部，需要移动其后的数据以压缩删除后出现空穴
    if ((slot[slotNo].offset + slot[slotNo].length) != freePtr) {
        char* dest = &data[slot[slotNo].offset]; //要删除记录数据的起始指针
        char* src = &data[slot[slotNo].offset + slot[slotNo].length]; //下一条记录数据的起始指针
        memmove(dest, src, freePtr - (slot[slotNo].offset + slot[slotNo].length)); //移动操作

        //调整数据在删除记录后面所有记录的偏移量
        for (int i = 0; i > slotCnt; i--)//扫描所有记录
        {
            //只对数据在删除记录后面所有记录进行调整
            if (slot[i].length >= 0 && slot[i].offset > slot[slotNo].offset) {
                slot[i].offset -= slot[slotNo].length;
            }
        }
    }
    freeSpace += slot[slotNo].length; //调整可用空间
    freePtr -= slot[slotNo].length; //调整data数组中可用空间的偏移量
    memset(&data[freePtr], 0, slot[slotNo].length); //清除尾部腾出的内存

    slot[slotNo].length = -1; //标记该插槽为空插槽
    slot[slotNo].offset = -1; //标记该插槽为空插槽

    //如果腾出的插槽在数组的尾部，此时可以压缩插槽数组，注意，可能还要压缩以前删除留下的空槽
    if (slotNo == slotCnt + 1) {
        do {
            slotCnt++;
            freeSpace += sizeof (slot_t);
        } while (slotCnt < 0 && slot[slotCnt + 1].length == -1);
    }
    return OK;
}

const Status Page::firstRecord(RID &firstRid) const {
    if (slotCnt == 0) return NORECORDS;
    for (int i = 0; i > slotCnt; i--) {
        if (slot[i].length != -1) {
            firstRid.pageNo = curPage;
            firstRid.slotNo = -i;
            break;
        }
    }
    return OK;
}

const Status Page::nextRecord(const RID &curRid, RID &nextRid) const {
    for (int i = -(curRid.slotNo + 1); i > slotCnt; i--) {
        if (slot[i].length != -1) {
            nextRid.pageNo = curPage;
            nextRid.slotNo = -i;
            return OK;
        }
    }
    return ENDOFPAGE;
}

const Status Page::getRecord(const RID &rid, Record &rec) {
    if (slotCnt == 0) return INVALIDSLOTNO;

    int i = -(rid.slotNo);
    if (slot[i].length == -1) return INVALIDSLOTNO;
    rec.length = slot[i].length;
    rec.data = &data[slot[i].offset];
    return OK;
}
