import pandas as pd
import numpy as np

## 随机生成'张三','李四','王五'的期中考试成绩数据
## 成绩包括'语文','英语','数学','python'，成绩分值在90至150之间
df1 = pd.DataFrame(np.random.randint(90,150,size=(3, 4)),columns=['语文', '英语', '数学', 'python'], index=['张三', '李四', '王五'])
print("随机生成期中考试成绩：\n",df1)

## 生成刘六的数据
df2 = pd.DataFrame(np.random.randint(90,150,size=(1, 4)),columns=['语文', '英语', '数学', 'python'], index=['刘六']) 
print("随机生成刘六的期中考试成绩：\n",df2)
df = pd.concat([df1,df2])
print("刘六加入以后的期中考试成绩：\n",df)

## 将语文改成java
df = df.rename(columns={'语文':'java'})
print("语文改成java后的期中考试成绩：\n",df)

## 增加C语言成绩
df3 = np.random.randint(70,90,size=(4, 1))
print("随机生成的c语言期中考试成绩：\n",df3)
df.insert(df.shape[1], 'c语言', df3)
print("增加c语言后的期中考试成绩：\n",df)

## 排序
df = df.sort_values(by='python',ascending=False)
print("按python成绩从高到底排序以后的期中考试成绩：\n",df)