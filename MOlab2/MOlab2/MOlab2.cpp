#include <iostream>
#include "common.h"
#include "vector_utils.h"
#include "matrix_utils.h"
#include <vector>

double penalty_coef = 2;
double delta_pen = 0.1;

double f(vec_n x) {
    return (x[0] - 5) * x[0] + (x[1] - 3) * x[1];
}

double internal_penalty(vec_n x, vec_n b, mat_mn n) {
    vec_n temp = n * x - b;
    double res = 0;
    for (int i = 0; i < temp.size(); i++) {
        res += pow(temp[i], -2.0);
    }
    return res;
}

double internal_penalty2(vec_n x) {
    return f(x) + penalty_coef * pow(f(x), 3);
}

double external_penalty2(vec_n x) {
    if (f(x) > 0) {
        return f(x) + penalty_coef * pow(f(x), 2);
    }
    return f(x);
}

vec_n heaviside(vec_n x) {
    vec_n res(x);
    for (int i = 0; i < x.size(); i++) {
        if (res[i] < 0) {
            res[i] = 0;
        }
        else {
            res[i] = 1;
        }
    }
    return res;
}

vec_n vec_mul(vec_n a, vec_n b) {
    vec_n coords(a);
    for (int i = 0; i < 3; i++)
        coords[i] = a[i + 1] * b[i + 2] - a[i + 2] * b[i + 1];
    return coords;
}

double external_penalty(vec_n x, vec_n b, mat_mn n) {
    vec_n temp = n * x - b;
    vec_n temp2(temp);
    double res = 0;
    temp2 = temp2 * 32;
    for (int i = 0; i < temp.size(); i++) {
        temp2[i] *= temp2[i];
    }
    temp = temp2 - temp;
    //temp = vec_mul(temp, temp2);
    for (int i = 0; i < temp.size(); i++) {
        res += pow(temp[i], -2.0);
    }
    return res;
}

vec_n bisect(func_nd f, vec_n x0, vec_n x1) {
    double eps = ACCURACY;
    vec_n dir = direction(x0, x1) * eps;
    vec_n xc;
    for (int i = 0; i < ITERS_MAX; i++) {
        if (magnitude(x1 - x0) < eps) {
            break;
        }
        xc = (x1 + x0) * 0.5;
        if (f(xc + dir) > f(xc - dir)) {
            x1 = xc;
        }
        else {
            x0 = xc;
        }
    }
    return xc;
}

vec_n coordDescend(func_nd f, vec_n x) {
    double eps = ACCURACY;
    double xi, y0, y1;
    double step = 0.5;
    vec_n x0 = x, x1 = x;
    int c_id = 0, c_n = 0, n = x.size();
    for (int i = 0; i < ITERS_MAX; i++) {
        c_id = i % n;
        x1[c_id] -= eps;
        y0 = f(x1);
        x1[c_id] += 2 * eps;
        y1 = f(x1);
        x1[c_id] -= eps;
        if (y0 < y1) {
            x1[c_id] -= step;
        }
        else {
            x1[c_id] += step;
        }
        xi = x0[c_id];
        x1 = bisect(f, x0, x1);
        x0 = x1;
        if (abs(x1[c_id] - xi) < eps)
        {
            c_n++;
            if (c_n == x1.size())
            {
                return x0;
            }
        }
        else {
            c_n = 0;
        }
    }
    return x0;
}

vec_n gradientDescend(func_nd f, vec_n x)
{
    vec_n  xi_1, grad;
    double eps = ACCURACY;
    for (int i = 0; i <= ITERS_MAX; i++)
    {
        grad = gradient(f, x, eps);
        xi_1 = x - grad;
        xi_1 = bisect(f, x, xi_1);
        if (magnitude(xi_1 - x) < eps) {
            break;
        }
        else {
            x = xi_1;
        }
    }
    return (xi_1 + x) * 0.5;
}

