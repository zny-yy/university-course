import numpy as np
my_array = np.array([[1, 2, 3], [4, 5, 6], [7, 8, 9]])
print("原数组为：\n",my_array)
print("第一个对角线元素：\n", my_array[0:1, 0:1])
print("第二个对角线元素：\n", my_array[1:2, 1:2])
print("第三个对角线元素：\n", my_array[2:3, 2:3])
my_array = np.diagonal(my_array)
print("对角线元素汇总：\n", my_array)