
/* 
 * 文件: DB.cpp
 * 班级: XXXXXXXX
 * 学号: YYYYYYYY
 * 姓名: ZZZZZZZZ
 */
 
#include "DB.h"

//using namespace std;

/*
 * 功能：当执行SQL语句时，如果有满足条件的记录要显现，将触发该回调函数输出查询结果
 * 参数：NotUsed，占位参数，该函数不使用它
 *      argc，查询结果所含列（字段）的数量
 *      argv，查询结果，argv[0]为第一列的值，argv[1]为第二列的值...
 *      szColName，列名，szColName[0]为第一列的列名，szColName[1]为第二列的列名...
 * 实现：通过for输出每列的值，然后输出换行
 * 返回：0
 * 输出：用tab分割的一条记录
 * 注意：每条记录都会调用该函数
 */
static int callback(void* NotUsed, int argc, char** argv, char** szColName) {
    for (int i = 0; i < argc; i++) {
        std::cout << argv[i] << "\t";
    }
    std::cout << std::endl;

    return 0;
}

/*
 * 功能：DB类的构造函数，用来创建或打开数据库，这是DB类的一个公开方法
 * 参数：要创建或打开的数据库文件名称
 * 实现：通过调用sqlite3_open接口来创建或打开一个数据库，
 *      由DB类的私有属性database来保存打开数据库对象的指针
 * 返回：无
 * 输出：无
 */
DB::DB(const std::string dbName) {
    sqlite3_open(dbName.c_str(), &database);
}

/*
 * 功能：DB类的析构函数，用来关闭当前打开的数据库，这是DB类的一个公开方法
 * 参数：无
 * 实现：通过调用sqlite3_close接口来关闭当前打开的数据库，参数为DB类的私有属性database
 * 返回：无
 * 输出：无
 */
DB::~DB() {
    sqlite3_close(database);
}

/*
 * 功能：用来创建联系人表，这是DB类的一个公开方法
 * 参数：创建联系人表的SQL语句
 * 实现：1、调用DB类的私有方法tableExist判断contacts是否存在，如果存在则先删除该表
 *      2、调用DB类的私有方法exeSQL执行创建表的SQL语句并输出提示信息
 * 返回：无
 * 输出：联系人添加是否成功的信息
 */
void DB::createTable(const std::string sql) {
    // 如果contacts表已经存在，先将其删除
    // exeSQL("DROP TABLE IF EXISTS contacts;");
    if (tableExist("contacts")) {
        exeSQL("DROP TABLE contacts;");
    }
    if (exeSQL(sql) == SQLITE_OK) {
        std::cout << "联系人表创建成功！" << std::endl;
    } else
        std::cout << "联系人表创建失败！" << std::endl;
}

/*
 * 功能：用来添加一个联系人信息，这是DB类的一个公开方法
 * 参数：name，要添加的联系人姓名
 *      Phone，更添加联系人电话
 * 实现：构造插入语句并通过exeSQL执行之
 * 返回：无
 * 输出：联系人添加是否成功的信息
 */
void DB::add(const std::string name, const std::string phone) {
    std::string sql = "INSERT INTO contacts(name, phone) VALUES('" + name + "','" + phone + "');";
    if (exeSQL(sql) == SQLITE_OK) {
        std::cout << "联系人添加成功！" << std::endl;
    } else
        std::cout << "添加联系人失败！" << std::endl;
}

/*
 * 功能：用来修改指定的联系人信息，这是DB类的一个公开方法
 * 参数：name，要修改的联系人姓名
 *      newName，更新后的姓名
 *      newPhone，更新后的电话
 * 实现：用WHERE子句构造带条件的更新语句（contacts表中name等于参数值）并通过exeSQL执行之
 * 返回：无
 * 输出：联系人修改是否成功的信息
 */
void DB::update(const std::string name, const std::string newName, const std::string newPhone) {
   
    /*
     *  在这里编写你的代码
	 *		 
	 */
    std::string sql = ("UPDATE contacts SET name=('"+ newName +"'), phone=('" +newPhone + "')WHERE name=('" +name+ "');");
    if (exeSQL(sql) == SQLITE_OK) {
        std::cout << "更新联系人成功！" << std::endl;
    } else
        std::cout << "更新联系人失败！" << std::endl;

}

