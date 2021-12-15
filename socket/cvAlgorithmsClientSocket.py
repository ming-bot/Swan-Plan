import os
import socket
import cv2
import numpy as np

import json

from utils import imageEncoderDecoder

FRAME_PATH = os.path.join("frames")
imgColor = cv2.imread(os.path.join(FRAME_PATH, "colorImage.png"), cv2.IMREAD_COLOR)
imgDepth = cv2.imread(os.path.join(FRAME_PATH, "depthImage.png"), cv2.IMREAD_ANYDEPTH)

listAccel = []

with open(os.path.join(FRAME_PATH, "vecAccel.txt"), "r") as f:
    lines = f.readlines()
    for line in lines:
        line = line[:len(line) - 1]
        listAccel.append(float(line))

listGyro = []

with open(os.path.join(FRAME_PATH, "vecGyro.txt"), "r") as f:
    lines = f.readlines()
    for line in lines:
        line = line[:len(line) - 1]
        listGyro.append(float(line))

listIntrins = []

with open(os.path.join(FRAME_PATH, "intrins.txt")) as f:
    lines = f.readlines()
    for line in lines:
        line = line[:len(line) - 1]
        listIntrins.append(float(line))

TCP_IP = '10.1.100.66'
TCP_PORT = 8004

sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
sock.connect((TCP_IP, TCP_PORT))

depthData = imageEncoderDecoder.encodeCVImageBase64(imgDepth, imgCVType=cv2.CV_16U)
colorData = imageEncoderDecoder.encodeCVImageBase64(imgColor)

dictParams = {}
dictParams["listAccel"] = listAccel
dictParams["listGyro"] = listGyro
dictParams["listIntrins"] = listIntrins

jsonParams = json.dumps(dictParams)
#print(jsonParams)
#print(type(jsonParams))

#print(str(len(depthData)).ljust(16).encode())
#print(str(len(colorData)).ljust(16).encode())

sock.send(str("close").ljust(16).encode())
pass

'''
sock.send(str(len(depthData)).ljust(16).encode())
sock.send(depthData.encode())
sock.send(str(len(colorData)).ljust(16).encode())
sock.send(colorData.encode())
sock.send(str(len(jsonParams)).ljust(16).encode())
sock.send(jsonParams.encode())
sock.close()
'''

#decimg = imageEncoderDecoder.decodeCVImageBase64(depthData)
#cv2.imshow('Client', decimg)
#cv2.waitKey(0)
#cv2.destroyAllWindows()
