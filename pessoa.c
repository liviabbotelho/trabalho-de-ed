#include "pessoa.h"
#include "playlist.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>

// Struct item que contém as informações de cada pessoa da lista
struct item {
    char* nome;
    tListaP* amigos;
    tListaPlaylist* playlists;
};

// Struct tPessoa que possibilita o encadeamento dessa lista por conter o ponteiro para a próxima pessoa da lista
struct cel {
    Item* usuario;
    tPessoa* prox;
}; //tPessoa

//
struct lista {
    tPessoa *ini, *fim;
}; //tListaP

//
struct play {
    tPlaylist* playlist;
    tPlay* prox;
}; //tPlay

//
struct listaPlaylist {
    tPlay *ini, *fim;
}; //tListaPlaylist



// Cria a lista de pessoas
tListaP* criaListaP() {
    tListaP* listaP = (tListaP*) malloc(sizeof (tListaP));

    listaP->ini = NULL;
    listaP->fim = NULL;

    return listaP;
}

tListaPlaylist* criaListaPlaylists() {
    tListaPlaylist* playlists = (tListaPlaylist*) malloc(sizeof (tListaPlaylist));

    playlists->ini = NULL;
    playlists->fim = NULL;

    return playlists;
}

void recebePlaylists(tListaP* lista) {
    FILE *arqEntradaListaPlaylists, *arqEntradaPlaylist;
    char nomePessoa[51];
    char nomePlaylist[51];
    char nomeArtistaAux[51];
    char nomeMusica[1000];
    char nomeArtista[51];
    char consomeCaracter;
    int i = 0, j = 0, contadorStr = 0;
    tPessoa* p1;
    tPessoa* p2;
    tPessoa* p;

    arqEntradaListaPlaylists = fopen("playlists.txt", "r"); // Abre arquivo de usuários e respectivas playlists

    // Ler arquivo de playlists
    while (1) {
        fscanf(arqEntradaListaPlaylists, "%[^;]s", nomePessoa); // Ler a primeira pessoa
        fscanf(arqEntradaListaPlaylists, "%c", &consomeCaracter); // Consome o ponto e vírgula
        // Caso seja o fim do arquivo o laço é quebrado
        if (feof(arqEntradaListaPlaylists))
            break;

        p = buscaPessoa(lista, nomePessoa); // Localiza a pessoa na lista
        tListaPlaylist* playlists = criaListaPlaylists(); // Cria uma lista de playlists para essa pessoa

        // Caso não encontre a pessoa imprime mensagem de erro e retorna
        if (p == NULL) {
            printf("Uma ou mais pessoas da lista de playlists não existe. O Programa foi encerrado!\n");
            exit(1);
        }

        fscanf(arqEntradaListaPlaylists, "%d", &j); // Ler quantidade de playlists que a pessoa possui
        fscanf(arqEntradaListaPlaylists, "%c", &consomeCaracter); // Consome o ponto e vírgula

        // Laço itera até a quantidade de playlists previamente estabelecidas
        for (i = 0; i < j; i++) {

            fscanf(arqEntradaListaPlaylists, "%[^\n;]s", nomePlaylist); // Ler o nome da i-ésima playlist
            fscanf(arqEntradaListaPlaylists, "%c", &consomeCaracter); // Consome caracteres não utilizados

            tPlaylist* playlist = criaPlaylist(nomePlaylist); // Cria uma playlist
            //printf("%s\n", nomePlaylist);
            arqEntradaPlaylist = fopen(nomePlaylist, "r"); // Abre o arquivo com o nome da i-ésima playlist

            if (arqEntradaPlaylist == NULL) {
                printf("Poxa... Um dos usuários possui uma ou mais playlists que não constam em nosso sistema. O programa foi encerrado!\n");
                exit(1);
            }
            // Itera incluindo os nomes das músicas e respectivos artistas na playlist criada
            while (1) {
                fscanf(arqEntradaPlaylist, "%[^-]s", nomeArtista); // Ler até hifen
                fscanf(arqEntradaPlaylist, "%c", &consomeCaracter); // Consome hífen
                fscanf(arqEntradaPlaylist, "%c", &consomeCaracter); // Consome um espaço em branco
                // Caso seja o fim do arquivo o laço é quebrado
                if (feof(arqEntradaPlaylist))
                    break;

                //Remove o espaço do fim do nome do artista
                for (contadorStr = 0; nomeArtista[contadorStr] != '\0'; contadorStr++) {
                    nomeArtistaAux[contadorStr] = nomeArtista[contadorStr];
                    if (nomeArtista[contadorStr + 1] == ' ' && nomeArtista[contadorStr + 2] == '\0') {
                        nomeArtistaAux[contadorStr + 1] = '\0';
                        break;
                    }
                }
                strcpy(nomeArtista, nomeArtistaAux);

                fscanf(arqEntradaPlaylist, "%[^\n]s", nomeMusica); // Ler o nome da música, em seguida, até a quebra de linha
                fscanf(arqEntradaPlaylist, "%c", &consomeCaracter); // Consome a quebra de linha
                insereMusica(playlist, inicializaMusica(nomeArtista, nomeMusica)); // Insere a música na playlist
            }

            playlists = inserePlaylist(playlists, inicializaPlay(playlist)); // Insere a nova playlist gerada em uma lista de playlists
            fclose(arqEntradaPlaylist); // Fecha o arquivo da playlist
        }

        usuarioRecebeListaPlaylists(playlists, p); // Usuário recebe a playlist recém criada em sua lista de playlists
    }

    fclose(arqEntradaListaPlaylists); // Fecha arquivo de usuários e respectivas playlists

}