vec_n conjugateGradDescend(func_nd f, vec_n x) {
    double eps = ACCURACY;
    vec_n  xi_1, si = -1 * gradient(f, x, eps), si_1;
    double w = 0;
    for (int i = 0; i < ITERS_MAX; i++) {
        xi_1 = x + si;
        if (magnitude(xi_1 - x) < eps) {
            break;
        }
        xi_1 = bisect(f, x, xi_1);
        si_1 = gradient(f, xi_1, eps);
        w = pow(magnitude(si_1), 2) / pow(magnitude(si), 2);
        si = si * w - si_1;
        x = xi_1;
    }
    return (xi_1 + x) * 0.5;
}

double penalty_extr(const mat_mn& bounds, const vec_n& b, const vec_n& x)
{
    double res = 0.0;
    for (const auto& value : bounds * x - b) res += value <= 0.0 ? 0.0 : pow(value, 2);
    return res;
}

vec_n newtoneRaphson(func_nd f, vec_n x)
{
    vec_n x1, grad;
    mat_mn hess;
    double eps = ACCURACY;
    for (int i = 0; i < ITERS_MAX; i++)
    {
        grad = gradient(f, x, eps);
        hess = invert(hessian(f, x, eps));
        x1 = x - hess * grad;
        if (magnitude(x1 - x) < eps) {
            break;
        }
        else {
            x = x1;
        }
    }
    return (x1 + x) * 0.5;
}

vec_n newtoneRaphson_with_penalty(func_nd f, vec_n x)
{
    vec_n x1, grad;
    mat_mn hess;
    double eps = ACCURACY;
    for (int i = 0; i < ITERS_MAX; i++)
    {
        grad = gradient(f, x, eps);
        hess = invert(hessian(f, x, eps));
        x1 = x - hess * grad;
        if (magnitude(x1 - x) < eps) {
            break;
        }
        else {
            x = x1;
            penalty_coef += delta_pen;
        }
    }
    return (x1 + x) * 0.5;
}

double penalty1(vec_n x) {
    vec_n b = { 0, 0, 0 };
    mat_mn n = { {7.0, -2.0}, { 1.0, 1.0}, {-7.0, 7.0} };
    return pow(x[0] - 4, 2) + pow(x[1] - 4, 2) + internal_penalty(x, b, n);
}

double penalty2(vec_n x) {
    vec_n b = { 0, 0, 0 };
    mat_mn n = { {7.0, -2.0}, { 1.0, 1.0}, {-7.0, 7.0} };
    return pow(x[0] - 4, 2) + pow(x[1] - 4, 2) + external_penalty(x, b, n);
}

