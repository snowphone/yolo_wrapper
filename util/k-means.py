#!/usr/bin/python3

from sys import argv, exit
import os

if len(argv) != 2:
    print("Usage: ./k-means.py <.data>")
    exit(1)


commands = [
        "./darknet",
        "detector",
        "calc_anchors",
        argv[1],
        "-num_of_clusters",
        "9",
        "-width",
        "416",
        "-height",
        "416"
        ]

os.system(" ".join(commands))
