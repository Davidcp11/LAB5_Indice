/*  David Costa Pereira                               */
/*  Turma 3                                           */
/*  Exercicio 5: Indice de Livro                      */
/*                                                    */
/*  Programa compilado com CodeBlocks 20.03           */

#include <iostream>
#include <string.h>

typedef struct noh noh;
FILE *entrada, *saida;
///Struct com os ponteiros para o pai, irmao direito e filho esquerdo.
struct noh{
    char info[22];
    noh *ird = NULL;
    noh *fesq = NULL;
    noh *pai = NULL;
};
///**************************************************************
/// Procurar dado elemento na arvore. Recebe a raiz e destino. Retorna um ponteiro para o destino, se existir,
/// e NULL caso nao exista
noh * procurar(noh *x, char inf[22]){
    noh *p,
        *resposta;
    if(strcmp(inf,x->info)==0){
        return x;
    }
    else{
        resposta=NULL;
        p=x->fesq;
        while (p!=NULL and resposta==NULL){
            resposta = procurar(p,inf);
            p=p->ird;
        }
        return resposta;
    }
}
///********************************************************************
/// Recebe a raiz e um vetor que serve como indice do sumario.
/// Escreve todos os elementos da arvore no arquivo de saida.
void listar(noh *r, int i1[10]){
    int k;
    if(r!=NULL){
        for( k=0; i1[k]!=0;k++) {
            fprintf(saida, "%d.", i1[k]);
        }
        fprintf(saida,"      %s\n",r->info);
        i1[k]++;
        listar(r->fesq,i1);
        i1[k]--;
        i1[k-1]++;
        listar(r->ird,i1);
        i1[k-1]--;
    }
}
///*************************************************************************
/// Dar free em toda(irmao direito em diante e filho esquerdo em diante) a arvore a partir de um dado noh.
void apagar(noh *x){
    if(x!=NULL) {
        apagar(x->ird);
        apagar(x->fesq);
        free(x);
    }
}
///**************************************************************************
/// Remove um dado elemento e todos os seus filhos.
bool remover(noh *raiz, char inf[22]){
    noh *p, *x;
    x = procurar(raiz, inf);
    if(x!=NULL) {
        if(x!=raiz) {
            p = x->pai->fesq;
            if (x->pai->fesq == x) {
                x->pai->fesq = x->ird;
                apagar(x->fesq);
                free(x);
            } else {
                while (p->ird != x) {
                    p = p->ird;
                }
                p->ird = x->ird;
                apagar(x->fesq);
                free(x);
            }
            return true;
        } else{
            apagar(x->fesq);
            return true;
        }
    }
    return false;
}
///*************************************************************************
/// Retira o noh da arvore
void retirar(noh *x){
    noh *p;
    p = x->pai->fesq;
    if(x == p){
        x->pai->fesq = x->ird;
    }
    else{
        while(p->ird == x){
            p = p->ird;
        }
        p->ird = x->ird;
    }
}
///*************************************************************************
/// Coloca um dado noh na posicao requirada, com comando sub.
bool transferir_sub(noh *raiz, noh *x, char inf[22]){
    noh *p1;
    p1 = procurar(raiz,inf);
    if(p1!=NULL){
        if(p1->fesq!=NULL){
            p1 = p1->fesq;
            while (p1->ird!=NULL)
                p1 = p1->ird;
            p1->ird = x;
            x->pai = p1->pai;
            x->ird = NULL;
        }
        else{
            p1->fesq = x;
            x->pai = p1;
            x->ird = NULL;
        }
        return true;
    }
    return false;
}
///*********************************************************************
/// Coloca um dado noh na posicao requirada, com comando seguinte_a.
bool transferir_seg(noh *raiz, noh *x, char inf[22]){
    noh *p1;
    p1 = procurar(raiz, inf);
    if(p1!= NULL){
        x->ird = p1->ird;
        p1->ird = x;
        x->pai = p1->pai;
        return true;
    } else return false;

}
int main() {
    ///Declaracao de variaveis
    int  indice[10],
         cont = 1;
    char leitura[72],
         secao[22],
         entrega[22],
         comando[12],
         destino[22];
    bool linha = true, encontrou, titulo = false, lista_vazia = true;
    noh *raiz, *aux, *aux2;
    entrada = fopen("entrada5.txt", "r");
    saida = fopen("Lab5_David_Costa_Pereira.txt", "w");
    raiz = (noh *) (malloc(sizeof(noh)));
    raiz->fesq = NULL;
    raiz->ird = NULL;
    fprintf(saida,"Bom dia flor do dia\n");
    fprintf(saida,"Bom dia flor do dia\n");
    fprintf(saida,"Bom dia flor do dia\n");
    fprintf(saida,"--------------------------------------------------\n");
    for (int i = 0; i < 4; i++)
        fgets(leitura, 72, entrada);
    do {
        fscanf(entrada, " %s ", leitura);
        ///Comando para inseir/alterar o titulo do livro.
        if (strcmp(leitura, "TITULO") == 0) {
            if(titulo == false) {
                fscanf(entrada, " %s ", secao);
                strcpy(raiz->info, secao);
                titulo = true;
                lista_vazia = false;
            } else{
                fscanf(entrada, " %s ", secao);
                aux = (noh *) (malloc(sizeof(noh)));
                strcpy(aux->info, secao);
                aux->fesq=raiz;
                aux->ird = NULL;
                raiz->pai=aux;
                raiz=aux;
                lista_vazia = false;
            }
        }
        if(titulo) {
            ///Comando inserir, devemos inserir o noh em dado local da arvore.
            if (strcmp(leitura, "INSERIR") == 0) {
                fscanf(entrada, " %s ", secao);
                fscanf(entrada, " %s ", comando);
                fscanf(entrada, " %s ", destino);
                if (strcmp(comando, "SUB") == 0) {
                    aux = procurar(raiz, destino);
                    if (aux != NULL) {
                        if (aux->fesq != NULL) {
                            aux = aux->fesq;
                            while (aux->ird != NULL)
                                aux = aux->ird;
                            aux->ird = (noh *) (malloc(sizeof(noh)));
                            aux->ird->pai = aux->pai;
                            aux = aux->ird;
                            aux->fesq = NULL;
                            aux->ird = NULL;
                            strcpy(aux->info, secao);
                        } else {
                            aux->fesq = (noh *) (malloc(sizeof(noh)));
                            aux->fesq->pai = aux;
                            aux = aux->fesq;
                            aux->fesq = NULL;
                            aux->ird = NULL;
                            strcpy(aux->info, secao);
                        }
                    } else {
                        if (!linha) {
                            fprintf(saida, "--------------------------------------------------\n");
                            linha = true;
                        }
                        fprintf(saida, "ERRO: nao encontrado item %s\n", destino);
                    }
                }
                if (strcmp(comando, "SEGUINTE_A") == 0) {
                    aux = procurar(raiz, destino);
                    if (aux != NULL and aux != raiz) {
                        aux2 = (noh *) (malloc(sizeof(noh)));
                        aux2->pai = aux->pai;
                        aux2->ird = aux->ird;
                        aux->ird = aux2;
                        aux2->fesq = NULL;
                        strcpy(aux2->info, secao);
                    } else {
                        if (!linha) {
                            fprintf(saida, "--------------------------------------------------\n");
                            linha = true;
                        }
                        if (aux == raiz) {
                            fprintf(saida, "Operacao invalida\n");
                        } else {
                            fprintf(saida, "ERRO: nao encontrado item %s\n", destino);
                        }
                    }
                }
            }
            ///Comando REMOVER, devemos remover o noh e todos os seus filhos.
            if (strcmp(leitura, "REMOVER") == 0) {
                fscanf(entrada, " %s ", destino);
                if (strcmp(destino, raiz->info) == 0) {
                    lista_vazia = true;
                    titulo = false;
                    raiz->ird=NULL;
                    raiz->fesq=NULL;
                    raiz->pai=NULL;
                }
                if (!remover(raiz, destino)) {
                    if (!linha) {
                        fprintf(saida, "--------------------------------------------------\n");
                        linha = true;
                    }
                    fprintf(saida, "ERRO: nao encontrado item %s\n", destino);
                }

            }
            /// Caso o comando seja LISTA, devemos listar a situacao da arvore, mantendo sua hierarquia.
            if (strcmp(leitura, "LISTA") == 0) {
                if (!lista_vazia) {
                    fprintf(saida, "--------------------------------------------------\n");
                    linha = false;
                    fprintf(saida, "LISTA %d\n\n", cont);
                    fprintf(saida, "      %s\n", raiz->info);
                    cont++;
                    for (int i = 0; i < 10; i++) {
                        indice[i] = 0;
                    }
                    indice[0] = 1;
                    listar(raiz->fesq, indice);
                } else {
                    if (!linha) {
                        fprintf(saida, "--------------------------------------------------\n");
                        linha = true;
                    }
                    fprintf(saida, "LISTA VAZIA\n");
                }
            }
            ///Caso o comando seja TRANSFERIR, teremos dois tipos de comando TRANSFERIR, o SUB e o SEGUINTE-A.
            if (strcmp(leitura, "TRANSFERIR") == 0) {
                fscanf(entrada, " %s ", entrega);
                fscanf(entrada, " %s ", comando);
                fscanf(entrada, " %s ", destino);
                ///Caso o comando seja SUB:
                if (strcmp(comando, "SUB") == 0) {
                    aux = procurar(raiz, entrega);
                    if (aux != NULL) {
                        retirar(aux);
                        encontrou = transferir_sub(raiz, aux, destino);
                        if (!encontrou) {
                            if (!linha) {
                                fprintf(saida, "--------------------------------------------------\n");
                                linha = true;
                            }
                            fprintf(saida, "ERRO: nao encontrado item %s\n", destino);
                        }
                    } else {
                        if (!linha) {
                            fprintf(saida, "--------------------------------------------------\n");
                            linha = true;
                        }
                        fprintf(saida, "ERRO: nao encontrado item %s\n", entrega);
                    }
                }
                ///Caso o comando seja SEGUINTE_A:
                if (strcmp(comando, "SEGUINTE_A") == 0) {
                    {
                        aux = procurar(raiz, entrega);
                        if (aux != NULL and strcmp(destino, raiz->info) != 0) {
                            retirar(aux);
                            encontrou = transferir_seg(raiz, aux, destino);
                            if (!encontrou) {
                                if (!linha) {
                                    fprintf(saida, "--------------------------------------------------\n");
                                    linha = true;
                                }
                                fprintf(saida, "ERRO: nao encontrado item %s\n", destino);
                            }
                        } else {
                            if (!linha) {
                                fprintf(saida, "--------------------------------------------------\n");
                                linha = true;
                            }
                            if (strcmp(destino, raiz->info) == 0) {
                                fprintf(saida, "Operacao invalida\n");
                            } else {
                                fprintf(saida, "ERRO: nao encontrado item %s\n", entrega);
                            }
                        }
                    }
                    if (strcmp(comando, "SEGUINTE_A") == 0) {
                    }
                }
            }
        } else{
            fgets(leitura,72,entrada);
            fprintf(saida, "Nao ha titiulo ainda\n");
        }
    } while (strcmp(leitura,"FIM")!=0);
    fprintf(saida, "--------------------------------------------------\n");
    ///Fechando os arquivos de entrada e saida.
    fclose(entrada);
    fclose(saida);
    return 0;
}