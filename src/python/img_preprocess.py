#!/usr/bin/env python3
import cv2 as cv
import os
import numpy as np
import random
import argparse
from PIL import Image
import subprocess


# region Define Variable with argparser
parser = argparse.ArgumentParser(
    prog='PROG',
    formatter_class=argparse.ArgumentDefaultsHelpFormatter)
parser.add_argument("work_topic", default="example", help='Name of folder in work space')
parser.add_argument("-m", "--MODE", default="bw", help='set of data that you classify (should be the type number that you name to folder')
parser.add_argument('-v','--IMAGE_INV', default=False, help='set IMAGE_INV=True if you willing to invert image grayscale')
args=parser.parse_args()
#endregion
img_list=[]
img_dir = f'data/{args.work_topic}/test/images'
list= os.listdir(img_dir)
for i in range(len(list)):
	if list[i].split('.')[1] == "tif":
		img_list.append(list[i])
	elif list[i].split('.')[1] == "jpg":
		img_list.append(list[i])
	elif list[i].split('.')[1] == "jpeg":
		img_list.append(list[i])
	elif list[i].split('.')[1] == "png":
		img_list.append(list[i])
	elif list[i].split('.')[1] == "tiff":
		img_list.append(list[i])		
data_size = len(img_list)
print(data_size)
Mode=args.MODE
print(Mode)
for i in range(data_size):		
	img_path = f'{img_dir}/{img_list[i]}'
	img_name = f'{img_list[i]}_{args.MODE}'
	img = cv.imread(img_path)
	gray = cv.cvtColor(img, cv.COLOR_BGR2GRAY)
	th = (np.mean(gray))/20
	k_size=int(gray.shape[1])	
	if k_size%2==1:
		k_size = k_size
	else:
		k_size = k_size+1
		
	inv_gray=abs(255-gray)
	if args.IMAGE_INV=="True":
		gray = inv_gray
	elif args.IMAGE_INV=="False":
		gray = gray

	th3 = cv.adaptiveThreshold(gray,255,cv.ADAPTIVE_THRESH_GAUSSIAN_C,cv.THRESH_BINARY,k_size,th)
	if Mode =="bw":
		cv.imwrite(os.path.join(f'{img_dir}/{img_name}.tif'), th3)
		print(f'{img_dir}/{img_name}_bw.tif')
	elif Mode =="gray":
		cv.imwrite(os.path.join(f'{img_dir}/{img_name}.tif'), gray)
    	
	print(f'saved image {img_name} at {img_path}')
