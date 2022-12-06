/* 
 * 文件: BufferManager.cpp
 * 班级: XXXXXXXX
 * 学号: YYYYYYYY
 * 姓名: ZZZZZZZZ
 */
 
 #include "BufferManager.h"

// BufferHashTable类6个私有方法的实现
// 该类管理已用缓冲帧的散列表

// 构造函数

BufferHashTable::BufferHashTable(const int htSize) {
    HTSIZE = htSize;
    ht = new BufferPageNode* [HTSIZE];
    for (int i = 0; i < htSize; i++)
        ht[i] = NULL;
}

// 析构函数

BufferHashTable::~BufferHashTable() {
    //释放所有散列节点的散列表(ht)
    //因为ht为指针数组，我们用delete [] ht来释放它    
    for (int i = 0; i < HTSIZE; i++) {
        BufferPageNode* tmpNode = ht[i];
        while (ht[i]) {
            tmpNode = ht[i];
            ht[i] = ht[i]->next;
            delete tmpNode;
        }
    }
    delete []ht;
}

// 散列函数，返回0到HTSIZE-1之间的整数用作桶号索引

int BufferHashTable::hash(const File* file, const int pageNo) {
    return ((unsigned long) file + pageNo) % HTSIZE;

}

// 向散列表插入映射(file,pageNo)到frameNo的散列节点
// 成功返回OK，发生错误返回HASHTBLERROR

Status BufferHashTable::insert(const File* file, const int pageNo, const int frameNo) {
    int index = hash(file, pageNo);
    //查找节点是否存在
    BufferPageNode* tmpBuc = ht[index];
    while (tmpBuc) {
        //如果已经存在这个节点
        if ((tmpBuc->file == file) && (tmpBuc->pageNo == pageNo)) return HASHTBLERROR;

        //遍历溢出链的下一个节点
        tmpBuc = tmpBuc->next;
    }

    //创建一个新的散列节点
    tmpBuc = new BufferPageNode;
    if (!tmpBuc) return HASHTBLERROR;
    
	//设置散列节点的属性
	/*
     *  在这里编写你的代码
	 *		 
	 */
	 

    //将散列节点插入到溢出链的头部
    /*
     *  在这里编写你的代码
	 *		 
	 */

    return OK;
}

// 检查(file,pageNo)是否在缓冲池中（是否在散列表中)
// 如果在，返回OK并通过参数frameNo返回对应的帧号。否则返回HASHNOTFOUND

Status BufferHashTable::lookup(const File* file, const int pageNo, int& frameNo) {
    int index = hash(file, pageNo); //计算散列表数组的下标
    BufferPageNode* tmpBuc = ht[index];

    //遍历溢出链节点
    while (tmpBuc) {   	   	
         
		/*
	     *  在这里编写你的代码
		 *		 
		 */
                          
    }
    return HASHNOTFOUND;
}

// 如果(file,pageNo)节点存在，删除散列节点并返回OK，否则返回HASHTBLERROR

Status BufferHashTable::remove(const File* file, const int pageNo) {
    int index = hash(file, pageNo); //计算散列表数组的下标

    BufferPageNode* tmpBuc = ht[index]; //溢出链上的第一个节点

    BufferPageNode* prevBuc = ht[index]; //溢出链上当前节点的前一个节点

    //遍历溢出链节点
    while (tmpBuc) {
        if ((tmpBuc->file == file) && (tmpBuc->pageNo == pageNo)) //找到匹配节点
        {
		
			/*
		     *  在这里编写你的代码
			 *		 
			 */
			 
            delete tmpBuc; //删除找到的匹配节点
            return OK;
            
        } else {
        	
            /*
		     *  在这里编写你的代码
			 *				 	 
			 */
        }
    }
    return HASHTBLERROR;
}

// BufferManager类7个公有方法的实现

// 构造函数

BufferManager::BufferManager(const int bufs) {
    bufferNumbers = bufs; // 设置缓冲池大小
    clockHand = 0; // 时钟指针初始位置
    bufferInfo = new BufferInfo[bufs]; // 分配缓存信息描述数组
    for (int i = 0; i < bufs; i++) bufferInfo[i].frameNo = i; //初始化帧号
    bufPool = new Page[bufs]; // 分配缓存池
    bzero(bufPool, bufs * sizeof (Page)); //初始化缓存池 
    bufferhashTable = new BufferHashTable(prime(bufs * 1.2)); // 创建散列表
    //cout << prime(bufs * 1.2) << endl;
}

// 析构函数

BufferManager::~BufferManager() {
    commit(); // 将所有脏页写回磁盘
    delete [] bufPool; // 释放缓存池
    delete [] bufferInfo; // 释放缓存信息描述数组
    delete bufferhashTable; // 释放散列表
}

// 读页

