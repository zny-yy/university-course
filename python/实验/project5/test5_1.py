# 手写数字识别

import numpy as np
import os


class DigitRecoginze():

    def __init__(self):
        self.label = None
        self.train_set = None
        self.test_set = None

    def img2vector(self, filename):
        # 将像素图像转化为向量
        # 图像像素为32*32
        image_vector = np.zeros((1, 1024))
        f = open(filename, 'r')
        for i in range(32):
            line = f.readline()
            for j in range(32):
                image_vector[0, 32 * i + j] = int(line[j])

        return image_vector

    def import_data(self, filepath):
        # 导入数据
        data_list = os.listdir(filepath)
        data_list_number = len(data_list)

        # 导入label数据
        return_label = np.zeros((data_list_number, 1))
        for i in range(data_list_number):
            return_label[i] = (data_list[i].strip().split('_'))[0]
        # 导入data数据
        return_data_set = np.zeros((data_list_number, 1024))
        for i in range(data_list_number):
            return_data_set[i] = self.img2vector(os.path.join(filepath, data_list[i]))

        return return_data_set, return_label

    def train_set_normalize(self, train_set):
        # 归一化
        data_range = np.max(train_set) - np.min(train_set)
        return (train_set - np.min(train_set)) / data_range

    def single_train(self, train_set, testcase_x, train_label, k=5):

        # 计算距离
        train_set_size = train_set.shape[0]
        diff_mat = np.tile(testcase_x, (train_set_size, 1)) - train_set
        distances = (diff_mat ** 2).sum(axis=1) ** 0.5
        # print(distances)

        # 排序，这里排序结果表示他的排序位置
        distances_sorted = distances.argsort()

        class_result = {}

        # 找出k个点
        for i in range(k):
            now_label = int(train_label[distances_sorted[i]][0])
            # print(now_label)
            class_result[now_label] = class_result.get(now_label, 0) + 1

        # 找出最近最多的点
        max_num = 0
        result_label = 0
        for single_result in class_result:
            if class_result[single_result] > max_num:
                max_num = class_result[single_result]
                result_label = single_result
        return result_label

    # 训练
    def test(self, train_set_filepath, test_set_filepath, k=5):
        # 导入数据
        train_set, train_label = self.import_data(train_set_filepath)
        train_set = self.train_set_normalize(train_set)
        test_set, test_label = self.import_data(test_set_filepath)
        test_set = self.train_set_normalize(test_set)

        error_number = 0
        all_number = test_set.shape[0]
        # 对于每一个测试样本进行测试
        for i in range(all_number):
            result_label = self.single_train(train_set, test_set[i, :], train_label, k)
            if result_label != int(test_label[i][0]):
                error_number = error_number + 1

            print("testnum %d: the result is: %d, the real answer is: %d" % (i, result_label, int(test_label[i][0])))
        print("The total number of errors is : %i" % error_number)
        print("error ratio = %.2f %%" % (float(all_number-error_number) / float(all_number)*100))


FILE_PATH_TEST = r'HWdigits\testSet'
FILE_PATH_TRAIN = r'HWdigits\trainSet'
_dr = DigitRecoginze()
_dr.test(FILE_PATH_TRAIN, FILE_PATH_TEST)
