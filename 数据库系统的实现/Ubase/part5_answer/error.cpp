/* 
 * File:   Error.cpp
 * Author: zhangtaihong
 * 
 * Created on 2015年11月4日, 下午3:58
 */
#include "error.h"

using namespace std;

void Error::print(Status status)
{
    cerr << "错误信息：";
    switch(status) {
        
        // 没有发生任何错误
        case OK:           cerr << "一切正常"; break;

        // File和DB错误
        case BADFILEPTR:   cerr << "文件指针错误"; break;
        case BADFILE:      cerr << "文件名称错误"; break;
        case FILETABFULL:  cerr << "管理打开文件的散列表已经满了"; break;
        case FILEOPEN:     cerr << "文件已经打开"; break;
        case FILENOTOPEN:  cerr << "文件没有打开"; break;
        case UNIXERR:      cerr << "Unix系统错误"; break;
        case BADPAGEPTR:   cerr << "页帧指针错误"; break;
        case BADPAGENO:    cerr << "页帧编号错误"; break;
        case FILEEXISTS:   cerr << "文件已经存在"; break;
        case BADREADOFFSET:   cerr << "设置读文件起始位置错误"; break;
        case BADWRITEOFFSET:   cerr << "设置写文件起始位置错误"; break;
        case BADREADSIZE:   cerr << "读出块与页大小不一致"; break;
        case BADWRITESIZE:   cerr << "写入块与页大小不一致"; break;


        // BufMgr和HashTable错误
        case HASHTBLERROR: cerr << "管理缓存的散列表错误"; break;
        case HASHNOTFOUND: cerr << "找不到散列项"; break;
        case BUFFEREXCEEDED: cerr << "缓存池已满"; break;
        case PAGENOTPINNED: cerr << "页帧没有被钉住"; break;
        case BADBUFFER: cerr << "缓存池崩溃"; break;
        case PAGEPINNED: cerr << "页帧仍然被钉住"; break;

        // Page类错误
        case NOSPACE: cerr << "本页帧可有空间不能容纳该记录"; break;
        case NORECORDS: cerr << "本页帧没有存储任何记录"; break;
        case ENDOFPAGE: cerr << "到达页尾"; break;
        case INVALIDSLOTNO: cerr << "插槽号无效"; break;
        case INVALIDRECLEN: cerr << "记录长度错误（length <= 0）";break;

        // Heap文件错误
        case BADRID:       cerr << "记录RID错误"; break;
        case BADRECPTR:    cerr << "记录指针错误"; break;
        case BADSCANPARM:  cerr << "扫描参数错误"; break;
        case SCANTABFULL:  cerr << "扫描表已满"; break;
        case FILEEOF:      cerr << "到达文件尾部"; break;
        case FILEFULL:     cerr << "堆达文（表）已满"; break;
        case FILEHDRFULL:  cerr << "堆文件头也已满"; break;


        // 索引错误
        case BADINDEXPARM: cerr << "索引参数错误"; break;
        case RECNOTFOUND:  cerr << "没有该记录"; break;
        case BUCKETFULL:   cerr << "散列通已满"; break;
        case DIROVERFLOW:  cerr << "目录已满"; break;
        case NONUNIQUEENTRY: cerr << "项目不唯一"; break;
        case NOMORERECS:   cerr << "不再有其它记录"; break;

        // 排序文件错误
        case BADSORTPARM:  cerr << "排序参数错误"; break;
        case INSUFMEM:     cerr << "内存不足"; break;

        // 数据字典错误
        case BADCATPARM:   cerr << "数据字典参数错误"; break;
        case RELNOTFOUND:  cerr << "关系不在数据字典表中"; break;
        case ATTRNOTFOUND: cerr << "属性不在数据字典表中"; break;
        case NAMETOOLONG:  cerr << "名称太长"; break;
        case ATTRTOOLONG:  cerr << "属性太长"; break;
        case DUPLATTR:     cerr << "属性名称重复"; break;
        case RELEXISTS:    cerr << "关系已经存在"; break;
        case NOINDEX:      cerr << "索引不存在"; break;
        case ATTRTYPEMISMATCH:   cerr << "属性类型不匹配"; break;
        case TMP_RES_EXISTS:    cerr << "临时结果已经存在"; break;
        case INDEXEXISTS:  cerr << "索引已经存在"; break;

        // 查询和更新操作符
        case ATTRNUMBERMISMATCH:  cerr << "属性数量不匹配"; break;
        case ATTRVALUENULL:  cerr << "属性无值"; break;

        default:           cerr << "未定义的错误: ";
    }
    cerr << endl;
}
