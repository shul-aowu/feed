from __future__ import print_function
from __future__ import division

import argparse
import itertools
from collections import defaultdict

import numpy as np
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


# show the data without delay compansation
show_original_meas_data = False

# figure style
can_fig_style = 'b'
navi_fig_style = 'r.'

#axis limit control
extended_axis_range = 50.0
#####################################################
#function used to mark the hmi reset point
def markReset(navi_data):
    for i in hmi_arr:
        plt.text(i, navi_data[i], '|', fontsize=10)
#####################################################
# load log files
parser = argparse.ArgumentParser()
parser.add_argument('log_fname', nargs='?', default='uos_navigation.log.bak.20180101015809')
args = parser.parse_args()
log_version = ''

#data set one
navi_log_fname = args.log_fname
navi_log_input = []
navi_log_output = []



with open(navi_log_fname) as log_file:
    for line in log_file:
        segments = line.split(':')
        if segments[0] == 'navi_log_input_data':
            navi_log_input.append(map(float, segments[1].split()[7:20]))

navi_log_input = np.array(navi_log_input)
of = open('imu.txt', 'w')
for line in navi_log_input:
    of.write('ts: {:.4f}  ang: {:.4f} {:.4f} {:.4f}   vel: {:.4f} {:.4f} {:.4f} acc: {:.4f} {:.4f}  {:.4f} state: {:d}\n'.format(
        line[0], line[2], line[3], line[4], line[5], line[6], line[7], line[8],
        line[9], line[10], int(line[12])))
of.close()
