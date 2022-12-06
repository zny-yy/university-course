## 以问题为主时，使用此程序
import numpy as np
my_array = np.array([[1, 2, 3], [4, 5, 6], [7, 8, 9]])
print("原数组：\n", my_array)
print("第一列数组：\n", my_array[: ,0])
print("第三行数组：\n", my_array[2, :])
print("第一三行合并数组方式一：\n", np.vstack((my_array[: ,0], my_array[2, :])))
print("第一三行合并数组方式二：\n", np.concatenate((my_array[: ,0], my_array[2, :]), axis = 0))

## 以答案为主时，使用此程序
import numpy as np
my_array = np.array([[1, 2, 3], [4, 5, 6], [7, 8, 9]])
print("原数组：\n", my_array)
print("第一列数组：\n", my_array[: ,0])
print("第三行数组：\n", my_array[2, :])
print("第一三行合并数组方式一：\n", np.vstack((my_array[0 ,:], my_array[2, :])))
print("第一三行合并数组方式二：\n", np.concatenate((my_array[0 ,:], my_array[2, :]), axis = 0))