void recebePessoasEAmigos(tListaP* lista) {
    FILE *arqEntradaAmizades = fopen("amizade.txt", "r"); // Abre arquivo amizades
    char nome1[51];
    char nome2[51];
    char consomeCaracter;

    // Caso o arquivo não exista, printa e finaliza o programa
    if (arqEntradaAmizades == NULL) {
        printf("Arquivo inexistente.\n");
        exit(1);
    }
    // Ler a primeira linha do arquivo, ou seja, os usuários do serviço
    while (consomeCaracter != '\n') {
        fscanf(arqEntradaAmizades, "%[^\n;]s", nome1);
        fscanf(arqEntradaAmizades, "%c", &consomeCaracter);
        inserePessoa(lista, inicializaItem(lista, nome1));
    }
    // À seguir, ler as amizades e as define
    while (1) {
        fscanf(arqEntradaAmizades, "%[^;]s", nome1); // Ler primeiro nome
        fscanf(arqEntradaAmizades, "%c", &consomeCaracter); // Consome o ponto e vírgula
        // Caso seja o fim do arquivo o laço é quebrado
        if (feof(arqEntradaAmizades))
            break;
        fscanf(arqEntradaAmizades, "%[^\n]s", nome2); // Ler segundo nome
        fscanf(arqEntradaAmizades, "%c", &consomeCaracter); // Consome a quebra de linha
        defineAmigos(buscaPessoa(lista, nome1), buscaPessoa(lista, nome2)); // Define a amizade mútua entre os pares indicados no arquivo
    }
    fclose(arqEntradaAmizades); // Fecha o arquivo amizades
}

// Insere uma pessoa na lista de pessoas

void inserePessoa(tListaP* listaP, Item* p) {
    tPessoa* nova = (tPessoa*) malloc(sizeof (tPessoa));
    tListaPlaylist* playlists = criaListaPlaylists();

    nova->usuario = p;

    if (listaP->fim == NULL) { // Verifica se a lista esta vazia
        listaP->fim = listaP->ini = nova;
        listaP->fim->prox = NULL;
        listaP->ini->prox = NULL;

    } else {
        listaP->fim->prox = nova;
        listaP->fim = nova;
        listaP->fim->prox = NULL;
    }
    nova->usuario->amigos = criaListaP(); // Cria lista de amigos;
    nova->usuario->playlists = playlists;
}

// Inicializa cada item

Item* inicializaItem(tListaP* lista, char* nome) {
    Item* p = (Item*) malloc(sizeof (Item));
    p->nome = strdup(nome);
    return p;
}

// Insere uma pessoa

void insereAmigo(tListaP* amigos, Item* p) {
    tPessoa* nova = (tPessoa*) malloc(sizeof (tPessoa));
    nova->usuario = p;

    if (amigos->fim == NULL) // Verifica se a lista esta vazia
        amigos->fim = amigos->ini = nova;

    else {
        amigos->fim->prox = nova;
        amigos->fim = nova;
    }
}

