#!/usr/bin/python3
import os
import sys
from pathlib import Path 

if len(sys.argv) == 1:
    print("Usage: ./namer.py <image folder>", file=sys.stderr)
    exit(1)

def is_image(path):
    imgext = {".jpg", ".JPG"}
    ext = os.path.splitext(path)[-1]
    return ext in imgext


folder = sys.argv[1] if sys.argv[1][-1] == '/' else sys.argv[1] + '/'
files = [item for item in os.listdir(folder) if is_image(item)]

parent_folder = folder + "../"
train_filename = parent_folder + "/train.txt"
with open(train_filename, mode="w+", encoding="utf8") as f:
    for image_name in files:
        f.write(folder + image_name + '\n')

print("Done!")
