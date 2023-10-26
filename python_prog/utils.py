import numpy as np


def partial(func, args: np.ndarray, arg_id: int, accuracy=1e-6) -> float:
    args[arg_id] += accuracy
    f_r = func(args)
    args[arg_id] -= 2.0 * accuracy
    f_l = func(args)
    args[arg_id] += accuracy
    return (f_r - f_l) / accuracy * 0.5


def magnitude(x: np.ndarray) -> float:
    return np.sqrt((x * x).sum())


def direction(x: np.ndarray) -> np.ndarray:
    return x / magnitude(x)


def gradient(func, args: np.ndarray, accuracy: float = 1e-6) -> np.ndarray:
    df = np.zeros_like(args)
    for i in range(args.size):
        df[i] = partial(func, args, i, accuracy)
    return df

def bisect_nd(f, x_0: np.ndarray, x_1: np.ndarray,
              accuracy: float = 0.001, max_iters: int = 100):
    if max_iters < 10:
        max_iters = 10
    x_0_: np.ndarray = x_0.copy()
    x_1_: np.ndarray = x_1.copy()
    x_c_: np.ndarray = (x_0_ + x_1_) * 0.5
    dir_: np.ndarray = direction(x_1_ - x_0_) * accuracy
    for i in range(max_iters):
        if magnitude(x_1_ - x_0_) < accuracy:
            break
        x_c_ = (x_1_ + x_0_) * 0.5
        if f(x_c_ + dir_) > f(x_c_ - dir_):
            x_1_ = x_c_
            continue
        x_0_ = x_c_
    return x_c_

def gradient_descend(func, start_x: np.ndarray,
                     rate: float = 1.0, accuracy: float = 0.001, max_iters: int = 100) -> np.ndarray:
    x_i = start_x.copy()
    x_i_1: np.ndarray
    grad: np.ndarray
    cntr: int = 0
    while (cntr < max_iters):
        cntr += 1
        grad = rate * gradient(func, x_i, accuracy)
        x_i_1 = x_i - grad
        x_i_1 = bisect_nd(func, x_i, x_i_1, accuracy, max_iters)
        if magnitude(x_i_1 - x_i) < accuracy:
            x_i = x_i_1
            break
        x_i = x_i_1

    return (x_i_1 + x_i) * 0.5