// Insere os amigos em suas listas de amizades

void defineAmigos(tPessoa* p1, tPessoa* p2) {

    if (p1 == NULL || p2 == NULL) // caso nao encontre a pessoa, retorna a função
        return;

    // Insere a pessoa p2 na lista de amizades de p1:
    if (p1->usuario->amigos->fim == NULL) {
        insereAmigo(p1->usuario->amigos, p2->usuario);
    } else {
        insereAmigo(p1->usuario->amigos, p2->usuario);
    }

    // Insere a pessoa p1 na lista de amizades de p2:
    if (p2->usuario->amigos->fim == NULL)
        insereAmigo(p2->usuario->amigos, p1->usuario);
    else {
        insereAmigo(p2->usuario->amigos, p1->usuario);
    }
}

// Busca uma pessoa na lista

tPessoa* buscaPessoa(tListaP* listaP, char* nome) {
    tPessoa* p;

    for (p = listaP->ini; p != NULL; p = p->prox) {
        if (!(strcmp(p->usuario->nome, nome)))
            return p;
    }
    return NULL;
}
// Imprime uma lista de pessoas

void imprimeLista(tListaP* lista) {
    tPessoa* p;

    for (p = lista->ini; p != NULL; p = p->prox) {
        printf("%s\n", p->usuario->nome);
    }
}
// Insere uma playlist na lista de playlists de uma pessoa

tListaPlaylist* inserePlaylist(tListaPlaylist* lista, tPlay* playlist) {

    if (lista->ini == NULL) { // Verifica se a lista esta vazia
        lista->fim = lista->ini = playlist;
        lista->fim->prox = NULL;
        lista->ini->prox = NULL;
        return lista;
    }
    else {
        lista->fim->prox = playlist;
        lista->fim = playlist;
        lista->fim->prox = NULL;
        return lista;
    }
}

// Inicializa uma celula tPlay a partir de um tPlaylist
tPlay* inicializaPlay(tPlaylist* playlist) {
    tPlay* p = (tPlay*) malloc(sizeof (tPlay));

    p->playlist = playlist;
    return p;
}

void usuarioRecebeListaPlaylists(tListaPlaylist* listaDePlaylists, tPessoa* pessoa) {
    pessoa->usuario->playlists = listaDePlaylists;
}

void exportaArquivosRefatoradas(tListaP* lista, char* diretorio) {
    FILE* arqSaidaPlayedRefatorada = fopen("played-refatorada.txt", "w");
    FILE* arqSaidaPlaylists;
    char endereco[1000];
    char enderecoAux[1000];
    char nomePasta[51];
    char nomePlaylistRefatorada[51];
    tPessoa* p = lista->ini;
    tPlay* celPlay;
    tMusica* musica;
    int numPlaylists = 0, retornomkdir;

    for (p; p != NULL; p = p->prox) {
        numPlaylists = 0;
        strcpy (endereco, diretorio);
        strcat (endereco, "/");
        strcpy (nomePasta, p->usuario->nome); 
        strcat (nomePasta, "/");
        strcat(endereco, nomePasta);
        retornomkdir = mkdir(endereco, 0700);
        
        for (celPlay = p->usuario->playlists->ini; celPlay != NULL; celPlay = celPlay->prox) {     
            strcpy(enderecoAux, endereco);
            strcpy (nomePlaylistRefatorada, retornaNomePlaylist(celPlay->playlist)); 
            strcat(enderecoAux, nomePlaylistRefatorada);
            strcat(enderecoAux, ".txt");
            arqSaidaPlaylists = fopen (enderecoAux, "w");
            
            for (musica=retornaMusicaIni(celPlay->playlist); musica!=NULL; musica=retornaMusicaProx(musica)){
                fprintf(arqSaidaPlaylists, "%s - %s\n", retornaArtista(musica), retornaNomeMusica(musica));
             }
            numPlaylists++;
        }


        fprintf(arqSaidaPlayedRefatorada, "%s;%d;", p->usuario->nome, numPlaylists);
        for (celPlay = p->usuario->playlists->ini; celPlay != NULL; celPlay = celPlay->prox) {
            fprintf(arqSaidaPlayedRefatorada, "%s.txt", retornaNomePlaylist(celPlay->playlist));

            if (celPlay->prox != NULL)
                fprintf(arqSaidaPlayedRefatorada, ";");
            else
                fprintf(arqSaidaPlayedRefatorada, "\n");
        }
    }

    fclose(arqSaidaPlayedRefatorada);
    fclose (arqSaidaPlaylists);
}