class simplex {
private:
    mat_mn table; //симплекс таблица
    vec_n basis; //базисные переменные
    int m = 0, n = 0; //размеры таблицы
    bool isMin;
public:
    simplex(mat_mn& source, const  bool& cond, const vec_n& ineq) {
        m = source.size();
        n = source[0].size();
        table = zeros(m, m + n - 1);
        isMin = cond;
        if (isMin) {
            for (int i = 0; i < n; i++) {
                source[m - 1][i] *= -1;
            }
        }
        for (int i = 0; i < m; i++)
        {
            for (int j = 0; j < table[0].size(); j++)
            {
                if (j < n) table[i][j] = source[i][j];
            }
            if ((n + i) < table[0].size())
            {
                table[i][n + i] = ineq[i] * -1;
                basis.push_back(n + i);
            }
        }
        n = table[0].size();
        std::cout << '\n';
        this->printBasis();
        this->printTable();
        std::cout << '\n';
    };
    bool endingCond() {
        bool flag = true;
        for (int j = 1; j < n; j++)
        {
                if (table[m - 1][j] < 0)
                {
                    flag = false;
                    break;
                }
        }
        return flag;
    }
    int findMainCol() {
        int mainCol = 1, flag = -1;
        for (int j = 2; j < n; j++) {
            if (table[m - 1][j] < table[m - 1][mainCol]) {
                mainCol = j;
                flag = 1;
            }
        }
        if (flag == 1 || table[m-1][mainCol] < 0) {
            return mainCol;
        }
        return flag;
    }
    int findMainRow(int mainCol) {
        int mainRow = 0;
        for (int i = 0; i < m - 1; i++) {
            if (table[i][mainCol] > 0) {
                mainRow = i;
                //std::cout << mainRow << '\n';
                break;
            }
        }
        for (int i = mainRow + 1; i < m - 1; i++) {
            if ((table[i][mainCol] > 0) && ((table[i][0] / table[i][mainCol]) < (table[mainRow][0] / table[mainRow][mainCol]))) {
                mainRow = i;
            }
        }
        return mainRow;
    }
    vec_n solve(int x_num) {
        int mainCol, mainRow; //ведущие столбец и строка
        while (!endingCond())
        {
            mainCol = findMainCol();
            if (mainCol == -1) {
                std::cout << "Error" << '\n';
                break;
            }
            mainRow = findMainRow(mainCol);
            basis[mainRow] = mainCol;
            mat_mn new_table = zeros(m, n);
            std::cout << "mainCol: " << mainCol << " mainRow: " << mainRow << '\n';
            for (int j = 0; j < n; j++) {
                new_table[mainRow][j] = table[mainRow][j] / table[mainRow][mainCol];
            }

            for (int i = 0; i < m; i++) {
                if (i != mainRow) {
                    for (int j = 0; j < n; j++) {
                        new_table[i][j] = table[i][j] - table[i][mainCol] * new_table[mainRow][j];
                    }
                }
            }
            table = new_table;
            this->printBasis();
            this->printTable();
            std::cout << '\n';
        }
        vec_n result;
        //заносим в result найденные значения X
        for (int i = 0; i < x_num; i++)
        {
            int k = -1;
            for (int j = 0; j < basis.size(); j++) {
                if (i + 1 == basis[j]) {
                    k = j;
                    break;
                }
            }
            if (k != -1) {
                result.push_back(table[k][0]);
            }
            else {
                result.push_back(0);
            }
        }
        return result;
    }
    void printTable() {
        for (auto a : table) {
            std::cout << a << '\n';
        }
    }
    void printBasis() {
        std::cout << "Basis: " << basis << '\n';
    }
};

int main()
{
    vec_n x0 = { 0, 0 };
    vec_n x1 = { 5, 3 };
    mat_mn m = { {7.0, -2.0}, { 1.0, 1.0}, {-7.0, 7.0} };
    std::cout << "x0 = " << x0 << ", x1 = " << x1 << '\n';
    std::cout << "bisect            : " << bisect(f, x0, x1) << '\n';
    std::cout << "coord descend     : " << coordDescend(f, x0) << '\n';
    std::cout << "grad descend      : " << gradientDescend(f, x0) << '\n';
    std::cout << "conj grad descend : " << conjugateGradDescend(f, x0) << '\n';
    std::cout << "newton_raphson    : " << newtoneRaphson(f, x0) << '\n';
    //std::cout << "newton_raphson    : " << newtoneRaphson(penalty1, x0) << '\n';
    //std::cout << "newton_raphson    : " << newtoneRaphson(penalty2, x0) << '\n';
    std::cout << "internal penalty  : " << newtoneRaphson_with_penalty(internal_penalty2, x0) << '\n';
    penalty_coef = 2;
    delta_pen = 0.5;
    std::cout << "external penalty  : " << newtoneRaphson_with_penalty(external_penalty2, x0) << '\n';
    mat_mn source = { {25, -3,  5},
                      {30, -2,  5},
                      {10,  1,  0},
                      { 6,  3, -8},
                      { 0, -6, -5} };
    vec_n i = { -1, -1, -1, -1 };
    simplex s(source, false, i);
    std::cout << "simplex           : " << s.solve(2) << '\n';
    source = { {240, 2, 3, 6},
               {200, 4, 2, 4},
               {160, 4, 6, 8},
               {0, -4, -5, -4} }; 
    i = { -1, -1, -1 };
    simplex s1(source, false, i);
    std::cout << "simplex           : " << s1.solve(3) << '\n';
    // mat_mn source = { {40, -2, 6},
    //            {28, 3, 2},
    //            {14, 2, -1},
    //            {0, -2, -3} };
    // vec_n i = { -1, -1, -1 };
    // simplex s2(source, false, i);
    // std::cout << "simplex           : " << s2.solve(2) << '\n';
    return 0;
}
