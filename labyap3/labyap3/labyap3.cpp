#include <fstream>
using namespace std;

struct tree { 
	int val; // значение элемента
	tree *left, *right; // указатели на левое и правое поддерево.
};

tree* init(int a) { // создание нового узла дерева
	tree* p = new tree;
	p->val = a;
	p->left = NULL;
	p->right = NULL;
	return p;
}
void push(tree*&root, int a) { // добавление узла в дерево
	if (root) { // если корень дерева не пуст
		if (a <= root->val) { // то сравниваем новое значение с его значением, если меньше или равно, то налево
			if (root->left) { // если поддерево уже существует, то вызываем функцию рекурсивно
				push(root->left, a);
			}
			else { // или просто создаем новые поддерево
				root->left = init(a);
			}
		}
		else { // если больше, то направо
			if (root->right) {
				push(root->right, a);
			}
			else {
				root->right = init(a);
			}
		}
	}
	else { // если корня не существует, то создаем его.
		root = init(a);
	}
}
void treeprint(tree* root, ofstream &fout) { // вывод дерева с симметричным обходом
	if (root) {
		treeprint(root->left, fout); // обходим левое поддерево
		fout << root->val << ' '; // выводим значение элементов
		treeprint(root->right, fout); // обходим правое поддерево
	}
}
void mirror(tree*& root) { // отражаем дерево
	if (root) {
		mirror(root->left); // идем по левой ветке, меняя местами левые и правые поддеревья
		tree* t = root->left;
		root->left = root->right;
		root->right = t;
		mirror(root->left); // после идем по бывшей правой ветке, повторяя эти же действия.
	}
}
void erase(tree*& root) { // удаляем дерево
	if (root) {
		erase(root->left); // удаляем левое поддерево
		erase(root->right); // правое поддерево
		delete root; // сам корень
	}
}
int main() {
	ifstream fin("input.txt");
	ofstream fout("output.txt");
	if (fin.is_open()) {
		tree* root = NULL; // создаем указатель на корень дерева
		int a;
		while (fin >> a) { // считываем дерево
			push(root, a);
		}
		mirror(root); // отражаем дерево
		treeprint(root, fout); // выводим
		erase(root); // удаляем
		fin.close(); // закрываем файлы.
		fout.close();
	}
	return 0;
}