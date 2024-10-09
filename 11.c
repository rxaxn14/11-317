#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

#define N 10

int main(int argc, char** argv) {
    int rank, size;
    int vectorA[N], vectorB[N], vectorC[N];
    int suma_pares = 0, suma_impares = 0;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (size != 3) {
        if (rank == 0) {
            printf("Este programa necesita exactamente 3 procesadores.\n");
        }
        MPI_Finalize();
        return 1;
    }

    if (rank == 0) {
        for (int i = 0; i < N; i++) {
            vectorA[i] = i + 1;
            vectorB[i] = (i + 1) * 2;
        }
    }

    MPI_Bcast(vectorA, N, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(vectorB, N, MPI_INT, 0, MPI_COMM_WORLD);

    if (rank == 1) {
        for (int i = 1; i < N; i += 2) {
            vectorC[i] = vectorA[i] + vectorB[i];
        }
        MPI_Send(vectorC, N, MPI_INT, 0, 0, MPI_COMM_WORLD);
    }

    if (rank == 2) {
        for (int i = 0; i < N; i += 2) {
            vectorC[i] = vectorA[i] + vectorB[i];
        }
        MPI_Send(vectorC, N, MPI_INT, 0, 0, MPI_COMM_WORLD);
    }

    if (rank == 0) {
        MPI_Recv(vectorC, N, MPI_INT, 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        MPI_Recv(vectorC, N, MPI_INT, 2, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        
        printf("Resultado de la suma de los vectores:\n");
        for (int i = 0; i < N; i++) {
            printf("%d ", vectorC[i]);
        }
        printf("\n");
    }

    MPI_Finalize();
    return 0;
}
