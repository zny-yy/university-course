/* 
 * File:   Page.h
 * Author: 张太红
 *
 * Created on 2015年11月4日, 下午4:54
 */
#include <iostream>
#include "Page.h"

// page class constructor

void Page::init(int pageNo) {
    memset(data, 0, PAGESIZE - DPFIXED);
    nextPage = -1;
    slotCnt = 0; // no slots in use
    curPage = pageNo;
    freePtr = 0; // offset of free space in data array
    freeSpace = PAGESIZE - DPFIXED + sizeof (slot_t); // amount of space available
    dummy = 0; // 避免编译警告信息
}

// dump page utlity

void Page::dumpPage() const {
    int i;

    cout << "curPage = " << curPage << ", nextPage = " << nextPage
            << "\nfreePtr = " << freePtr << ",  freeSpace = " << freeSpace
            << ", slotCnt = " << slotCnt << endl;

    for (i = 0; i > slotCnt; i--)
        cout << "slot[" << i << "].offset = " << slot[i].offset
            << ", slot[" << i << "].length = " << slot[i].length << endl;
}

const Status Page::setNextPage(const int pageNo) {
    nextPage = pageNo;
    return OK;
}

const Status Page::getNextPage(int &pageNo) const {
    pageNo = nextPage;
    return OK;
}

const short Page::getFreeSpace() const {
    return freeSpace;
}

// Add a new record to the page. Returns OK if everything went OK
// otherwise, returns NOSPACE if sufficient space does not exist
// RID of the new record is returned via rid parameter

const Status Page::insertRecord(const Record & rec, RID& rid) {
    // you will implement this method during part2
    
    return OK;
}

// delete a record from a page. Returns OK if everything went OK
// compacts remaining records but leaves hole in slot array
// use bcopy and not memcpy to do the compaction

const Status Page::deleteRecord(const RID & rid) {
    // you will implement this method during part2

    return OK;
}

// update the record with the specified rid

const Status Page::updateRecord(const Record & rec, const RID& rid) {
    // you will implement this method during part2

    return OK;
}

// returns RID of first record on page

const Status Page::firstRecord(RID& firstRid) const {
    // you will implement this method during part2

    return OK;
}

// returns RID of next record on the page
// returns ENDOFPAGE if no more records exist on the page; otherwise OK

const Status Page::nextRecord(const RID &curRid, RID& nextRid) const {
    // you will implement this method during part2
    
    return OK;
}

// returns length and pointer to record with RID rid

const Status Page::getRecord(const RID & rid, Record & rec) {
    // you will implement this method during part2

    return OK;
}
