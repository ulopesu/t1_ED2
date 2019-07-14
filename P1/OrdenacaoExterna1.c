#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "OrdenacaoExterna1.h"

void Insercao(TipoRegistro1* buffer, int nRegistrosLidos) {
    int i, j;
    TipoRegistro1 aux;
    for (i = 2; i <= nRegistrosLidos; i++) {
        aux = buffer[i];
        j = i - 1;
        buffer[0] = aux; /* sentinela */
        while (aux.chave < buffer[j].chave) {
            buffer[j + 1] = buffer[j];
            j--;
        }
        buffer[j + 1] = aux;
    }
}

bool verificaBufferVazio(TipoRegistro1 buffer[]) {
    if (buffer[1].chave == '\0') {
        return true;
    } else {
        return false;
    }
}

bool EnchePaginas(ArqEntradaTipo ArqEntrada, int* nRegistrosLidos, TipoRegistro1 buffer[], int n, int m) {
    int i;
    char chave, dados[31];
    *nRegistrosLidos = 0;

    for (i = 0; i < m; i++) {
        if (feof(ArqEntrada)) {
            return true;
        } else {
            if (fread(&chave, sizeof (char), 1, ArqEntrada) && fread(dados, sizeof (char), 31, ArqEntrada)) {
                buffer[i + 1].chave = chave;
                strcpy(buffer[i + 1].dados, dados);
                // IMPRESSAO DOS BLOCOS OBTIDOS//
                printf("%c", buffer[i + 1].chave);
                printf("%s", buffer[i + 1].dados);
                *nRegistrosLidos += 1;
            }
        }
    }

    printf("\n");

    if (feof(ArqEntrada)) {
        return true;
    }

    return false;
}

ArqEntradaTipo AbreArqSaida(int NBloco) {
    char nome[100];
    sprintf(nome, "files/output/%d.bin", (NBloco - 1));
    return fopen(nome, "wb+");
}

void DescarregaPaginas(ArqEntradaTipo ArqSaida, TipoRegistro1 *buffer, int nRegistrosLidos) {
    int i;
    for (i = 0; i < nRegistrosLidos; i++) {
        fwrite(&buffer[i + 1].chave, sizeof (char), 1, ArqSaida);
        fwrite(&buffer[i + 1].dados, sizeof (char), 31, ArqSaida);
    }
}

int Minimo(int a, int b) {
    if (a < b) {
        return a;
    } else {
        return b;
    }
}

void AbreArqEntrada(ArqEntradaTipo *ArrArqEnt, int Low, int Lim) {
    char nome[100], c;
    int j = 0, i;
    for (i = Low; i <= Lim; i++) {
        sprintf(nome, "files/output/%d.bin", i);
        ArrArqEnt[j] = fopen(nome, "r+");
        if (ArrArqEnt[j] == NULL) {
            break;
        } else {
            printf("\nNome do Arquivo de Entrada: %d.bin\n", i);
            printf("Conteudo:\n");
            while (fread(&c, sizeof (char), 1, ArrArqEnt[j])) {
                printf("%c", c);
            }
            printf("\t   ***FIM***\n");
            rewind(ArrArqEnt[j]);
        }
        j++;
    }
}

int ContCabValido(TipoRegistro1 **reg, int f) {
    int i, cont;
    for (i = 0; i < f; i++) {
        if (reg[i] != NULL) {
            cont++;
        }
    }
    return cont;
}

bool VerRegistroLimpo(TipoRegistro1 *reg) {
    if (reg->chave == 127 && reg->dados[0] == 127) {
        return true;
    } else {
        return false;
    }
}

void LimpaRegistro(TipoRegistro1 **reg, int i) {
    reg[i]->chave = 127;
    reg[i]->dados[0] = 127;
}

void LimpaCabecote(TipoRegistro1 **reg, int tam) {
    int i;
    for (i = 0; i < tam; i++) {
        reg[i]->chave = 127;
        reg[i]->dados[0] = 127;
    }
}

int MenorPosChaveRegistro(TipoRegistro1 **cabecote, int f) {
    int posmenorChave, i, aux = 0;
    char menorChave = 127;
    bool verRegLimpo;

    for (i = 0; i < f; i++) {
        if (cabecote[i] != NULL) {
            verRegLimpo = VerRegistroLimpo(cabecote[i]);
            if (!verRegLimpo) {
                if (aux == 0) {
                    menorChave = cabecote[i]->chave;
                    posmenorChave = i;
                    aux++;
                } else if (cabecote[i]->chave < menorChave) {
                    menorChave = cabecote[i]->chave;
                    posmenorChave = i;
                }
            }
        }
    }
    if (menorChave == 127) {
        return -1;
    }
    return posmenorChave;
}

TipoRegistro1** CriaRegistro(int tam) {
    TipoRegistro1 **reg;
    int i;
    reg = (TipoRegistro1**) malloc(sizeof (TipoRegistro1*) * tam);
    for (i = 0; i < tam; i++) {
        reg[i] = (TipoRegistro1*) malloc(sizeof (TipoRegistro1));
    }
    return reg;
}

TipoRegistro1** EliminaRegistro(TipoRegistro1 **reg, int tam) {
    int i;
    for (i = 0; i < tam; i++) {
        free(reg[i]);
    }
    free(reg);
    return NULL;
}

