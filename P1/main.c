#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "OrdenacaoExterna1.h"

long int n;
long int m;
long int seg;

int main(int argc, char** argv) {
    int f = 2;
    n = 22;
    m = 3;
    
    printf("\nFAZENDO ORDENACAO EXTERNA...\n");

    OrdenaExterna1("files/registros1.bin", "files/output/final1.bin", n, m, f);

    printf("\nOrdem de intercalação: %d\nNumero de Registros: %li\nEspaco de Memoria: %li\n", f, n, m);

    return 0;
}
