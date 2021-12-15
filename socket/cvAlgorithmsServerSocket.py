import os
import socket
import time
import cv2
import numpy as np

import json
import grpc

from utils import imageEncoderDecoder
import planeExtractor

def recvall(sock, count):
    #print(count)
    buf = b''
    while count:
        newbuf = sock.recv(count)
        #print(count)
        #print(newbuf)
        if not newbuf and buf == b'':
            return None
        elif not newbuf and buf != b'':
            return buf
        buf += newbuf
        count -= len(newbuf)

    return buf

def tcpServer():
    host = "10.1.100.66"
    port = 8004

    s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    s.bind((host, port))
    s.listen(True)

    while 1:
        conn, address = s.accept()
        print("connection from ", str(address))
    
        depthLength = recvall(conn, 16)
        depthLength = depthLength.decode()

        if depthLength == "close           ":
            s.close()
            break
        
        depthData = recvall(conn, int(depthLength)).decode()
        colorLength = recvall(conn, 16).decode()
        colorData = recvall(conn, int(colorLength)).decode()
        jsonLength = recvall(conn, 16).decode()
        jsonData = recvall(conn, int(jsonLength)).decode()

        print(colorLength)
        #print(colorData)

        imgDepth = imageEncoderDecoder.decodeCVImageBase64(depthData, imgCVType=cv2.IMREAD_ANYDEPTH)
        imgColor = imageEncoderDecoder.decodeCVImageBase64(colorData)

        dictParams = json.loads(jsonData)
        listAccel = dictParams["listAccel"]
        listGyro = dictParams["listGyro"]
        listIntrins = dictParams["listIntrins"]
        imgFeatures = planeExtractor.processPlaneExtraction(imgColor, \
                                                            imgDepth, \
                                                            listAccel, \
                                                            listGyro, \
                                                            listIntrins)

        results = imgFeatures.getVecRotatedCenters()
        conn.send(str(len(results)).ljust(20).encode())
        
        for result in results:
            for para in result:
                conn.send(str(para).ljust(20).encode())
    
    s.close()

if __name__ == "__main__":
    tcpServer()