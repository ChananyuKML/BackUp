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
parser.add_argument("work_topic", default="tamako", help='Name of folder in work space')
parser.add_argument('-v','--IMAGE_INV', default=False, help='set IMAGE_INV=True if you willing to invert image grayscale')
parser.print_help()
args = parser.parse_args()
#endregion


crop_dir = f'data/{args.work_topic}/train/images/cropped'
gray_dir = f'data/{args.work_topic}/train/images/cropped_gray'
bw_dir = f'data/{args.work_topic}/train/images/cropped_bw'
ground_text = f'data/{args.work_topic}/train/text'
ground_truth_dir = f'data/{args.work_topic}/train/training_data/{args.work_topic}-ground-truth'
img_list = os.listdir(crop_dir)
img_name = img_list[0].split('_')[0]
img_surname = img_list[0].split('.')[1]
char_list=[]
kernel = np.ones((3, 3), np.uint8)
#read prepared text file for making ground truth file
text_list=os.listdir(ground_text)

kernel = cv.getStructuringElement(cv.MORPH_ELLIPSE,(3,3))
data_size = len(img_list)
batch_numbers = 10000//data_size
for i in range(data_size):
	img_path = f'{crop_dir}/{img_name}_{i}.{img_surname}'		
	print(img_path)
	img = cv.imread(img_path)
	blur = cv.blur(img,(1,1),0)
	gray = cv.cvtColor(img, cv.COLOR_BGR2GRAY)
	if args.IMAGE_INV == True:
		gray = abs(255-gray)
	else:
		gray = gray
	cv.imwrite(os.path.join(f'{gray_dir}/{img_name}_{i}.tif'), gray)

gray_list = os.listdir(gray_dir)
gray_name = gray_list[0].split('_')[0]
subprocess.run(["mkdir",ground_truth_dir])

for k in range(batch_numbers):
	for i in range(data_size):
		count = i
		gray_path = f'{gray_dir}/{img_name}_{i}.tif'
		gray = cv.imread(gray_path)
		gray = cv.cvtColor(gray, cv.COLOR_BGR2GRAY)
		th = random.randint(int(np.mean(gray)/64),int(np.mean(gray)/8))
		k_min = int(gray.shape[1]/2)
		if k_min%2==1:
			k_min = k_min
		else:
			k_min = k_min+1
		print(k_min)
		k_size = random.randrange(k_min,int(gray.shape[1]*2)+1,2)
		print(int((gray.shape[1]*2))+1)
		print(k_size)	
		th3 = cv.adaptiveThreshold(gray,255,cv.ADAPTIVE_THRESH_GAUSSIAN_C,cv.THRESH_BINARY,k_size,th)
		cv.imwrite(os.path.join(f'{bw_dir}/{img_name}_{(k*data_size)+i}.tif'), th3)
		cv.imwrite(os.path.join(f'{ground_truth_dir}/{img_name}_{(k*data_size)+i}.tif'), th3)
		ground_truth_file = f'{ground_text}/{img_name}_{i}.gt.txt'
		with open(ground_truth_file, 'r') as input_file:
			for line in input_file:
				char=line.strip()
		ground_truth_text = os.path.join(ground_truth_dir, f'{img_name}_{(k*data_size)+i}.gt.txt')
		with open(ground_truth_text, 'w') as output_file:
			output_file.writelines(char)
		print(char)
		print(f'saved image {img_name}_{(k*data_size)+i}.tif')