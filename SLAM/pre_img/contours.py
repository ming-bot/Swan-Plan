import cv2

img = cv2.imread('./3.jpg', flags=cv2.IMREAD_GRAYSCALE)
GBlur = cv2.GaussianBlur(img, (3, 3), 0)
canny = cv2.Canny(GBlur, 50, 150)
cv2.namedWindow("img", 0)
cv2.namedWindow("canny", 0)
cv2.imshow('img', img)
cv2.imshow('canny', canny)
cv2.waitKey(0)
cv2.destroyAllWindows()