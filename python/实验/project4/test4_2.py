# （1） 数据获取及预处理
import pandas as pd
import matplotlib.pyplot as plt
import numpy as np
plt.rcParams['font.family'] = ['sans-serif'] # 解决中文字体显示问题
plt.rcParams['font.sans-serif'] = ['SimHei'] # 设置字体为黑体
import datetime
import statsmodels.api as sm

# 导入数据 通过pd形式读取数据
urumchi_data = pd.read_csv('wulumuqi.csv', encoding = 'utf-8', engine = 'python')
print('~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~')
print('原始数据为：',urumchi_data) # 打印原始数据
print('~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~')

# 获取 2018 年夏季数据的索引
startIndex = urumchi_data.index[urumchi_data['date'] == '2018-06-01'].tolist()[0]
endIndex = urumchi_data.index[urumchi_data['date'] == '2018-08-31'].tolist()[0]
# 获取2018年夏季的数据，6月到8月的数据作为训练模型
trainUrumchi_data = urumchi_data[startIndex: endIndex+1]
print(trainUrumchi_data)    # 打印6-8月将要进行训练的数据

dateData = trainUrumchi_data['date']
AQIData = trainUrumchi_data['AQI']


'''
白噪声检测
ljungbox_result为tuple格式数据得到的结果：
第1行为ljun-Box统计量
第2行为p值
如果p<0.05，拒绝原假设，说明原始序列存在相关性，则为非白噪声
如果p>=0.05,接受原假设，说明原始序列独立，纯随机，则为白噪声
'''
# 使用sm包中的acorr_ljungbox方法得到白噪声的判断结果
white_noise = sm.stats.diagnostic.acorr_ljungbox(AQIData, lags=1)
print(u'白噪声检测结果：', white_noise)    # 打印白噪声检测结果
# 值为8.4224768e-256， 因此为非白噪声
# 如果数值小于0.05 为为白噪声数字，可以进行预测，反之不行。
if white_noise.iloc[0,1] < 0.05:
    print(u'这是非白噪声')
else:
    print(u'这是白噪声')

# 模型定阶
# 对模型进行定阶，得出p与q的值
pmax = int(len(AQIData) / 10)  # 一般阶数不超过 length /10
qmax = int(len(AQIData) / 10)
bic_matrix = []
# 用迭代方式，以循环形式测试p，q合适的取值
for p in range(pmax + 1):
    temp = []
    for q in range(qmax + 1):
        try:
            # 由于原始数据平稳，因此差分d为0
            value = sm.tsa.ARIMA(AQIData, order=(p, 0, q)).fit().bic
            temp.append(value)
        except Exception as e:
            # print(e)
            temp.append(None)
    bic_matrix.append(temp)  # 追加合适数据到矩阵

bic_matrix = pd.DataFrame(bic_matrix) # 将其转换成Datafram 数据结构
print(bic_matrix)
# 先使用stack展平成一行数组类型数据， 然后使用idxmin 找出最小值的位置
p, q = bic_matrix.stack().astype(float).idxmax()
print(u'BIC 最小的p值 和 q值: %s，%s' % (p, q))          # BIC 最小的p值和q值

# 建立ARIMA模型
model = sm.tsa.ARIMA(AQIData, order=(p, 0, q)).fit()  # 通过ARIMA对AQIData数据进行0阶 模型建立
print(model.summary())                                # 打印数据模型结构

# 预测未来一月
# forecase是一个series类型，需要转换为array数据类型
predictions = model.forecast(30)      # 通过时序内容预测未来一个月数据
print(predictions)                    # 打印追加的编号和对应预测的数据
print(type(predictions))              # 打印predictions数据类型
print(np.array(predictions))          # 打印predictions数据第二列
# 预测结果为：
pre_result = np.array(predictions)    # 取第二列
print(u'预测结果为：\n', pre_result)    # 打印未来一个月AQI的预测结果