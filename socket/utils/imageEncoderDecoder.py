import cv2
import base64
import numpy as np

def encodeCVImageBase64(image, imgCVType=cv2.CV_8UC3):
    if imgCVType == cv2.CV_8UC3:
        #print("1")
        image = cv2.imencode('.png', image)[1]
    else:    
        image = cv2.imencode('.png', image, params=[imgCVType])[1]
    base64Data = str(base64.b64encode(image))[2:-1]

    return base64Data

def decodeCVImageBase64(base64Data, imgCVType=cv2.IMREAD_COLOR):
    
    imgData = base64.b64decode(base64Data)
    nparr = np.fromstring(imgData, np.uint8)
    image = cv2.imdecode(nparr, imgCVType)

    return image