const Status BufferManager::readPage(File* file, const int pageNo, Page* &page) {
    Status status;
    int index;

    if (!file) return BADFILE;

    if (bufferhashTable->lookup(file, pageNo, index) == HASHNOTFOUND) {
        status = chooseFrame(index);
        if (status != OK) return status;

        status = file->readPage(pageNo, &bufPool[index]);
        if (status != OK) return status;

        status = bufferhashTable->insert(file, pageNo, index);
        if (status != OK) return status;

        bufferInfo[index].Set(file, pageNo);
    } else {
        if (bufferInfo[index].empty) return BADBUFFER;
        bufferInfo[index].pinCnt++;
    }

    page = &(bufPool[index]);
    return OK;
}

// 解除摁钉

const Status BufferManager::unPinPage(const File* file, const int pageNo, const bool dirty) {
    Status status;
    int index;
    status = bufferhashTable->lookup(file, pageNo, index);
    if (status != OK) return status;

    if (bufferInfo[index].empty) return BADBUFFER;
    if (bufferInfo[index].pinCnt == 0) return PAGENOTPINNED;
    if (dirty)
        bufferInfo[index].dirty = dirty;

    bufferInfo[index].pinCnt--;

    return OK;
}

// 分配新页

const Status BufferManager::allocPage(File* file, int &pageNo, Page* &page) {
    Status status;
    int index;
    status = chooseFrame(index);
    if (status != OK) return status;

    status = file->allocatePage(pageNo);
    if (status != OK) return status;

    status = bufferhashTable->insert(file, pageNo, index);
    if (status != OK) return status;

    bufferInfo[index].Set(file, pageNo);

    page = &(bufPool[index]);

    return OK;
}


// 回收空白页

const Status BufferManager::disposePage(File* file, const int pageNo) {
    Status status;
    int index;

    if (!file) return BADFILE;

    if (bufferhashTable->lookup(file, pageNo, index) == OK) {
        BufferInfo* bInfo = &bufferInfo[index];

        if (bInfo->empty || bInfo->file != file || bInfo->pageNo != pageNo) return BADBUFFER;
        if (bInfo->pinCnt > 0) return PAGENOTPINNED;

        if ((status = bufferhashTable->remove(file, pageNo)) != OK) return status;
        // void bzero(void *s, int n);
        bzero(&bufPool[bInfo->frameNo], PAGESIZE);
        bInfo->Clear();
    }

    return file->disposePage(pageNo);

}
// 提交所有修改，即将所有修改过的页写回磁盘文件

const Status BufferManager::commit() {
    // 将所有脏页写回磁盘
    for (int i = 0; i < bufferNumbers; i++) {
        BufferInfo* bufInfo = &(bufferInfo[i]);
        if (bufInfo->pinCnt > 0) return PAGEPINNED;
        if (!bufInfo->empty && bufInfo->dirty && bufInfo->file!=NULL) {
            Status s = bufInfo->file->writePage(bufInfo->pageNo, &(bufPool[i]));
            if (s != OK) return s;
            //bufInfo->dirty = false;
        }
    }
    return OK;
}

// 从缓存中剔除文件(删除散列节点、清除缓存描述、写回脏页)，为彻底关闭文件做准备

const Status BufferManager::rejectFile(File* file) {
    for (int i = 0; i < bufferNumbers; i++) {
        BufferInfo* bufInfo = &(bufferInfo[i]);
        if ((!bufInfo->empty) && (bufInfo->file == file)) {
            if (bufInfo->dirty) {
                Status s = bufInfo->file->writePage(bufInfo->pageNo, &(bufPool[i]));
                if (s != OK) return s;
            }
            Status s = bufferhashTable->remove(file, bufInfo->pageNo);
            if (s != OK) return s;
            bufInfo->Clear();
        }
    }
    return OK;

}



// BufferManager类1个私有方法的实现
// 按时钟替代策略在缓冲池中选择一帧

const Status BufferManager::chooseFrame(int &frame) {
    Status status;
    BufferInfo* victim;
    int pinFrames = 0; // 计算是否全部被钉住
    while (true) {
        advanceClock(); // 时钟走一步

        /*
         *  在这里编写你的代码
	     *		 
	     */ 
		                       
    }
    
    // 脏页写入文件
    if (victim->dirty) {
        status = victim->file->writePage(victim->pageNo, &bufPool[victim->frameNo]);
        if (status != OK) return status;
    }
    
    // 有数据的页从散列表删除
    if (!victim->empty) {
        status = bufferhashTable->remove(victim->file, victim->pageNo);
        if (status != OK) return status;
    }
    
    // 找到的帧号，通过引用参数返回给调用者
    frame = victim->frameNo;

    // 清理缓冲帧描述信息
    victim->Clear();
    return OK;
}