#include <iostream>
#include <fstream>
using namespace std;

struct list { // список
	double value; // значение
	list *prev, *next;  // указатели на следующий и предыдущие элементы
};
list* init(double a)  // создаем первый элемент списка
{
	list *lst = new list;
	lst->value = a;
	lst->next = NULL; // указатель на следующий узел
	lst->prev = NULL; // указатель на предыдущий узел
	return lst;
}
list* pushback(list* last, double k) // добавляем элемент в конец списка.
{
    list* p = new list;
    p->value = k;
    p->prev = last;
    p->next = NULL;
    last->next = p;
    return p;
}
void erase(list *&head) { // удаление списка
    while (head) {
        list* s = head;
        head = head->next;
        delete s;
    }
}

list* swap(list* lst1, list* lst2, list* head)
{
    list* prev1, * prev2, * next1, * next2;
    prev1 = lst1->prev;  // узел предшествующий lst1
    prev2 = lst2->prev;  // узел предшествующий lst2
    next1 = lst1->next; // узел следующий за lst1
    next2 = lst2->next; // узел следующий за lst2
    if (lst2 == next1)  // обмениваются соседние узлы
    {
        lst2->next = lst1;
        lst2->prev = prev1;
        lst1->next = next2;
        lst1->prev = lst2;
        if (next2 != NULL)
            next2->prev = lst1;
        if (lst1 != head)
            prev1->next = lst2;
    }
    else if (lst1 == next2)  // обмениваются соседние узлы
    {
        lst1->next = lst2;
        lst1->prev = prev2;
        lst2->next = next1;
        lst2->prev = lst1;
        if (next1 != NULL)
            next1->prev = lst2;
        if (lst2 != head)
            prev2->next = lst1;
    }
    else  // обмениваются отстоящие узлы
    {
        if (lst1 != head)  // указатель prev можно установить только для элемента,
            prev1->next = lst2; // не являющегося корневым
        lst2->next = next1;
        if (lst2 != head)
            prev2->next = lst1;
        lst1->next = next2;
        lst2->prev = prev1;
        if (next2 != NULL) // указатель next можно установить только для элемента,
            next2->prev = lst1; // не являющегося последним
        lst1->prev = prev2;
        if (next1 != NULL)
            next1->prev = lst2;
    }
    return lst2;
}

void vuborsort(list *&head) { // сортировка выбором
    list* min = head;
    while (min->next) {  // проходим по всему списку от головы
        list* last = min->next;
        while (last) { // сравниваем элементы и выбираем наименьший
            if (min->value > last->value) {
                min = swap(min, last, head); // меняем ячейки местами
                if (!min->prev) { // проверяем, не съехала ни случайно голова списка
                    head = min;
                }
            }
            last = last->next;
        }
        min = min->next;
    }
}

void downHeap(list *&head, int k, int n) { // просеивание элементов через пирамиду.
    list* last = head;
    for (int i = 0; i - k; ++i) { // находим к-тый элемент списка.
        last = last->next;
    }
    while (k <= n / 2) {
        list* child = head;
        int l = 2 * k + 1; // номер "левого потомка"
        if (l <= n) {
            for (int i = 0; i - l; i++) { // находим его в списке, если он существует.
                child = child->next;
            }
        }
        if (l + 1 <= n && child->value < child->next->value) { // сравниваем значения обоих потомков, если правый больше, то выбираем его.
            child = child->next;
            ++l;
        }
        if (last->value >= child->value) { break; } // сравниваем большего потомка с родителем, если он больше, то меняем местами
        if (l <= n) {
            last = swap(last, child, head);
            while (head->prev) {
                head = head->prev;
            }
            child = head;
            for (int i = 0; i - l; i++) { // находим его в списке, если он существует.
                child = child->next;
            }
            last = child;
        }
        k = l;
    }
}
void piramidsort(list *&head, int n) { // сортировка пирамидой
    int i;
    for (i = n / 2; i >= 0; i--) { // создаем пирамиду первый раз
        downHeap(head, i, n - 1);
    }
    for (i = n - 1; i; i--) {
        list* last = head;
        for (int j = 0; j - i; j++) { 
            last = last->next;
        }
        head = swap(head, last, head);
        downHeap(head, 0, i - 1); // восстанавливаем пирамиду, забывая о отсортированных частях
    }
}

int main() {
    ifstream fin("input.txt");
    ofstream fout("output.txt");
    if (fin.is_open() && fout.is_open()) {
        double k;
        int a, n = 0;
        fin >> a >> k; // получаем определяющий сортировку номер и первый элемент списка
        list* last = init(k), * head = last; // создаем список
        ++n;
        while (fin >> k)
        {
            last = pushback(last, k); // заполняем список
            ++n;
        }
        if (a) {
            vuborsort(head);
        }
        else {
            piramidsort(head, n);
        }
        last = head;
        cout << n << ' '; // выводим число элементов на экран
        while (last)
        {
            cout << last->value << ' '; // выводим сам список
            last = last->next;
        }
        erase(head); // удаляем список
        fin.close();
        fout.close();
    }
	return 0;
}