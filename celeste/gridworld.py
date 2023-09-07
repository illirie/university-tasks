# from mss import mss
# # import numpy as np
# # import cv2

# bounding_box = {'top': 50, 'left': 0, 'width': 600, 'height': 450}

# HTBX_COLOR = (0, 255, 0)
# BG_COLOR = (0, 0, 0)

# cur_pos = new_pos = (-1, -1)


# def get_hitbox_upper_corner(img) -> tuple:
#     for x in range(bounding_box['width']):
#         for y in range(bounding_box['height']):
#             if img.pixel(x, y) == HTBX_COLOR:
#                 return (x, y)
#     return (-1, -1)


# sct = mss()
# while True:
#     # cv2.imshow('screen', np.array(sct_img))
#     new_pos = get_hitbox_upper_corner(sct.grab(bounding_box))
#     if cur_pos != new_pos:
#         print(new_pos)
#         cur_pos = new_pos

# from mss import mss
import mss.tools
from PIL import Image#, ImageDraw
# import threading
import numpy as np
import cv2 as cv
import time

bounding_box = {'top': 40, 'left': 0, 'width': 955, 'height': 535}

HTBX_COLOR = (0, 255, 0)
DNG_ZONE_COLOR = (178, 0, 0)
BG_HTBX_COLOR = (255, 135, 85)
BG_COLOR = (0, 0, 0)

CHAR_DIMS = (24, 27)
CHAR_DIMS_CROUCH = (24, 12)

cur_pos = new_pos = (-1, -1)


def get_hitbox_upper_corner(img) -> tuple:
    start_x = 0 if cur_pos[0] == -1 or cur_pos[0] < 100 else cur_pos[0] - 100
    end_x = bounding_box['width'] if cur_pos[0] == -1 or cur_pos[0] > 855 else cur_pos[0] + 100
    start_y = 0 if cur_pos[1] == -1 or cur_pos[1] < 100 else cur_pos[1] - 100
    end_y = bounding_box['height'] if cur_pos[1] == -1 or cur_pos[1] > 435 else cur_pos[1] + 100
    for x in range(start_x, end_x):
        for y in range(start_y, end_y):
            if img.pixel(x, y) == HTBX_COLOR:
                return (x, y)
    return (-1, -1)

def find_size(img):
    if cur_pos != (-1, -1):
        for x in range(cur_pos[0], cur_pos[0] + 50):
            if img.pixel(x, cur_pos[1]) != HTBX_COLOR:
                print(x - cur_pos[0])
                break
        for y in range(cur_pos[1], cur_pos[1] + 50):
            if img.pixel(cur_pos[0], y) != HTBX_COLOR:
                print(y - cur_pos[1])
                break


# def prep_matrix_for_printing(bg):

def get_all_colors(img):
    print(img.pixel)


# sct = mss()
# print(compose_bg_matrix(sct.grab(bounding_box)))
# image = Image.fromarray(image_matrix, 'RGB')
# image .save('image-grad.jpg')

# level = cv.imread('sct-40x0_955x535.png', cv.IMREAD_COLOR)
# template = cv.imread('char.png', cv.IMREAD_COLOR)
# res = cv.matchTemplate(level, template, 0)
# # res = cv.matchTemplate(np.array(sct.grab(bounding_box).pixel), template, 0)
# minval, maxval, minloc, maxloc = cv.minMaxLoc(res)
# print(minloc)
template = cv.imread('char.png', cv.IMREAD_COLOR)
while True:
    start = time.time()
    with mss.mss() as sct:
        output = "sct-{top}x{left}_{width}x{height}.png".format(**bounding_box)
        sct_img = sct.grab(bounding_box)
        mss.tools.to_png(sct_img.rgb, sct_img.size, output=output)
    level = cv.imread('sct-40x0_955x535.png', cv.IMREAD_COLOR)
    res = cv.matchTemplate(level, template, 0)
# res = cv.matchTemplate(np.array(sct.grab(bounding_box).pixel), template, 0)
    minval, maxval, minloc, maxloc = cv.minMaxLoc(res)
    # cv2.imshow('screen', np.array(sct_img))

    new_pos = minloc
    # new_pos = get_hitbox_upper_corner(sct.grab(bounding_box))
    end = time.time()
    # find_size(sct.grab(bounding_box))
    if cur_pos != new_pos:
        print(new_pos)
        cur_pos = new_pos
        print(end - start)