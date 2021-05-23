import pyqtgraph as pg
import pyqtgraph.exporters
import array
import serial
import threading
import numpy as np
from queue import Queue
import time

import sys
import random
from PyQt5.QtCore import Qt
from PyQt5.QtWidgets import QApplication, QWidget, QVBoxLayout, QMainWindow, QTextBrowser, QLabel, QLineEdit\
    , QPushButton
from PyQt5.QtGui import QIcon, QFont, QPixmap, QPalette


i = 0
Pulse_1_Queue = Queue(maxsize=0)
Pulse_2_Queue = Queue(maxsize=0)
Envelop_1_Queue = Queue(maxsize=0)
Data_Pulse_1 = array.array('i')  # 可动态改变数组的大小,double型数组
Data_Pulse_2 = array.array('i')  # 可动态改变数组的大小,double型数组
Data_Envelop_1 = array.array('i')

BPM1 = ""
IBI1 = ""
imageName=""

historyLength = 200  # 横坐标长度

Data_Pulse_1 = np.zeros(historyLength).__array__('d')  # 把数组长度定下来
Data_Pulse_2 = np.zeros(historyLength).__array__('d')  # 把数组长度定下来
Data_Envelop_1 = np.zeros(historyLength).__array__('d')




class Win(QWidget):
    def __init__(self):
        super(Win,self).__init__()
        self.setWindowTitle("脉搏波检测手套上位机程序")
        self.resize(1800,900)

        self.pw = pg.PlotWidget(self)  # 创建一个绘图控件
        #要将pyqtgraph的图形添加到pyqt5的部件中，我们首先要做的就是将pyqtgraph的绘图方式由window改为widget。PlotWidget方法就是通过widget方法进行绘图的
        self.pw.resize(900,600)
        self.pw.move(10,10)
        self.pw.showGrid(x=True, y=True)  # 把X和Y的表格打开
        self.pw.setRange(xRange=[0, historyLength], yRange=[300, 850], padding=0)
        self.Curve_Pulse_1 = self.pw.plot(Data_Pulse_1, pen='r')  # 在绘图控件中绘制图形
        self.Curve_Pulse_2 = self.pw.plot(Data_Pulse_2, pen='g')  # 在绘图控件中绘制图形

        #self.Curve_Envelop_1 = self.pw.plot(Data_Envelop_1)  # 在绘图控件中绘制图形

        self.L_BPM1 = QLabel(self)
        self.L_BPM1.setText("BPM")
        self.L_BPM1.resize(400, 50)
        self.L_BPM1.move(1000, 10)
        self.L_BPM1.setStyleSheet("QLabel{color:rgb(255,22,22,255);font-size:50px;font-weight:normal;font-family:Arial;}")


        self.L_IBI1 = QLabel(self)
        self.L_IBI1.setText("IBI(ms)")
        self.L_IBI1.resize(400, 50)
        self.L_IBI1.move(1000, 70)
        self.L_IBI1.setStyleSheet("QLabel{color:rgb(255,22,22,255);font-size:50px;font-weight:normal;font-family:Arial;}")

        self.L_BPM2 = QLabel(self)
        self.L_BPM2.setText("BPM")
        self.L_BPM2.resize(400, 50)
        self.L_BPM2.move(1000, 130)
        self.L_BPM2.setStyleSheet(
            "QLabel{color:rgb(127,255,0,255);font-size:50px;font-weight:normal;font-family:Arial;}")

        self.L_IBI2 = QLabel(self)
        self.L_IBI2.setText("IBI(ms)")
        self.L_IBI2.resize(400, 50)
        self.L_IBI2.move(1000, 190)
        self.L_IBI2.setStyleSheet(
            "QLabel{color:rgb(127,255,0,255);font-size:50px;font-weight:normal;font-family:Arial;}")

        self.LE_Name = QLineEdit(self)
        self.LE_Name.setText("输入图像备注")
        self.LE_Name.resize(400, 50)
        self.LE_Name.move(1000, 250)
        self.LE_Name.setStyleSheet("QLineEdit{font-size:30px;font-weight:normal;font-family:Arial;}")

        self.B_SaveImage = QPushButton(self)
        self.B_SaveImage.setText("保存图像")
        self.B_SaveImage.resize(200, 50)
        self.B_SaveImage.move(1420, 250)
        self.B_SaveImage.setStyleSheet("QPushButton{font-size:30px;font-weight:normal;font-family: 微软雅黑;}")
        self.B_SaveImage.clicked.connect(self.saveImage)









    def plotData(self):
        global i;
        if i < historyLength:
            Data_Pulse_1[i] = Pulse_1_Queue.get()
            Data_Pulse_2[i] = Pulse_2_Queue.get()
            #Data_Envelop_1[i] = Envelop_1_Queue.get()
            i = i + 1
        else:
            Data_Pulse_1[:-1] = Data_Pulse_1[1:]
            Data_Pulse_1[i - 1] = Pulse_1_Queue.get()
            Data_Pulse_2[:-1] = Data_Pulse_2[1:]
            Data_Pulse_2[i - 1] = Pulse_2_Queue.get()
            # Data_Envelop_1[:-1] = Data_Envelop_1[1:]
            # Data_Envelop_1[i - 1] = Envelop_1_Queue.get()
        self.Curve_Pulse_1.setData(Data_Pulse_1)
        self.Curve_Pulse_2.setData(Data_Pulse_2)
        #self.Curve_Envelop_1.setData(Data_Envelop_1)



    def Serial(self):
        global i;
        global q;
        while (True):
            n = mSerial.inWaiting()

            if (n):
                dat = mSerial.readline()
                # print(dat)
                if (dat == b'P1\r\n'):
                    dat = mSerial.readline()
                    Pulse_1_Queue.put(dat)
                # elif (dat == b'E1\r\n'):
                #     dat = mSerial.readline()
                #     Envelop_1_Queue.put(dat)
                elif (dat == b'P2\r\n'):
                    dat = mSerial.readline()
                    Pulse_2_Queue.put(dat)
                elif (dat == b'B1\r\n'):
                    dat = mSerial.readline()
                    BPM1 = dat.decode()
                    # print(BPM1)
                    self.L_BPM1.setText("BPM  "+BPM1)

                elif (dat == b'Q1\r\n'):
                    dat = mSerial.readline()
                    IBI1 = bytes.decode(dat)
                    self.L_IBI1.setText("IBI(ms)  " + IBI1)
                elif (dat == b'B2\r\n'):
                    dat = mSerial.readline()
                    BPM1 = dat.decode()
                    # print(BPM1)
                    self.L_BPM2.setText("BPM  "+BPM1)

                elif (dat == b'Q2\r\n'):
                    dat = mSerial.readline()
                    IBI1 = bytes.decode(dat)
                    self.L_IBI2.setText("IBI(ms)  " + IBI1)
                else:
                    dat = mSerial.readline()



    def saveImage(self):
        imageName = self.LE_Name.text()
        self.ex = pyqtgraph.exporters.ImageExporter(self.pw.scene())
        self.ex.export(fileName="./output/"+imageName+BPM1+".png")





if __name__ == "__main__":

    app = QApplication(sys.argv)

    w = Win()


    portx = 'COM5'
    bps = 115200
    # 串口执行到这已经打开 再用open命令会报错
    mSerial = serial.Serial(portx, int(bps))
    if (mSerial.isOpen()):
        dat = 0xff;
        dat >> 2;
        print("open success")
        # 向端口些数据 字符串必须译码
        mSerial.write("hello".encode())
        mSerial.flushInput()  # 清空缓冲区
    else:
        print("open failed")
        serial.close()  # 关闭端口
    th1 = threading.Thread(target=w.Serial)
    th1.start()

    timer = pg.QtCore.QTimer()
    timer.timeout.connect(w.plotData)  # 定时刷新数据显示
    timer.start(1)  # 多少ms调用一次

    w.show()



    sys.exit(app.exec_())
