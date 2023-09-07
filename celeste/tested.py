import mss.tools
from PIL import Image
import numpy as np
import time 

HTBX_COLOR = (0, 255, 0)
DNG_ZONE_COLOR = (255, 0, 0)
BG_HTBX_COLOR = (255, 135, 85)
BG_COLOR = (0, 0, 0)
BG_COLOR_2 = (43, 43, 43)

#955x535
bounding_box = {'top': 40, 'left': 0, 'width': 955, 'height': 535}

def get_all_colors(img):
    res = set()
    for i in img.pixels:
        for j in i:
            res.add(j)
    print(res)


def compose_bg_matrix(img):
    #bg = [[0] * bounding_box['width']] * bounding_box['height']
    bg1 = []
    get_all_colors(img)
    for y in range(bounding_box['height']):
        col_bg = []
        for x in range(bounding_box['width']):
            if img.pixel(x, y) == BG_HTBX_COLOR:
                # print(img.pixel(x, y), ' ', x, ' ', y)
                col_bg.append(100)
                #bg[y][x] = 1
            elif img.pixel(x, y) == DNG_ZONE_COLOR:
                # print(img.pixel(x, y), ' ', x, ' ', y)
                col_bg.append(200)
                #bg[y][x] = 2
            else:
                # print(img.pixel(x, y), ' ', x, ' ', y)
                col_bg.append(0)
                # bg[y][x] = 0
        bg1.append(col_bg)
    # for i in bg:
    #     print(i)
    # print('  ')
    # for i in bg1:
    #     print(i)
    return bg1
start = time.time()
with mss.mss() as sct:
    # The screen part to capture
    output = "sct-{top}x{left}_{width}x{height}.png".format(**bounding_box)

    # Grab the data
    sct_img = sct.grab(bounding_box)
    get_all_colors(sct_img)
    image_matrix = np.array(compose_bg_matrix(sct_img))
    end = time.time()
    print(end - start)
    image = Image.fromarray(image_matrix, 'L')
    image.save('bg.png')
    

    # Save to the picture file
    mss.tools.to_png(sct_img.rgb, sct_img.size, output=output)
    print(output)