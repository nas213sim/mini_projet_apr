#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <time.h>

#define N 4 // Size of matrix A (N x N)
#define BLOCK_SIZE (N/4) // Size of each block

void createVector(int V[N]){
    for (int i = 0; i < N; ++i)
    {
        V[i] = rand() % 15;
    }
}

void writeVector(int V[N]){
    for (int i = 0; i < N; ++i)
    {
        printf("%d ", V[i]);
    }
}

void createMatrix(int M[N][N]){
    int i,j;
    for(i=0; i<N; i++){
        for(j=0;j<N;j++){
            M[i][j] = rand() % 15;
        }
    }
}

void writeMatrix(int M[N][N]){
    int i,j;
    for (i=0; i < N; ++i)
    {
        printf("\n");
        for(j=0;j<N;j++){
            printf("%d ", M[i][j]);
        }
    }
    printf("\n");
}

void multiplyMatrixInteger(int result[BLOCK_SIZE], int M[BLOCK_SIZE][N], int V, int ROWS, int COLS)
{
    for (int i = 0; i < ROWS; i++) {
        result[i] = 0;
        for (int j = 0; j < COLS; j++) {
            result[i] += M[i][j] * V;
        }
    }
}

int main(int argc, char** argv) {
    int rank, size;
    srand(time(NULL));
    int A[N][N], X[N], Y[N], local_A[BLOCK_SIZE][N], local_X[N], local_Y[BLOCK_SIZE];
    int number;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if(size != N){
        if(rank == 0){
           printf("le nombre de processus n'égale pas la taille N de matrice"); 
        }
        printf("\n");
        
        MPI_Finalize();
        return -1;
    }
    if(rank == 0)
    {
        createMatrix(A);
        printf("Matrix");
        writeMatrix(A);
        printf("\n");

        createVector(X);
        printf("Vector\n");
        writeVector(X);

    }
    printf("\n");
    // Scatter matrix A to each process
    MPI_Scatter(A, BLOCK_SIZE * N, MPI_INT, local_A, BLOCK_SIZE * N, MPI_INT, 0, MPI_COMM_WORLD);
    // scatter vector X to all process including the root
    MPI_Scatter(X, 1, MPI_INT, &number, 1, MPI_INT, 0, MPI_COMM_WORLD);
    for(int i=0; i<BLOCK_SIZE; i++){
        printf("\n");
        for (int j = 0; j < N; j++)
        {
            printf("%d\t", local_A[i][j]);
        }
        printf("\n");
    }
    printf("process: %d,number %d", rank, number);
    printf("\n");
    multiplyMatrixInteger(local_Y, local_A, number, BLOCK_SIZE, N);
    // Gather local results to vector Y on the root process
    MPI_Gather(local_Y, BLOCK_SIZE, MPI_INT, Y, BLOCK_SIZE, MPI_INT, 0, MPI_COMM_WORLD);
    // Print the resulting vector Y on the root process
    if (rank == 0) {
        printf("Resulting vector Y:\n");
        writeVector(Y);
        printf("\n");
    }
    MPI_Finalize();
    return 0;
}
