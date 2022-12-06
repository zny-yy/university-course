# 导入opencv包
import cv2 as cv

# 设置相机模块
cap = cv.VideoCapture(0)                                     # 使用设备编号为0的摄像头进行返回视频数据
fourcc = cv.VideoWriter_fourcc(*'XVID')                      # 设置视频编码格式
out = cv.VideoWriter('test.avi', fourcc, 20.0, (640, 480))  # 设置视频输出位置及格式avi，编码方式，视频帧数，视频分辨率

# 视频录制
while(True):                                                 # 通过循环方式进行视频采集
    ret, frame = cap.read()                                  # 从cap循环中读取视频帧
    out.write(frame)                                         # 将视频帧写入到需要保存的视频文件
    cv.imshow('frame', frame)                                # 将视频帧在窗口显示
    if cv.waitKey(1) & 0xFF == ord('q'):                     # 通过键盘事件点击q时，跳出视频采集
        break                                                # 中断，跳出循环

cap.release()                                                # 释放摄像头
out.release()                                                # 释放视频流
cv.destroyAllWindows()                                       # 关闭所有窗口