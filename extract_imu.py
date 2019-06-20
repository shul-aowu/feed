# coding=utf-8
from __future__ import print_function
#强制浮点除法
from __future__ import division
#Argparse的作用就是为py文件封装好可以选择的参数，使他们更加灵活，丰富 -h,-l等
import argparse
#用于操作迭代对象的函数
import itertools
#使用dict时，如果引用的Key不存在，就会抛出KeyError。如果希望key不存在时，返回一个默认值，就可以用
from collections import defaultdict
#它是一个由多维数组对象和用于处理数组的例程集合组成的库。
import numpy as np
#pyplot模块的plot函数可以接收输入参数和输出参数，还有线条粗细等参数
import matplotlib.pyplot as plt

INVALID_PITCH_ROLL = -1000000.0

PI = 3.1415926

class Location:
    def __init__(self, name, color, is_valid_idx, data_scope):
        self.enabled = True
        self.name = name
        self.is_valid_idx = is_valid_idx
        self.data_scope = data_scope

        self.linestyle=' '
        self.marker='x'
        self.ori_marker='o'
        self.color=color

    def set_valid_data(self, navi_input):
        self.valid_data = np.array(navi_input[:, self.data_scope[0]:self.data_scope[-1]], dtype='float')

    def set_invalid_idx(self, idx):
        self.invalid_idx = idx

#显示数据 无需时延补偿
# show the data without delay compansation
show_original_meas_data = False

# figure style  颜色
can_fig_style = 'b'
navi_fig_style = 'r.'

#axis limit control 坐标限制
extended_axis_range = 50.0
#####################################################
#function used to mark the hmi reset point 函数用于标记hmi复位点
def markReset(navi_data):
    for i in hmi_arr:
#在图形中，通过text添加纯文本的注释 数据，文字大小
        plt.text(i, navi_data[i], '|', fontsize=10)
#####################################################
# load log files
parser = argparse.ArgumentParser()#创建一个解析对象
parser.add_argument('log_fname', nargs='?', default='uos_navigation.log.bak.20180101015809')#向该对象中添加你要关注的命令行参数和选项
args = parser.parse_args()#进行解析
log_version = ''

#data set one 数据集
navi_log_fname = args.log_fname
navi_log_input = []
navi_log_output = []


#打开文件log_file = 文件名
with open(navi_log_fname) as log_file:
    for line in log_file:
        segments = line.split(':')#对于每一行，以：作为分隔符，每一节存入segment数组，在实际数据中，其实就是两节，一个是navi_log_input_data，另一个是数据
        if segments[0] == 'navi_log_input_data':
            navi_log_input.append(map(float, segments[1].split()[7:20]))
#map函数，对数据做处理，这里强制转换喂float类型数据
#append函数，增加数据，这里是把float类型的数据放入navi_log_input 中
navi_log_input = np.array(navi_log_input)#把数据弄成数组的形式
of = open('imu.txt', 'w')#创建新文件或打开文件，写入内容，若已存在，原内容删除在写入。
for line in navi_log_input:#对于该数据，准备输入
    of.write('ts: {:.4f}  ang: {:.4f} {:.4f} {:.4f}   vel: {:.4f} {:.4f} {:.4f} acc: {:.4f} {:.4f}  {:.4f} state: {:d}\n'.format(
        line[0], line[2], line[3], line[4], line[5], line[6], line[7], line[8],
        line[9], line[10], int(line[12])))#对应输入数据
of.close()