void Intercale(ArqEntradaTipo* ArrArqEnt, int Low, int Lim, ArqEntradaTipo ArqSaida, int f) {
    int i = 0;
    int arquivosFinalizados = 0;
    int posmenorChave;
    TipoRegistro1 **cabecote = CriaRegistro(f);
    LimpaCabecote(cabecote, f);

    for (i = 0; i < f; i++) {
        if (ArrArqEnt[i] == NULL) {
            arquivosFinalizados++;
        }
    }

    if (arquivosFinalizados == f) {
        cabecote = EliminaRegistro(cabecote, f);
        return;
    }

    do {
        for (i = 0; i < f; i++) {
            if (cabecote[i] != NULL && ArrArqEnt[i] != NULL) {
                if (VerRegistroLimpo(cabecote[i]) && !feof(ArrArqEnt[i])) {
                    fread(&cabecote[i]->chave, sizeof (char), 1, ArrArqEnt[i]);
                    fread(cabecote[i]->dados, sizeof (char), 31, ArrArqEnt[i]);
                } else if (feof(ArrArqEnt[i])) {
                    cabecote[i] = NULL;
                    arquivosFinalizados++;
                }
            }
        }

        if (f - arquivosFinalizados == 1) {
            for (i = 0; i < f; i++) {
                if (cabecote[i] != NULL && ArrArqEnt[i] != NULL) {
                    break;
                }
            }

            while (1) {
                if (!feof(ArrArqEnt[i])) {
                    if (VerRegistroLimpo(cabecote[i])) {
                        fread(&cabecote[i]->chave, sizeof (char), 1, ArrArqEnt[i]);
                        fread(cabecote[i]->dados, sizeof (char), 31, ArrArqEnt[i]);
                        if (!VerRegistroLimpo(cabecote[i])) {
                            fwrite(&cabecote[i]->chave, sizeof (char), 1, ArqSaida);
                            fwrite(cabecote[i]->dados, sizeof (char), 31, ArqSaida);
                            LimpaRegistro(cabecote, i);
                        }
                    } else {
                        fwrite(&cabecote[i]->chave, sizeof (char), 1, ArqSaida);
                        fwrite(cabecote[i]->dados, sizeof (char), 31, ArqSaida);
                        LimpaRegistro(cabecote, i);
                    }
                } else {
                    cabecote[i] = NULL;
                    arquivosFinalizados++;
                    break;
                }
            }
        } else if (f - arquivosFinalizados > 1) {
            posmenorChave = MenorPosChaveRegistro(cabecote, f);
            if (posmenorChave != -1) {
                fwrite(&cabecote[posmenorChave]->chave, sizeof (char), 1, ArqSaida);
                fwrite(cabecote[posmenorChave]->dados, sizeof (char), 31, ArqSaida);
                LimpaRegistro(cabecote, posmenorChave);
            }
        }
    } while (arquivosFinalizados < f);
    cabecote = EliminaRegistro(cabecote, f);
}

void ApagaArquivo(int i) {
    char nome[100];
    sprintf(nome, "files/output/%d.bin", i);
    remove(nome);
}

void OrdenaExterna1(char *arquivoEntrada, char* arquivoFinal, int n, int m, int f) {
    int NBlocos = 0;
    char ultimoArq[100], c;
    ArqEntradaTipo ArqEntrada1, ArqSaida;
    ArqEntradaTipo ArrArqEnt[f];
    TipoRegistro1 buffer[m + 1];
    bool Fim;
    int Low, High, Lim, nRegistrosLidos = 0, i, j;

    ArqEntrada1 = fopen(arquivoEntrada, "r+");

    if (ArqEntrada1 == NULL) {
        printf("Erro Arquivo de Entrada\n");
        return;
    }

    printf("Blocos Obtidos:\n");

    do /*Formacao inicial dos NBlocos ordenados */ {
        Fim = EnchePaginas(ArqEntrada1, &nRegistrosLidos, buffer, n, m);
        if (verificaBufferVazio(buffer)) {
            break;
        }
        NBlocos++;
        Insercao(buffer, nRegistrosLidos);
        ArqSaida = AbreArqSaida(NBlocos);
        DescarregaPaginas(ArqSaida, buffer, nRegistrosLidos);
        nRegistrosLidos = 0;
        fclose(ArqSaida);
    } while (!Fim);

    fclose(ArqEntrada1);

    Low = 0;
    High = NBlocos - 1;

    while (Low < High) { // Intercalacao dos NBlocos ordenados
        Lim = Minimo(Low + f - 1, High);

        // IMPRESSAO LOW, LIM, HIGH //
        printf("\n\nLow: %d, Lim: %d, High: %d.", Low, Lim, High);
        // IMPRESSÃO LOW, LIM, HIGH //

        AbreArqEntrada(ArrArqEnt, Low, Lim);
        High++;
        ArqSaida = AbreArqSaida(High + 1);
        Intercale(ArrArqEnt, Low, Lim, ArqSaida, f);

        // IMPRESSAO ARQUIVO DE SAIDA //
        printf("\nNome do Arquivo de Saida: %d.bin\n", High);
        printf("Conteudo:\n");
        rewind(ArqSaida);
        while (fread(&c, sizeof (char), 1, ArqSaida)) {
            printf("%c", c);
        }
        printf("\t   ***FIM***\n");
        // IMPRESSAO ARQUIVO DE SAIDA //

        fclose(ArqSaida);

        for (i = Low, j = 0; i <= Lim; i++, j++) {
            if (ArrArqEnt[j] != NULL) {
                fclose(ArrArqEnt[j]);
                ArrArqEnt[j] = NULL;
                ApagaArquivo(i);
            }
        }
        Low += f;
    } //Mudar o nome do arquivo FINAL(high) para o nome fornecido pelo usuario;

    sprintf(ultimoArq, "files/output/%d.bin", i);
    rename(ultimoArq, arquivoFinal);
}