/*
 * 功能：用来删除指定的联系人，这是DB类的一个公开方法
 * 参数：要删除的联系人姓名
 * 实现：用WHERE子句带构造条件的删除语句（contacts表中name等于参数值）并通过exeSQL执行之
 * 返回：无
 * 输出：联系人删除是否成功的信息
 */
void DB::remove(const std::string name) {
    
	/*
     *  在这里编写你的代码
	 *		 
	 */
    std::string sql = ("DELETE FROM contacts WHERE name=('" +name+ "');");
    if (exeSQL(sql) == SQLITE_OK) {
        std::cout << "删除人添加成功！" << std::endl;
    } else
        std::cout << "删除联系人失败！" << std::endl;

}

/*
 * 功能：用来查找指定的联系人，这是DB类的一个公开方法
 * 参数：要查询的联系人姓名
 * 实现：用WHERE子句构带造条件的查询语句（contacts表中name等于参数值）并通过exeSQL执行之
 * 返回：无
 */
void DB::find(const std::string name) {
    
	/*
     *  在这里编写你的代码
	 *		 
	 */
    std::string sql = ("SELECT* FROM contacts WHERE name=('"+name+"');");
    if (exeSQL(sql) == SQLITE_OK) {
        std::cout << "查找人添加成功！" << std::endl;
    } else
        std::cout << "查找联系人失败！" << std::endl;

}

/*
 * 功能：显示所有联系人，这是DB类的一个公开方法
 * 参数：无
 * 实现：构造返contacts表所有记录的查询语句并通过exeSQL执行之
 * 返回：无
 */
void DB::listAll() {
    std::string sql = "SELECT * FROM contacts;";
    exeSQL(sql);
}

/*
 * 功能：用来执行SQL语句，这是DB类的一个私有方法
 * 参数：sql为要执行的SQL语句
 * 实现：通过调用sqlite3_exec接口来执行SQL语句
 * 返回：整数，直接返回sqlite3_exec的执行状态
 */
int DB::exeSQL(const std::string sql) {
    char *szErrMsg = 0;
    return sqlite3_exec(database, sql.c_str(), callback, NULL, &szErrMsg);
}

/*
 * 功能：判断一个表是否已经在数据库中存在，这是DB类的一个私有方法
 * 参数：表名
 * 实现：1、构造带参数的查询，参数用?表示
 *      2、用sqlite3_prepare_v2编译查询，生成预编译的查询语句stmt
 *      3、用sqlite3_bind_text绑定参数值
 *      4、用sqlite3_step执行查询
 *      5、用sqlite3_column_int获取结果
 *      6、销毁stmt
 *      7、根据查询返回表示
 * 返回：布尔值，true：表已经存在，false：表不存在
 */
bool DB::tableExist(const std::string tableName) {
    int result = 0;
    sqlite3_stmt *stmt = NULL;

    // 查询系统表sqlite_master
    std::string sql = "SELECT COUNT(*) FROM sqlite_master WHERE type='table' AND name=?;";

    // 编译SQL语句
    sqlite3_prepare_v2(database, sql.c_str(), -1, &stmt, NULL);

    // 绑定查询条件
    sqlite3_bind_text(stmt, 1, tableName.c_str(), -1, NULL);

    /* 如果查询结果包含多条记录，请用以下循环逐行处理结果
    while (sqlite3_step(stmt) == SQLITE_ROW) {  // 执行查询语句
        result = sqlite3_column_int(stmt, 0);   // 获取查询结果
        sqlite3_reset(stmt);                    // 让stmt准备就绪
    }
     */

    sqlite3_step(stmt); // 执行查询语句

    result = sqlite3_column_int(stmt, 0); // 获取查询结果

    sqlite3_finalize(stmt); // 删除stmt

    if (result > 0)
        return true;

    return false;
}