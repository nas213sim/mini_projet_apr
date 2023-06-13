#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <time.h>

#define N 4 // Size of matrix A (N x N)

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

void multiplyMatrixInteger(int result[1], int M[N/2][N/2], int V, int ROWS, int COLS)
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
    int A[N][N], X[N], Y[N], local_A[N/2][N/2], local_X[N/2], local_Y[N/2];
    int number;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    //si le programme n'égale le nombre de processus voulue, le programme s'arrete 
    if(size != 16){
        if(rank == 0){
           printf("le nombre de processus n'égale pas la taille N de matrice, veuillez ecrire 16"); 
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

        createVector(X);
        printf("Vector\n");
        writeVector(X);
    }
    if(rank < 4){
        MPI_Scatter(X, 1, MPI_INT, &number, 1, MPI_INT, 0, MPI_COMM_WORLD);
    }
    // Scatter matrix A to each process
    MPI_Scatter(A, 1, MPI_INT, local_A, 1, MPI_INT, 0, MPI_COMM_WORLD);
    // Broadcast vector X to all process including the root
    multiplyMatrixInteger(local_Y, local_A, number, 1, 1);
    // Gather local results to vector Y on the root process
    MPI_Gather(local_Y, 1, MPI_INT, Y, 1, MPI_INT, 0, MPI_COMM_WORLD);
    printf("\n");
    // Print the resulting vector Y on the root process
    if (rank == 0) {
        printf("Resulting vector Y:\n");
        writeVector(Y);
        printf("\n");
    }
    MPI_Finalize();
    return 0;
}
