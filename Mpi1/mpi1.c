#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <time.h>

#define N 4 // La taille de matrice A N*N
#define BLOCK_SIZE (N/4) // La taille de block de ligne

//des fonctions pour créer des matrice et vecteurs aléatoirement
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
//fonction qui fait la multiplication
void multiplyMatrixVector(int result[BLOCK_SIZE], int M[BLOCK_SIZE][N], int V[N], int ROWS, int COLS)
{
    for (int i = 0; i < ROWS; i++) {
        result[i] = 0;
        for (int j = 0; j < COLS; j++) {
            result[i] += M[i][j] * V[j];
        }
    }
}

int main(int argc, char** argv) {
    int rank, size;
    srand(time(NULL));
    int A[N][N], X[N], Y[N], local_A[BLOCK_SIZE][N], local_X[N], local_Y[BLOCK_SIZE];
    //on initialise MPI ici
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
    // Ici on distribute un block de ligne de matrice A pour tous les processus
    MPI_Scatter(A, BLOCK_SIZE * N, MPI_INT, local_A, BLOCK_SIZE * N, MPI_INT, 0, MPI_COMM_WORLD);
    // on fait le broadcast pour le vecteur X a tous les processus
    MPI_Bcast(X, N, MPI_INT, 0, MPI_COMM_WORLD);
    //loop pour voir le resultat de distribution
    for(int i=0; i<BLOCK_SIZE; i++){
        printf("\n");
        for (int j = 0; j < N; j++)
        {
            printf("%d\t", local_A[i][j]);
        }
    }
    for(int i=0; i<N; i++){
        printf("process :%d, local_X: %d\t", rank, X[i]);
    }
    printf("\n");
    //on appelle la fonction pour multiplication
    multiplyMatrixVector(local_Y, local_A, X, BLOCK_SIZE, N);
    // aprés avoir terminer la multiplication, on prends des resultats et on les stock dans nouveau vecteur Y
    MPI_Gather(local_Y, BLOCK_SIZE, MPI_INT, Y, BLOCK_SIZE, MPI_INT, 0, MPI_COMM_WORLD);
    // on affiche le resultat dans processus 0 (root)
    if (rank == 0) {
        printf("Resulting vector Y:\n");
        writeVector(Y);
        printf("\n");
    }
    //on termine programme mpi 
    MPI_Finalize();
    return 0;
}
