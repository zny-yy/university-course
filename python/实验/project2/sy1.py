import numpy as np
sarray = np.random.randint(-10,10,size=(4, 4))
print('随机生成4*4 整数矩阵：\n',sarray)
sarray = np.triu(sarray)
print('变成上三角矩阵：\n',sarray)
sarray += sarray.T -np.diag(sarray.diagonal())
print('生成4*4 对称矩阵：\n',sarray)
np.savetxt("D:\sarrays.csv", sarray, fmt="%d", delimiter=",")
