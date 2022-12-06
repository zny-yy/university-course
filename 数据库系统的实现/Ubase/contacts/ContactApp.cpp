
#include <iostream>

#include "ContactHashTable.h"
int main() {

    ContactHashTable* contacts = new ContactHashTable();

    Person* person = new Person("张一", "13345566232");
    contacts->insert(person);

    person = new Person("王五", "14230036000");
    contacts->insert(person);

    person = new Person("周董", "15022334567");
    contacts->insert(person);

    person = new Person("谢天", "13800023232");
    contacts->insert(person);

    person = new Person("马丽", "13144556789");
    contacts->insert(person);

    Person* interestPerson;
    contacts->find("王五", interestPerson);
    cout << *interestPerson << endl;

    contacts->find("周董", interestPerson);
    cout << *interestPerson << endl;

    contacts->find("谢天", interestPerson);
    cout << *interestPerson << endl;

    contacts->erase("王五");
    contacts->find("王五", interestPerson);
    //输出联系人对象指针
    cout << interestPerson << endl;

    contacts->listHashTable();
    cout << endl << "恭喜：所有测试顺利通过." << endl;
    return (0);
}
