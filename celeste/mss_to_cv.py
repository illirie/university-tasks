from mss import mss
from PIL import Image
import numpy as np
import cv2 as cv
import time

def get_all_colors(img):
    res = set()
    for i in img:
        for j in i:
            res.add(tuple(j))
    print(res)

def compose_bg_matrix(img):
    #bg = [[0] * bounding_box['width']] * bounding_box['height']
    bg1 = []
    for y in range(bounding_box['height']//3):
        col_bg = []
        for x in range(bounding_box['width']//3):
            if img[y][x][0] == img[y][x][1] and img[y][x][1] == img[y][x][2]:
                # print(img.pixel(x, y), ' ', x, ' ', y)
                col_bg.append(0)
                #bg[y][x] = 1
            else:
                # print(img.pixel(x, y), ' ', x, ' ', y)
                col_bg.append(1)
                # bg[y][x] = 0
        bg1.append(col_bg)
    # for i in bg:
    #     print(i)
    # print('  ')
    # for i in bg1:
    #     print(i[:5])
    return bg1

bounding_box = {'top': 40, 'left': 0, 'width': 955, 'height': 535}
template = cv.imread('char.png', cv.IMREAD_COLOR)
sct = mss()
sct_img = sct.grab(bounding_box)

start = time.time()
pixels = np.array(sct_img)
pixels = cv.cvtColor(pixels, cv.COLOR_RGB2BGR)
 # Zoom, бикубическая интерполяция
# newimg1=cv.resize(pixels,(955//5,535//5),interpolation=cv.INTER_CUBIC)
# cv.imwrite('img.png', newimg1)
 # Zoom, интерполяция ближайшего соседа
# newimg2=cv.resize(pixels,(955//3,535//3),interpolation=cv.INTER_NEAREST)
# cv.imwrite('img.png', newimg2)
# get_all_colors(newimg2)
# bg = compose_bg_matrix(newimg2)
#  # Zoom, пересчет пикселей
# newimg3=cv2.resize(pixels,(955//5,535//5),interpolation=cv.INTER_AREA)
#  #Shrink, пересчет пикселей
newimg4=cv.resize(pixels,(955//5,535//5),interpolation=cv.INTER_LANCZOS4)
cv.imwrite('img.png', newimg4)
end = time.time()
print(end - start)


def get_char_pos(level) -> tuple:
    res = cv.matchTemplate(pixels, template, 2)
    minval, maxval, minloc, maxloc = cv.minMaxLoc(res)  
    return maxloc