// Função que refatora/divide as playlists por artistas e detroi as playlists antigas
void refatoraPlaylist(tListaP* lista) {
    tPessoa* p = lista->ini;

    for (; p != NULL; p = p->prox) {
        tListaPlaylist* listaAux = criaListaPlaylists(); // Criação de uma nova lista de playlists (que será a refatorada e substituirá a lista de playlists da pessoa p ao final da função)
        tPlay* celPlaylist = p->usuario->playlists->ini; // Variavel que recebe a primeira playlist
 
        for (; celPlaylist != NULL; celPlaylist = celPlaylist->prox) { // Laço que roda a lista de criaListaPlaylists
            tMusica* celMus;
            celMus = retornaMusicaIni(celPlaylist->playlist); // Retorna a musica inicial para rodarmos celPlaylist (celula de playlist)

            while (celMus != NULL) { // Laço que roda a playlist
                tMusica* musica = desencadeiaMusica(celMus); // Inicializa uma música nova identica a celMus porém não está encadeada na playlist (prox nulo)

                if (!buscaMusicaLista(listaAux, musica)) { // Se a musica ainda não foi inserida na listaAux (lista de playlists refatoradas) 
                    if (!buscaPlaylistLista(listaAux, retornaArtista(musica))) { // Se a playlist refatorada desse artista ainda não foi criada
                        tPlay* nova = (tPlay*) malloc(sizeof (tPlay)); // Cria-se uma nova celúla de playlist
                        nova->playlist = criaPlaylist(retornaArtista(musica)); // Inicializa a playlist com o nome do artista
                        insereMusica(nova->playlist, musica); // Insere musica na playlist nova 
                        inserePlaylist(listaAux, nova); // Insere playlist na listaAux (lista de playlists refatoradas)
                    } 
                    else // Se a playlista refatorada já foi criada;
                        insereMusica(buscaPlaylistLista(listaAux, retornaArtista(musica)), musica); // Insere a musica na playlist do artista
                }
                celMus = retornaMusicaProx(celMus); // Atualiza a musica para a proxima
            }
        }
        destroiListaDePlaylists(p->usuario->playlists); // Destroi a lista de playlists antiga
        usuarioRecebeListaPlaylists(listaAux, p); // Insere listaAux (a lista de playlists refatoradas) no usuario
    }

}


tPlaylist* buscaPlaylistLista(tListaPlaylist* listaPlaylist, char* nome) {
    tPlay* p;

    for (p = listaPlaylist->ini; p != NULL; p = p->prox) {
        if (retornaNomePlaylist(p->playlist) == NULL)
            break;
        else if (!(strcmp(retornaNomePlaylist(p->playlist), nome))) // Verifica se o nome da playlist p é igual a variavel nome
            return p->playlist; // Retorna a playlist
    }
    return NULL;
}

int buscaMusicaLista(tListaPlaylist* listaPlaylist, tMusica* m) {
    tPlay* p = listaPlaylist->ini;

    for (; p != NULL; p = p->prox) {
        if (buscaMusicaPlaylist(p->playlist, m))
            return 1;
    }
    return 0;
}

// Função que destroi as lista de playlists
void destroiListaDePlaylists(tListaPlaylist* listaPlaylist) {
    tPlay* celPlay = listaPlaylist->ini;
    tPlay* x=celPlay;

    for (; x != NULL; celPlay = x) {
        x = celPlay->prox;
        destroiPlaylist(celPlay->playlist); // Destroi a playlist
        free(celPlay); // Destroi a célula de playlists
    }
    free(listaPlaylist); // Destroi a lista de playlists
}

// Função que destroi as lista de amizades
void destroiListaDeAmigos(tListaP* amizades){
    tPessoa* amigo = amizades->ini;
    tPessoa* p=amigo; // variável que salva o endereço do prox

    for (; p != NULL; amigo = p) {
        p = p->prox;
        free(amigo->usuario->nome); 
        free(amigo);
    }
    free(amizades); // Destroi o ponteiro para a lista
}

