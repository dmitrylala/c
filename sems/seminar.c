// Спецификация:
// Считаем, что подаётся такая строка, что все её эл-ты
// вмещаются в некоторую область памяти, в которой располагается
// объявленная структура. Кол-во одинаковых символов в строке
// такое, что суммарное число не превосходит max от unsigned
// short int. Строка в моей программе задаётся инициализацией
// после объявления. Функция вывода дерева была написана для себя.
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Объявление структуры
struct tree 
{
	unsigned char c; 
	short a;
	int n; 
	struct tree* left; 
	struct tree* right; 
	struct tree* top; 
};

//Функция создания вершины дерева
struct tree* create_tree(unsigned char symbol) {
    struct tree *n = malloc(sizeof(*n));
    n -> left = NULL;
    n -> right = NULL;
    n -> top = NULL;
    n -> c = symbol;
    n -> n = 1;
    return n;
}

// Рекурсивный вариант функции добавления в дерево
void recursion_add(unsigned char symbol, struct tree *x) {
    struct tree *n;
    if (symbol == x -> c) {
        x -> n += 1;
    }
    if ((symbol > x -> c) && (x -> right != NULL)) {
        recursion_add(symbol, x -> right);

    }
    if ((symbol < x -> c) && (x -> left != NULL)) {
        recursion_add(symbol, x -> left);

    }
    if ((symbol > x -> c) && (x -> right == NULL)) {
        n = (struct tree*)malloc(sizeof(struct tree));
        x -> right = n;
        n -> top = x;
        n -> left = NULL;
        n -> right = NULL;
        n -> n = 1;
        n -> c = symbol;
    }
    if ((symbol < x -> c) && (x -> left == NULL)) {
        n = (struct tree*)malloc(sizeof(struct tree));
        x -> left = n;
        n -> top = x;
        n -> left = NULL;
        n -> right = NULL;
        n -> n = 1;
        n -> c = symbol;
    }
}

// Нерекурсивная функция удаления памяти
void delete_memory(struct tree *x) {
    struct tree *y;
    while ((x -> left != NULL) || (x -> right != NULL)) {
        y = x;
        while ((y -> left != NULL) || (y -> right != NULL)) {
            if (y -> left != NULL) {
                y = y -> left;
            }
            if (y -> right != NULL) {
                y = y -> right;
            }
        }
        if ((y -> top) -> left != NULL) {
            (y -> top) -> left = NULL;
        }
        else {
            (y -> top) -> right = NULL;
        }
        free(y);
    }
}

// Рекурсивная функция вывода дерева
// (Для себя)
void output_tree(struct tree *x) {
    printf("%c %d\n", x -> c, x -> n);
    if (x -> left != NULL) {
        output_tree(x -> left);
    }
    if (x -> right != NULL) {
        output_tree(x -> right);
    }
}

int 
main(void) {
    struct tree *x, *y;
    char str[256];
    
    fgets(str, 256, stdin); 
    int len = strlen(str);
    if (str[len - 1] == '\n') {
		str[len - 1] = '\0';
	}
    
    x = create_tree(str[0]);
    unsigned short len1, i;
    len1 = strlen(str);
    for (i = 1; i < len1; ++i) {
        y = x;
        recursion_add(str[i], y);
    } 
    y = x;
    output_tree(y);
    delete_memory(x);
    int *n = NULL;
    return 0;
}
