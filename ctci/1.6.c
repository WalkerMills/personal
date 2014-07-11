#include <stdio.h>
#include <stdlib.h>

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

void m_rotate (Matrix *m) {
    unsigned N = m_width(m);
    int top, right, bottom, left;

    if (m_height(m) != m_width(m)) {
        fprintf(stderr, "Error: cannot rotate non-square matrix\n");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i <= (N / 2) - ((N + 1) % 2); i++) {
        for (int j = i; j <= N - 2 - i; j++) {
            top = m_get(m, i, j);
            right = m_get(m, j, N - 1 - i);
            bottom = m_get(m, N - 1 - i, N - 1 - j);
            left = m_get(m, N - 1 - j, i);

            m_put(m, i, j, right);
            m_put(m, j, N - 1 - i, bottom);
            m_put(m, N - 1 - i, N - 1 - j, left);
            m_put(m, N - 1 - j, i, top);
        }
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

int main () {
    Matrix *m = m_create(5, 5);

    for (unsigned i = 0; i < 5 * 5; i++) {
        m_put(m, i / 5, i % 5, i);
    }

    m_print(m);
    m_rotate(m);
    printf("\n");
    m_print(m);

    return 0;
}
