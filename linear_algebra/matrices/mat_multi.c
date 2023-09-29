#include "matrices.h"

// Multi

static u32 row;

static void* mthread(void* arg) {

    mat *matrices = (mat *)arg;
    mat _A = matrices[0];
    mat _B = matrices[1];
    mat _C = matrices[2];

    u32 i = row++;

    for (size_t j = 0; j < _A.n; j++) {
        for (size_t k = 0; k < _A.m; k++) {
           _C.elements[i][j] += _A.elements[i][k] * _B.elements[k][j];
        }
    }

    return NULL;
}

static mat naive_mthread(mat A, mat B) {

    size_t MAX_THREAD = A.m;

    row = 0;

    mat C = mat_zero(A.n, B.m);

    mat *matrices = (mat *)malloc(3 * sizeof(mat));

    matrices[0] = A;
    matrices[1] = B;
    matrices[2] = C;

    // declaring threads
    pthread_t threads[MAX_THREAD];

    // Creating four threads, each evaluating its own part
    for (size_t i = 0; i < MAX_THREAD; i++) {
        pthread_create(&threads[i], NULL, mthread, matrices);
    }

    // joining and waiting for all threads to complete
    for (size_t i = 0; i < MAX_THREAD; i++)
        pthread_join(threads[i], NULL);

    return C;
}

static mat naive(mat A, mat B) {
    if (A.m != B.n) {
	      LOG_ERROR("Action undefined on matrices of incompatible sizes (nxl and lxm): n: %f, l: %f and l: %f, m: %f", A.n, B.m, A.m, B.n);
        exit(1);
    }

    mat multi = new_matrix(A.n, B.m);

    for (size_t i = 0; i < A.n; i++) {
        for (size_t j = 0; j < B.m; j++) {
            multi.elements[i][j] = 0;

            for (size_t k = 0; k < B.n; k++) {
                multi.elements[i][j] += A.elements[i][k] * B.elements[k][j];
            }
        }

    }
    return multi;
}

mat mat_multi(mat a, mat b, bool mthread) {

    if (a.m != b.n) {
	      LOG_ERROR("Action undefined on matrices of incompatible sizes (nxl and lxm): n: %f, l: %f and l: %f, m: %f", A.n, B.m, A.m, B.n);
        exit(1);
    }

    mat c;

    if (mthread) {
        c = naive_mthread(a, b);
    }

    else {
        c = naive(a, b);
    }

    return c;
}