'''
File: _Dictionary_ 
Autor: yuen
time: 2022/10/2
'''
from typing import Counter

txt = open('data.txt').read()
print(type(txt))
for s in " , . \n ' ": # 去除文档中标点符号
    txt = txt.replace(s, "")

def count_way1(): # 字典特性查询字母出现次数
    count = {}
    for i in txt:
        if i not in count:
            count[i] = 1
        else:
            count[i] = count[i] + 1
    print('字典方式计算data.txt字母统计：\n',count)

def count_way2 (): #counter函数统计字母次数
    count1 = Counter(txt)
    print('counter函数方式字母统计：\n',count1)

def count_way3(): # setdefault函数统计字母次数
    count2 = {}
    for char in txt:
        count2.setdefault(char, 0)
        count2[char] += 1
    print('setdefault函数方式字母统计：\n',count2) 

count_way1()
count_way2()
count_way3()