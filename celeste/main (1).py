from typing import Tuple
import mss.tools
import cv2 as cv
import numpy as np
import keyboard
import random
import time
import csv

CHAR_DIMS = (24, 27)
CHAR_DIMS_CROUCH = (24, 12)
COMPRESS_COEFF = 3

cur_pos = (-1, -1)
new_pos = (-1, -1)

# environment globals
bounding_box = {'top': 40, 'left': 0, 'width': 955, 'height': 535}
template = cv.imread('char.png', cv.IMREAD_COLOR)
sct = mss.mss()
sct_img = sct.grab(bounding_box)  # screen capture
pixels = cv.cvtColor(np.array(sct_img), cv.COLOR_RGB2BGR)

action_space = ['a', 'w', 'd', 's', 'space', 'x', 'x+w', 'x+a', 'x+d', 'x+w+a', 'x+w+d', 'x+space', 'z', 'z+w']

# qlearning globals
eps = 1  # exploration rate
eps_decay_rate = 0.01  # delta between episodes
num_episodes = 1000
max_steps_per_episode = 200
learning_rate = 0.1  # alpha
discount_rate = 0.99  # gamma

# progress report
# route = []


def get_char_pos() -> Tuple[int, int]:
    """
    :return: maxloc // COMPRESS_COEFF
    """
    global sct_img, pixels, bounding_box, template
    sct_img = sct.grab(bounding_box)  # screen capture
    pixels = cv.cvtColor(np.array(sct_img), cv.COLOR_RGB2BGR)
    res = cv.matchTemplate(pixels, template, 2)
    coords = cv.minMaxLoc(res)[3]
    return coords[0] // COMPRESS_COEFF, coords[1] // COMPRESS_COEFF


def compose_bg_matrix(img) -> list:
    def set_border_rewards(matrix: list[list[int]]):
        REW_BORDER_WIDTH = 5  # since width of the collider is always greater than 5
        matrix = np.array(matrix)

        # Set top and bottom borders
        matrix[:REW_BORDER_WIDTH, :] = 2
        matrix[-REW_BORDER_WIDTH:, :] = 2

        # Set left and right borders
        matrix[:, :REW_BORDER_WIDTH] = 2
        matrix[:, -REW_BORDER_WIDTH:] = 2

        return matrix.tolist()

    bg1 = [
        [
            1 if img[y][x][0] == img[y][x][1] == img[y][x][2] else 0
            for x in range(bounding_box['width'] // COMPRESS_COEFF)
        ]
        for y in range(bounding_box['height'] // COMPRESS_COEFF)
    ]
    return set_border_rewards(bg1)


def save_qtable(qtable, file_path='qtable.csv'):
    with open(file_path, 'w') as f:
        csv.writer(f, delimiter=';').writerows(np.round(qtable, 3))


def read_qtable(file_path='qtable.csv') -> np.ndarray[float, np.dtype[float]] | None:
    try:
        with open(file_path, 'r') as f:
            return np.genfromtxt(f, delimiter=';')
    except FileNotFoundError:
        return None


def init_qtable(matrix) -> list[list[int]]:
    return [[0] * len(action_space)] * len(matrix) * len(matrix[0])


def move(key: str):
    keyboard.press(key)
    time.sleep(1)
    keyboard.release(key)


def act(qtable, cur_pos: Tuple[int, int]) -> int:
    """
    :return: index of the taken action
    """
    x = cur_pos[0]
    y = cur_pos[1]
    actions_for_cur_state = qtable[y * bounding_box['width'] // COMPRESS_COEFF + x]
    if random.random() > eps:
        action_ind = actions_for_cur_state.tolist().index(max(actions_for_cur_state))
    else:
        action_ind = random.randint(0, len(actions_for_cur_state) - 1)
    print(action_space[int(action_ind)], action_ind)
    move(action_space[int(action_ind)])
    return int(action_ind)


def update_qtable(qtable, action: int, state: int, reward: int, new_state: int):
    """
    !! qtable might be rotated

    :param new_state: updated character state
    :param reward: from bg matrix
    :param state: cur_pos (considering bg matrix is constant)
    :param qtable:
    :param action: action index in action space;
    """
    global learning_rate, discount_rate
    qtable[state][action] = qtable[state][action] * (1 - learning_rate) + \
                            learning_rate * (reward + discount_rate * np.max(qtable[new_state]))
    return qtable


def qlearning_loop(qtable, image_matrix):
    fast_reload = lambda: keyboard.send('R')
    global eps, cur_pos, new_pos, route, eps_decay_rate
    for episode in range(num_episodes):
        print(episode, " episode")
        route = []  # allows to track actions taken in the current run
        prev_coord_delta = [0] * 5  # remember 5 prev coordinate changes
        for _ in range(max_steps_per_episode):
            # take new action
            action_ind = act(qtable, cur_pos)
            new_pos = get_char_pos()
            if new_pos[0] >= len(image_matrix) or new_pos[1] >= len(image_matrix[0]):
                fast_reload()
                with open(f'{episode}.csv', 'w') as f:
                    csv.writer(f, delimiter=';').writerows(route)
                qtable[cur_pos[1] * bounding_box['width'] // COMPRESS_COEFF + cur_pos[0]][action_ind] -= 5
                save_qtable(qtable)
                time.sleep(3)
                route.clear()
                f.close()
                break
            # print(new_pos)
            # if image_matrix[new_pos[0]][new_pos[1]] == 2:  # check if done
            #     save_qtable(qtable)
            #     return
            qtable = update_qtable(
                qtable=qtable,
                action=action_ind,
                state=cur_pos[1] * bounding_box['width'] // COMPRESS_COEFF + cur_pos[0],
                new_state=new_pos[1] * bounding_box['width'] // COMPRESS_COEFF + new_pos[0],
                reward=image_matrix[new_pos[0]][new_pos[1]]
            )
            route.append([cur_pos, action_ind])
            # print(route)
            # if the character does not make significant progress on the route
            prev_coord_delta.append(abs(cur_pos[0] - new_pos[0]) + abs(cur_pos[1] - new_pos[1]))
            prev_coord_delta.pop(0)
            if sum(prev_coord_delta) < 30:
                print('too small coord delta -- restart')
                fast_reload()
                # deduct q-table values, so the agent doesn't repeat unsuccessful route
                deduction_delta = (5 - 0.1) / len(route)
                deduction_coef = 0.1
                for (key, value) in route:
                    qtable[key[1] * bounding_box['width'] // COMPRESS_COEFF + key[0]][value] -= deduction_coef
                    deduction_coef += deduction_delta
                fast_reload()
                continue

            cur_pos = new_pos

        fast_reload()
        if episode % 5 == 0:
            # TODO save qtable in separate execution thread (import threading)
            save_qtable(qtable)
        eps = 0.01 + 0.99 * np.exp(-eps_decay_rate * episode)


def main():
    global cur_pos, pixels, COMPRESS_COEFF
    time.sleep(5)

    newimg2 = cv.resize(pixels,
                        (bounding_box['width'] // COMPRESS_COEFF, bounding_box['height'] // COMPRESS_COEFF),
                        interpolation=cv.INTER_NEAREST)
    image_matrix = compose_bg_matrix(newimg2)
    qtable = read_qtable()
    if qtable is None:
        qtable = init_qtable(matrix=image_matrix)
    cur_pos = get_char_pos()
    qlearning_loop(qtable, image_matrix)


if __name__ == "__main__":
    main()