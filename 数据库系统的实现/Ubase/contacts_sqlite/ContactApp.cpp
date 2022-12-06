/* 
 * File:   ContactApp.cpp
 * Author: 张太红
 *
 * Created on 2015年10月23日, 下午5:13
 */
#include "DB.h"

int main() {
    /* 
     * 通过调用DB类的构造函数创建一个DB对象，命名为db
     * 参数为数据库文件名称，即当前目录中的contacts.db
     * 如果当前目录中存在contacts.db，就打开该数据库，否则在当前目录中创建contacts.db文件
     */
    DB db = DB("contacts.db");

    // 在数据库中创建contacts表
    db.createTable("CREATE TABLE contacts(_id INTEGER PRIMARY KEY AUTOINCREMENT, name varchar(24), phone varchar(11));");

    // 将所有同学信息添加到通讯录表中
    db.add("张兵冰", "18146401100");
    db.add("杨涛", "18146431101");
    db.add("马国安", "18160601102");
    db.add("温强", "15894761103");
    db.add("顾东", "18290861128");
    db.add("何洪", "15160901129");
    db.add("彭小惠", "18119151130");
    db.add("高夏", "18209911131");
    db.add("王邵明", "18703021132");
    db.add("胡陶", "15999111133");
    db.add("王晓", "18399671134");
    

    // 再添加一个联系人
    db.add("张兵", "13325522066");

    //查找何洪
    db.find("何洪");

    // 修改何洪
    db.update("何洪", "何洪森", "15160902235");

    // 删除王晓
    db.remove("王晓");

    //输出全部联系人
    std::cout << "联系人表contacts最终所有的记录如下：" << std::endl;
    std::cout << "===========================" << std::endl;
    std::cout << "编号\t" << "姓名\t" << "电话" << std::endl;
    std::cout << "===========================" << std::endl;
    db.listAll();
    std::cout << "===========================" << std::endl;
}
