import matplotlib.pyplot as plt
from typing import Tuple, Union
import numpy as np
import random
import copy
from utils import gradient_descend

class Regression:
    def __new__(cls, *args, **kwargs):
        raise RuntimeError("Regression class is static class")

    @staticmethod
    def rand_in_range(rand_range: Union[float, Tuple[float, float]] = 1.0) -> float:
        if isinstance(rand_range, float):
            return random.uniform(-0.5 * rand_range, 0.5 * rand_range)
        if isinstance(rand_range, tuple):
            return random.uniform(rand_range[0], rand_range[1])
        return random.uniform(-0.5, 0.5)

    @staticmethod
    def test_data_along_line(k: float = 1.0, b: float = 0.1, arg_range: float = 1.0,
                             rand_range: float = 0.05, n_points: int = 100) -> Tuple[np.ndarray, np.ndarray]:
        """
        Генерирует линию вида y = k * x + b + dy, где dy - аддитивный шум с амплитудой half_disp
        :param k: наклон линии
        :param b: смещение по y
        :param arg_range: диапазон аргумента от 0 до arg_range
        :param rand_range: диапазон шума данных
        :param n_points: количество точек
        :return: кортеж значений по x и y
        """
        x_step = arg_range / (n_points - 1)
        return np.array([i * x_step for i in range(n_points)]),\
               np.array([i * x_step * k + b + Regression.rand_in_range(rand_range) for i in range(n_points)])

    @staticmethod
    def test_data_2d(kx: float = -2.0, ky: float = 2.0, b: float = 12.0, args_range: float = 1.0,
                     rand_range: float = 1.0, n_points: int = 100) -> Tuple[np.ndarray, np.ndarray, np.ndarray]:
        """
        Генерирует плоскость вида z = kx*x + ky*x + b + dz, где dz - аддитивный шум в диапазоне rand_range
        :param kx: наклон плоскости по x
        :param ky: наклон плоскости по y
        :param b: смещение по z
        :param args_range: диапазон аргументов по кажой из осей от 0 до args_range
        :param rand_range: диапазон шума данных
        :param n_points: количество точек
        :returns: кортеж значенией по x, y и z
        """
        x = np.array([Regression.rand_in_range(args_range) for _ in range(n_points)])
        y = np.array([Regression.rand_in_range(args_range) for _ in range(n_points)])
        dz = np.array([b + Regression.rand_in_range(rand_range) for _ in range(n_points)])
        return x, y, x * kx + y * ky + dz 
    
    @staticmethod
    def second_order_surface_2d(surf_params:
                                Tuple[float, float, float, float, float, float] = (1.0, -2.0, 3.0, 1.0, 2.0, -3.0),
                                args_range: float = 1.0, rand_range: float = .1, n_points: int = 1000) -> \
                                Tuple[np.ndarray, np.ndarray, np.ndarray]:
        """
        Генерирует набор тестовых данных около поверхности второго порядка.
        Уравнение поверхности:
        z(x,y) = a * x^2 + x * y * b + c * y^2 + d * x + e * y + f
        :param surf_params: 
        :param surf_params [a, b, c, d, e, f]:
        :param args_range x in [x0, x1], y in [y0, y1]:
        :param rand_range:
        :param n_points:
        :return:
        """
        x = np.array([Regression.rand_in_range(args_range) for _ in range(n_points)])
        y = np.array([Regression.rand_in_range(args_range) for _ in range(n_points)])
        dz = np.array([surf_params[5] + Regression.rand_in_range(rand_range) for _ in range(n_points)])
        return x, y, surf_params[0] * x * x + surf_params[1] * y * x + surf_params[2] * y * y + \
               surf_params[3] * x + surf_params[4] * y + dz
    @staticmethod
    def test_data_nd(surf_settings: np.ndarray = np.array([1.0, 2.0, 3.0, 4.0, 5.0, 12.0]), args_range: float = 1.0,
                     rand_range: float = 0.1, n_points: int = 125) -> np.ndarray:
        """
        Генерирует плоскость вида z = k_0*x_0 + k_1*x_1...,k_n*x_n + d + dz, где dz - аддитивный шум в диапазоне rand_range
        :param surf_settings: параметры плоскости в виде k_0,k_1,...,k_n,d
        :param args_range: диапазон аргументов по кажой из осей от 0 до args_range
        :param n_points: количество точек
        :param rand_range: диапазон шума данных
        :returns: массив из строк вида x_0, x_1,...,x_n, f(x_0, x_1,...,x_n)
        """
        n_dims = surf_settings.size - 1
        data = np.zeros((n_points, n_dims + 1,), dtype=float)
        for i in range(n_dims):
            data[:, i] = np.array([Regression.rand_in_range(args_range) for _ in range(n_points)])
            data[:, n_dims] += surf_settings[i] * data[:, i]
        dz = np.array([surf_settings[n_dims] + Regression.rand_in_range(rand_range) for _ in range(n_points)])
        data[:, n_dims] += dz
        return data

    @staticmethod
    def distance_sum(x: np.ndarray, y: np.ndarray, k: float, b: float) -> float:
        """
        Вычисляет сумму квадратов расстояний от набора точек до линии вида y = k*x + b при фиксированных k и b
        по формуле: F(k, b) = (Σ(yi -(k * xi + b))^2)^0.5 (суммирование по i)
        :param x: массив значений по x
        :param y: массив значений по y
        :param k: значение параметра k (наклон)
        :param b: значение параметра b (смещение)
        :returns: F(k, b) = (Σ(yi -(k * xi + b))^2)^0.5
        """
        return np.sqrt(np.power((y - x * k + b), 2.0).sum())

    @staticmethod
    def distance_field(x: np.ndarray, y: np.ndarray, k: np.ndarray, b: np.ndarray) -> np.ndarray:
        """
        Вычисляет сумму квадратов расстояний от набора точек до линии вида y = k*x + b, где k и b являются диапазонами
        значений. Формула расстояния для j-ого значения из набора k и k-ого значения из набора b:
        F(k_j, b_k) = (Σ(yi -(k_j * xi + b_k))^2)^0.5 (суммирование по i)
        :param x: массив значений по x
        :param y: массив значений по y
        :param k: массив значений параметра k (наклоны)
        :param b: массив значений параметра b (смещения)
        :returns: поле расстояний вида F(k, b) = (Σ(yi -(k * xi + b))^2)^0.5 (суммирование по i)
        """
        return np.array([[Regression.distance_sum(x, y, k_i, b_i) for k_i in k.flat] for b_i in b.flat])

    @staticmethod
    def linear_regression(x: np.ndarray, y: np.ndarray) -> Tuple[float, float]:
        """
        Линейная регрессия.
        :param x: массив значений по x
        :param y: массив значений по y
        :returns: возвращает пару (k, b), которая является решением задачи (Σ(yi -(k * xi + b))^2)->min
        k = (Σxi*yi - Σxi*Σyi / n) / (Σxi^2 - (Σxi)^2 / n)\n
        b = (Σyi - k * Σxi) /n\n
        """
        one_div_by_n = 1.0 / x.size
        xx_sum = (x * x).sum()
        xy_sum = (x * y).sum()
        x_sum = x.sum()
        y_sum = y.sum()
        k = (xy_sum - x_sum * y_sum * one_div_by_n) / (xx_sum - x_sum * x_sum * one_div_by_n)
        b = (y_sum - k * x_sum) * one_div_by_n
        return k, b

    @staticmethod
    def bi_linear_regression(x: np.ndarray, y: np.ndarray, z: np.ndarray) -> Tuple[float, float, float]:
        """
        Билинейная регрессия.\n

        ====================================================================================================================\n
        Hesse matrix:\n
                       | Σ xi^2;  Σ xi*yi; Σ xi |\n
        H(kx, ky, b) = | Σ xi*yi; Σ yi^2;  Σ yi |\n
                       | Σ xi;    Σ yi;    n    |\n
        ====================================================================================================================\n
                          | Σ-zi*xi + ky*xi*yi + kx*xi^2 + xi*b |\n
        grad(kx, ky, b) = | Σ-zi*yi + ky*yi^2 + kx*xi*yi + b*yi |\n
                          | Σ-zi + yi*ky + xi*kx                |\n
        ====================================================================================================================\n
        Окончательно решение:\n
        |kx|   |1|\n
        |ky| = |1| -  H(1, 1, 0)^-1 * grad(1, 1, 0)\n
        | b|   |0|\n

        :param x: массив значений по x
        :param y: массив значений по y
        :param z: массив значений по z
        :returns: возвращает тройку (kx, ky, b), которая является решением задачи (Σ(zi - (yi * ky + xi * kx + b))^2)->min
        """
        n = x.size
        x_sum = x.sum()
        xx_sum = (x * x).sum()
        y_sum = y.sum()
        yy_sum = (y * y).sum()
        xy_sum = (x * y).sum()
        z_sum = z.sum()
        xz_sum = (z * x).sum()
        yz_sum = (y * z).sum()
        vec = np.array([[1.], [1.], [0.]])
        H = np.array([[xx_sum, xy_sum, x_sum], [xy_sum, yy_sum, y_sum], [x_sum, y_sum, n]])
        grad = np.array([[-xz_sum + xy_sum + xx_sum], [-yz_sum + yy_sum + xy_sum], [-z_sum + y_sum + x_sum]])
        res = vec - np.dot(np.linalg.inv(H), grad)
        return res

    @staticmethod
    def n_linear_regression(data_rows: np.ndarray) -> np.ndarray:
        """
        H_ij = Σx_i * x_j, i in [0, rows - 1] , j in [0, rows - 1]
        H_ij = Σx_i, j = rows i in [rows, :]
        H_ij = Σx_j, j in [:, rows], i = rows

               | Σkx * xi^2    + Σky * xi * yi + b * Σxi - Σzi * xi|\n
        grad = | Σkx * xi * yi + Σky * yi^2    + b * Σyi - Σzi * yi|\n
               | Σyi * ky      + Σxi * kx                - Σzi     |\n

        x_0 = [1,...1, 0] =>

               | Σ xi^2    + Σ xi * yi - Σzi * xi|\n
        grad = | Σ xi * yi + Σ yi^2    - Σzi * yi|\n
               | Σxi       + Σ yi      - Σzi     |\n

        :param data_rows:  состоит из строк вида: [x_0,x_1,...,x_n, f(x_0,x_1,...,x_n)]
        :return:
        """
        s_cols = data_rows.shape[1]
        H = np.zeros((s_cols, s_cols), dtype=float)
        grad = np.zeros((s_cols), dtype=float)
        x_0 = np.ones((s_cols), dtype=float)
        x_0[len(x_0)-1] = 0
        for row in range(s_cols - 1):
            for col in range(row + 1):
                value = np.sum(np.dot(data_rows[:, row], data_rows[:, col]))
                H[row, col] = value
                H[col, row] = value
        for i in range(s_cols):
            value = np.sum(data_rows[:, i])
            H[i, s_cols - 1] = value
            H[s_cols - 1, i] = value
        H[s_cols - 1, s_cols - 1] = data_rows.shape[0]
        for row in range(s_cols - 1):
            grad[row] = np.sum(H[row, 0: s_cols - 1]) - np.dot(data_rows[:, s_cols - 1], data_rows[:, row])
        grad[s_cols - 1] = np.sum(H[s_cols - 1, 0 : s_cols - 1]) - np.sum(data_rows[:, s_cols - 1])
        return x_0 - np.dot(np.linalg.inv(H), grad)

    @staticmethod
    def poly_regression(x: np.ndarray, y: np.ndarray, order: int = 5) -> np.ndarray:
        """
        Полином: y = Σ_j x^j * bj\n
        Отклонение: ei =  yi - Σ_j xi^j * bj\n
        Минимизируем: Σ_i(yi - Σ_j xi^j * bj)^2 -> min\n
        Σ_i(yi - Σ_j xi^j * bj)^2 = Σ_iyi^2 - 2 * yi * Σ_j xi^j * bj +(Σ_j xi^j * bj)^2\n
        условие минимума:\n d/dbj Σ_i ei = d/dbj (Σ_i yi^2 - 2 * yi * Σ_j xi^j * bj +(Σ_j xi^j * bj)^2) = 0\n
        :param x: массив значений по x
        :param y: массив значений по y
        :param order: порядок полинома
        :return: набор коэффициентов bi полинома y = Σx^i*bi
        """
        a_m = np.zeros((order, order,), dtype=float)
        c_m = np.zeros((order,), dtype=float)
        n = x.size
        x1 = 1
        for row in range(order):
            c_m[row] = np.sum(y * (x1)) / n
            x2 = x1
            for col in range(row + 1):
                a_m[row][col] = np.sum(x2) / n
                a_m[col][row] = a_m[row][col]
                x2 = x2 * x
            x1= x1 * x
        return np.linalg.inv(a_m) @ c_m

    @staticmethod
    def polynom(x: np.ndarray, b: np.ndarray) -> np.ndarray:
        """
        :param x: массив значений по x\n
        :param b: массив коэффициентов полинома\n
        :returns: возвращает полином yi = Σxi^j*bj\n
        """
        y = []
        for i in x:
            p = 0
            for j in range(len(b)):
                p += b[j] * pow(i, j)
            y.append(p)
        return np.array(y)

    @staticmethod
    def quadratic_regression_2d(x: np.ndarray, y: np.ndarray, z: np.ndarray) -> np.ndarray:
        params = [x*x, x*y, y*y,  x, y, np.array((1.0, ))]
        # a_ij = (params_i, params_j)
        # b_i = (params_i, z)
        # A^-1*b
        a = np.zeros((len(params), len(params)), dtype=float)
        b = np.ones(len(params), dtype=float)
        for i in range(len(params)):
            b[i] = (params[i] * z).sum()
            for j in range(i + 1):
                a[i][j] = a[j][i] = (params[i] * params[j]).sum()
        a[-1, -1] = x.size
        return np.linalg.inv(a) @ b # np.dot(np.linalg.inv(a) @ b)

    @staticmethod
    def distance_field_example():
        """
        Функция проверки поля расстояний:\n
        1) Посчитать тестовыe x и y используя функцию test_data\n
        2) Задать интересующие нас диапазоны k и b (np.linspace...)\n
        3) Рассчитать поле расстояний (distance_field) и вывести в виде изображения.\n
        4) Проанализировать результат (смысл этой картинки в чём...)\n
        :return: 
        """
        print("distance field test:")
        x, y = Regression.test_data_along_line()
        k_, b_ = Regression.linear_regression(x, y)
        print(f"y(x) = {k_:1.5} * x + {b_:1.5}\n")
        k = np.linspace(-2.0, 2.0, 128, dtype=float)
        b = np.linspace(-2.0, 2.0, 128, dtype=float)
        z = Regression.distance_field(x, y, k, b)
        plt.imshow(z, extent=[k.min(), k.max(), b.min(), b.max()])
        plt.plot(k_, b_, 'r*')
        plt.xlabel("k")
        plt.ylabel("b")
        plt.grid(True)
        plt.show()

    @staticmethod
    def linear_reg_example():
        """
        Функция проверки работы метода линейной регрессии:\n
        1) Посчитать тестовыe x и y используя функцию test_data\n
        2) Получить с помошью linear_regression значения k и b\n
        3) Вывести на графике x и y в виде массива точек и построить\n
           регрессионную прямую вида: y = k*x + b\n
        :return:
        """
        print("linear reg test:")
        x, y = Regression.test_data_along_line()
        k, b = Regression.linear_regression(x, y)
        print(f"y(x) = {k:1.5} * x + {b:1.5}\n")
        x_ = np.array([0, 1])
        y_ = k*x_ + b
        plt.plot(x_, y_, 'r')
        plt.scatter(x, y)
        plt.grid(True)
        plt.show()

    @staticmethod
    def bi_linear_reg_example():
        """
        Функция проверки работы метода билинейной регрессии:\n
        1) Посчитать тестовыe x, y и z используя функцию test_data_2d\n
        2) Получить с помошью bi_linear_regression значения kx, ky и b\n
        3) Вывести на трёхмерном графике x, y и z в виде массива точек и построить\n
           регрессионную плоскость вида:\n z = kx*x + ky*y + b\n
        :return:
        """
        print('\nbi_linear regression test:')
        from matplotlib import cm
        x, y, z = Regression.test_data_2d()
        coefs = Regression.bi_linear_regression(x, y, z)
        print(coefs)
        # print(f"z(x, y) = {coefs[0]:1.5} * x + {coefs[1]:1.5} * y + {coefs[2]:1.5}")
        x_, y_ = np.meshgrid(np.linspace(np.min(x), np.max(x), 100), np.linspace(np.min(y), np.max(y), 100))
        z_ = coefs[0] * x_ + y_ * coefs[1] + coefs[2]
        fig, ax = plt.subplots(subplot_kw={"projection": "3d"})
        ax.plot(x, y, z, 'r.')
        surf = ax.plot_surface(x_, y_, z_, cmap=cm.coolwarm, linewidth=0, antialiased=False, edgecolor='none', alpha=0.5)
        plt.xlabel("x")
        plt.ylabel("y")
        fig.colorbar(surf, shrink=0.5, aspect=5)
        plt.show()

    @staticmethod
    def poly_reg_example():
        """
        Функция проверки работы метода полиномиальной регрессии:\n
        1) Посчитать тестовыe x, y используя функцию test_data\n
        2) Посчитать набор коэффициентов bi полинома y = Σx^i*bi используя функцию poly_regression\n
        3) Вывести на графике x и y в виде массива точек и построить\n
           регрессионную кривую. Для построения кривой использовать метод polynom\n
        :return:
        """
        print('\npoly regression test:')
        x, y = Regression.test_data_along_line()
        coefficients = Regression.poly_regression(x, y)
        print(coefficients)
        x_ = np.arange(0, 1, 0.01)
        y_ = Regression.polynom(x_, coefficients)
        plt.plot(x_, y_, 'r')
        plt.scatter(x, y)
        plt.grid(True)
        plt.show()

    @staticmethod
    def n_linear_reg_example():
        print("\nn linear regression test:")
        data = Regression.test_data_nd()
        coeffs = Regression.n_linear_regression(data)
        print(coeffs)


    @staticmethod
    def quadratic_reg_example():
        """
        """
        print('2d quadratic regression test:')
        from matplotlib import cm
        x, y, z = Regression.second_order_surface_2d()
        coeffs = Regression.quadratic_regression_2d(x, y, z)
        print(
            f"z(x, y) = {coeffs[0]:1.3} * x^2 + {coeffs[1]:1.3} * x * y + {coeffs[2]:1.3} * y^2 + {coeffs[3]:1.3} * x + {coeffs[4]:1.3} * y + {coeffs[5]:1.3}")
        x_, y_ = np.meshgrid(np.linspace(np.min(x), np.max(x), 100), np.linspace(np.min(y), np.max(y), 100))
        z_ = coeffs[0]*x_**2 + coeffs[1]*x_*y_ + coeffs[2]*y_**2 + coeffs[3]*x_ + coeffs[4]*y_ + coeffs[5]
        fig, ax = plt.subplots(subplot_kw={"projection": "3d"})
        ax.plot(x, y, z, 'r.')
        surf = ax.plot_surface(x_, y_, z_, cmap=cm.coolwarm, linewidth=0, antialiased=False, edgecolor='none', alpha=0.5)
        plt.xlabel("x")
        plt.ylabel("y")
        fig.colorbar(surf, shrink=0.5, aspect=5)
        plt.show()


if __name__ == "__main__":
    # Regression.distance_field_example()
    # Regression.linear_reg_example()
    # Regression.bi_linear_reg_example()
    # Regression.n_linear_reg_example()
    # Regression.poly_reg_example()
    Regression.quadratic_reg_example()

