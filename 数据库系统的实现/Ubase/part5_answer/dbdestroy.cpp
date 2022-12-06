/* 
 * File:   dbdestroy.cpp
 * Author: 张太红
 * 
 * Created on 2015年11月29日, 下午1:13
 */

#include <iostream>  // cout
#include <stdio.h>   // scanf(),sprintf())
#include <stdlib.h>  // system())
using namespace std;

int main(int argc, char *argv[]) {
    if (argc < 2) {
        cerr << "用法: " << argv[0] << " 数据库名称" << endl;
        return 1;
    }

    char command[128];

    cout << "你准备删除数据库" << argv[1] << "，请输入y确认：";

    if (scanf("%s", command) >= 1
            && (command[0] == 'y' || command[0] == 'Y')) {
        sprintf(command, "rm -r %s", argv[1]);
        //cout << "Executing " << command << endl;
        int result = system(command);
        if (result == 0) {
            cout << argv[1] << "数据库删除成功" << endl;
        } else {
            cout << "删除数据库" << argv[1] << "时发生错误" << endl;
        }
    }
    return 0;
}
