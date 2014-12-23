#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct _matrix {
    unsigned w;
    unsigned h;
    int **array;
} Matrix;

Matrix *m_create(unsigned x, unsigned y) {
    Matrix *m = (Matrix *) malloc(sizeof(Matrix));

    if (x == 0 || y == 0) {
        m->w = 0;
        m->h = 0;
        m->array = NULL;
    } else {
        m->w = x;
        m->h = y;
        m->array = (int **) calloc(x, sizeof(char *));
        for (int i = 0; i < x; i++) {
            m->array[i] = (int *) calloc(y, sizeof(char));
        }
    }

    return m;
}

unsigned m_width (Matrix *m) {
    return m->w;
}

unsigned m_height (Matrix *m) {
    return m->h;
}

int m_get (Matrix *m, unsigned i, unsigned j) {
    if (i < m_width(m) && j < m_height(m)) {
        return m->array[i][j];
    } else {
        fprintf(stderr, "Error: position (%d, %d) out of bounds\n", i, j);
        exit(EXIT_FAILURE);
    }
}

void m_put (Matrix *m, unsigned i, unsigned j, int n) {
    if (i < m_width(m) && j < m_height(m)) {
        m->array[i][j] = n;
    } else {
        fprintf(stderr, "Error: position (%d, %d) out of bounds\n", i, j);
        exit(EXIT_FAILURE);
    }
}

void m_print (Matrix *m) {
    unsigned N = m_width(m);
    int n;

    for (unsigned i = 0; i < N; i++) {
        for (unsigned j = 0; j < N; j++) {
            n = m_get(m, i, j);
            if (n < 10) {
                printf(" %d", n);
            } else {
                printf("%d", n);
            }
            printf(" ");
        }
        printf("\n");
    }
}

typedef struct _node {
    unsigned row;
    unsigned col;
    struct _node *next;
} Node;

Node *n_create (unsigned r, unsigned c) {
    Node *n = (Node *) malloc(sizeof(Node));
    n->row = r;
    n->col = c;
    n->next = NULL;

    return n;
}

void n_insert(Node *n, Node *new) {
    new->next = n->next;
    n->next = new;
}

void zeroes_from_list(Matrix *m, Node *head) {
    if (head == NULL) {
        return;
    }

    for (Node *n = head; n != NULL; n = n->next) {
        for (unsigned j = 0; j < m_width(m); j++) {
            m_put(m, n->row, j, 0);
        }
        for (unsigned i = 0; i < m_height(m); i++) {
            m_put(m, i, n->col, 0);
        }
    }
}

void zeroes_from_array(Matrix *m, int nrows, int ncols, int *rows, int *cols) {
    for (int r = 0; r < nrows; r++) {
        if (rows[r] != -1) {
            for (int j = 0; j < m_width(m); j++) {
                m_put(m, rows[r], j, 0);
            }
        }
    }
    for (int c = 0; c < ncols; c++) {
        if (cols[c] != -1) {
            for (int i = 0; i < m_height(m); i++) {
                m_put(m, i, cols[c], 0);
            }
        }
    }
}

void zeroes (Matrix *m) {
#ifndef ARRAY
    Node *head = (Node *) malloc(sizeof(Node));
    Node *tail = head;
#else
    int nrows = 0;
    int ncols = 0;
    int *rows = (int *) calloc(m_height(m), sizeof(int));
    int *cols = (int *) calloc(m_width(m), sizeof(int));
    memset(rows, -1, sizeof(int));
    memset(cols, -1, sizeof(int));
#endif

    for (int i = 0; i < m_height(m); i++) {
        for (int j = 0; j < m_width(m); j++) {
            if (m_get(m, i, j) == 0) {
#ifndef ARRAY
                Node *n = n_create(i, j);
                n_insert(tail, n);
                tail = n;
#else
                rows[nrows] = i;
                cols[ncols] = j;
                nrows++;
                ncols++;
#endif
            }
        }
    }

#ifndef ARRAY
    zeroes_from_list(m, head->next);
#else
    zeroes_from_array(m, nrows, ncols, rows, cols);
#endif
}

int main () {
    Matrix *m = m_create(8, 8);

    for (unsigned i = 0; i < 8 * 8; i++) {
        m_put(m, i / 8, i % 8, i % 23);
    }
    
    m_print(m);
    zeroes(m);
    printf("\n");
    m_print(m);

    return 0;
}
