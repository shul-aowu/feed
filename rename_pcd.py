#!/usr/bin/env python
from __future__ import print_function
import os
from os.path import join, exists, abspath
import sys

if len(sys.argv) < 2:
    print('Usage: rename_pcd.py 0727_1')
    exit(0)
root = sys.argv[1]

tss = []
state_fn = join(root, 'ml_lidar_state')
assert exists(state_fn), 'Error: {} not exists!'.format(state_fn)
for l in open(state_fn):
    print(l)
    tss.append(l.split()[0])

renamed_dir = join(root, 'renamed_pcd')
assert not exists(renamed_dir), 'Error: {} exists! Please remove it.'.format(renamed_dir)
os.mkdir(renamed_dir)

for i, ts in enumerate(tss):
    src_fn = abspath(join(root , 'ml_lidar_raw_1_{:06d}.pcd'.format(i)))
    assert exists(src_fn), 'Error: {} not exists!'.format(src_fn)
    dst_fn = join(renamed_dir, ts + '000.pcd')
    print(dst_fn)
    os.system('ln -s {} {}'.format(src_fn, dst_fn))
