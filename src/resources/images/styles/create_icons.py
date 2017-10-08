#!/usr/bin/python

import os
origin_dir = './black/';
colors_dirs = [('#3498db', './blue/'), ('#BDC3C7', './gray/'), ('#F7A729', './orange/')];
dirs = ['blue', 'gray', 'orange'];

def change_image_color_in_dir(directory, color):
    for file_name in os.listdir(directory):
        full_file_name = directory + file_name;
        if os.path.isdir(full_file_name):
            change_image_color_in_dir(full_file_name+"/", color)
        else:
            os.system("convert " + full_file_name +" -fuzz 100% -fill '" + color + "' -opaque white " + full_file_name)


for color, directory in colors_dirs:
    os.system('rm -rf ' + directory)
    os.system('cp -r ' + origin_dir + " " + directory)
    change_image_color_in_dir(directory, color)