// Função que desroi a lista de pessoas
void destroiListaDePessoas(tListaP* lista){
    tPessoa* pessoa = lista->ini;
    tPessoa* p=pessoa;

    for (; p != NULL; pessoa = p) {
        p=p->prox;
        destroiListaDePlaylists(pessoa->usuario->playlists);
        destroiListaDeAmigos(pessoa->usuario->amigos);
        free(pessoa->usuario);
        free(pessoa); // Destroi a célula pessoa
    }
    free(lista); // Destroi o ponteiro para a lista de pessoas
}


int comparaNomePlaylists(char* nomePlay1, char* nomePlay2) {
    if (strcmp(nomePlay1, nomePlay2))
        return 0;
    else
        return 1;
}

int comparaNomeMusicas(char* nomeMusica1, char* nomeMusica2) {
    if (strcmp(nomeMusica1, nomeMusica2))
        return 0;
    else
        return 1;
}

int calculaSimilaridades(tPessoa* pessoa1, tPessoa* pessoa2) {
    int contador = 0;
    tPlay* p1 = pessoa1->usuario->playlists->ini;
    tPlay* p2 = pessoa2->usuario->playlists->ini;
    tMusica* m1;
    tMusica* m2;

    for (; p1 != NULL; p1 = p1->prox) {
        for (p2 = pessoa2->usuario->playlists->ini; p2 != NULL; p2 = p2->prox) {
            if (comparaNomePlaylists(retornaNomePlaylist(p1->playlist), retornaNomePlaylist(p2->playlist))) {
                for (m1 = retornaMusicaIni(p1->playlist); m1 != NULL; m1 = retornaMusicaProx(m1)) {
                    for (m2 = retornaMusicaIni(p2->playlist); m2 != NULL; m2 = retornaMusicaProx(m2)) {
                        if (comparaNomeMusicas(retornaNomeMusica(m1), retornaNomeMusica(m2)))
                            contador++;
                    }
                }
            }
        }
    }
    return contador;
}

void geraSimilaridades(tListaP* lista) {
    FILE *arqEntradaAmizades = fopen("amizade.txt", "r"); // Abre arquivo amizades
    FILE *arqSaidaSimilaridades = fopen("similaridades.txt", "w");
    char nome1[51];
    char nome2[51];
    char consomeCaracter;
    int quantidadeSimilaridades = 0, auxEnter = 0;

    // Ler a primeira linha do arquivo, ou seja, os usuários do serviço, não fará nada, pois não temos interesse nessa linha
    while (consomeCaracter != '\n') {
        fscanf(arqEntradaAmizades, "%[^\n;]s", nome1);
        fscanf(arqEntradaAmizades, "%c", &consomeCaracter);
    }
    // À seguir, ler os pares de amizades e gera as saídas de similaridades
    while (1) {
        fscanf(arqEntradaAmizades, "%[^;]s", nome1); // Ler primeiro nome
        fscanf(arqEntradaAmizades, "%c", &consomeCaracter); // Consome o ponto e vírgula
        // Caso seja o fim do arquivo o laço é quebrado
        if (feof(arqEntradaAmizades))
            break;
        if (auxEnter == 1)
            fprintf(arqSaidaSimilaridades, "\n"); // Artíficio para não imprimir o enter no fim do arquivo
        fscanf(arqEntradaAmizades, "%[^\n]s", nome2); // Ler segundo nome
        fscanf(arqEntradaAmizades, "%c", &consomeCaracter); // Consome a quebra de linha
        quantidadeSimilaridades = calculaSimilaridades(buscaPessoa(lista, nome1), buscaPessoa(lista, nome2)); // Conta as similaridades e retorna um inteiro
        fprintf(arqSaidaSimilaridades, "%s;%s;%d", nome1, nome2, quantidadeSimilaridades); // Printa no arquivo de saída os nomes e as similaridades
        auxEnter = 1;
    }
    fclose(arqEntradaAmizades); // Fecha o arquivo amizades
    fclose(arqSaidaSimilaridades); // Fecha o arquivo de saída

}