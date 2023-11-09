#!/usr/bin/env python3
import cv2 as cv
import os
import numpy as np
import random
import argparse
from PIL import Image
import subprocess

#region get_argument_data
parser = argparse.ArgumentParser(formatter_class=argparse.ArgumentDefaultsHelpFormatter)
parser.add_argument("project_name", help="Name of your project")
parser.add_argument("train_ratio",type=float,default=0.75, help="ratio of train dataset")
parser.print_help
args=parser.parse_args()
#endregion


image_dir = f'data/{args.project_name}/all/images'
train_img_dir = f'data/{args.project_name}/train/images/cropped'
train_txt_dir = f'data/{args.project_name}/train/text'
test_img_dir = f'data/{args.project_name}/test/images'
test_txt_dir = f'data/{args.project_name}/test/text'
ground_text = f'data/{args.project_name}/all/text/gt.txt'
file_list=os.listdir(image_dir)

image_name = file_list[0].split('_')[0]
image_surname = file_list[0].split('.')[1]
train_length=int(len(file_list)*args.train_ratio)

char_list=[]
all_list=[]
train_list=[]
text_list=[]


with open(ground_text, 'r') as input_file:
	for line in input_file.readlines():
		char_list.append(line.strip())

for x in range(len(file_list)):
            all_list.append(x)

random.shuffle(all_list)
train_list=all_list[:train_length]
test_list=all_list[train_length:]

print(char_list)

for i in range(len(train_list)):
            image_path=f'{image_dir}/{image_name}_{train_list[i]}.{image_surname}'
            image=cv.imread(image_path)
            print(image_path)
            cv.imwrite(os.path.join(f'{train_img_dir}/{image_name}_{i}.{image_surname}'), image)
            ground_truth_text = os.path.join(train_txt_dir, f'{image_name}_{i}.gt.txt')
            with open(ground_truth_text, 'w') as output_file:
                    output_file.writelines(char_list[train_list[i]])

for i in range(len(test_list)):
            image_path=f'{image_dir}/{image_name}_{test_list[i]}.{image_surname}'
            image=cv.imread(image_path)
            cv.imwrite(os.path.join(f'{test_img_dir}/{image_name}_{i}.{image_surname}'), image)
            ground_truth_text = os.path.join(test_txt_dir, f'{image_name}_{i}.gt.txt')
            with open(ground_truth_text, 'w') as output_file:
                    output_file.writelines(char_list[test_list[i]])           
        