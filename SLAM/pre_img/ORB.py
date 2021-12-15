import cv2 as cv
from matplotlib import pyplot as plt

# 1.图像读取
img = cv.imread("./3.jpg")
gray = cv.cvtColor(img, cv.COLOR_BGR2GRAY)

# 2.ORB角点检测
# 2.1实例化ORB对象
orb = cv.ORB_create(nfeatures=10000)

# 2.2 检测关键点，并计算特征描述符
kp, des = orb.detectAndCompute(gray, None)

print(des.shape)

# 3.将关键点检测在图像上
img2 = cv.drawKeypoints(img, kp, None, color=(255, 255, 0), flags=0) # 也可以添加 color 参数指定图像显示关键点的颜色，例如 img2 = cv.drawKeypoints(img, kp, None, color=(0, 0, 255), flags=0)

# 4.绘制图像
plt.figure(figsize=(10, 8), dpi=100)
plt.imshow(img2[:, :, ::-1])
plt.xticks([]),
plt.yticks([])
plt.show()