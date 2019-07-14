#ifndef ORDENACAOEXTERNA_H
#define ORDENACAOEXTERNA_H

#ifdef __cplusplus
extern "C" {
#endif

    enum boolean {
        true = 1, false = 0
    };
    typedef enum boolean bool;

    typedef FILE* ArqEntradaTipo;
    
    typedef struct {
        char chave;
        char dados[31];
    } TipoRegistro1;

    void OrdenaExterna1(char *arquivoEntrada, char* arquivoFinal, int n, int m, int f);

#ifdef __cplusplus
}
#endif

#endif /* ORDENACAOEXTERNA_H */

