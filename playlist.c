#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "playlist.h"

// Struct tMusica que contém as informações de cada música e um ponteiro para a próxima musica da lista

struct musica {
    char *nome, *artista;
    tMusica* prox;
}; //tMusica

// Struct playlists que é uma lista de playlists contendo o nome de cada uma delas, a primeira e a ultima

struct playlist {
    char* nome;
    tMusica *ini, *fim;
}; //tPlaylist


// Cria uma playlist que é uma lista de músicas

tPlaylist* criaPlaylist(char* nomePlaylist) {
    tPlaylist* playlist = (tPlaylist*) malloc(sizeof (tPlaylist));

    playlist->nome = strdup(nomePlaylist);
    playlist->ini = NULL;
    playlist->fim = NULL;

    return playlist;
}

// Insere uma musica em determinada playlist

void insereMusica(tPlaylist* playlist, tMusica* m) {
    if (playlist->fim == NULL) {// Verifica se a lista esta vazia
        playlist->fim = playlist->ini = m;
        playlist->fim->prox = NULL;
        playlist->ini->prox = NULL;
    }
    else {
        playlist->fim->prox = m;
        playlist->fim = m;
        playlist->fim->prox = NULL;
    }
}

// Inicializa uma musica

tMusica* inicializaMusica(char* artista, char* nome) {
    tMusica* m = (tMusica*) malloc(sizeof (tMusica));

    m->nome = strdup(nome);
    m->artista = strdup(artista);

    return m;
}
// Retorna o nome de uma playlist

char* retornaNomePlaylist(tPlaylist* playlist) {
    return playlist->nome;
}

char* retornaNomeMusica(tMusica* m) {
    return m->nome;
}

// Retorna a musica inicial de uma playlist

tMusica* retornaMusicaIni(tPlaylist* playlist) {
    return playlist->ini;
}

// Retorna a proxima musica da playlist

tMusica* retornaMusicaProx(tMusica* m) {
        return m->prox;
}

char* retornaArtista(tMusica* m) {
    return m->artista;
}

tMusica* desencadeiaMusica(tMusica* m) {
    
    tMusica* x = inicializaMusica(m->artista, m->nome);
    x->prox=NULL;
    return x;
}

int buscaMusicaPlaylist(tPlaylist* playlist, tMusica* m) {
    tMusica* musica = playlist->ini;

    for (; musica != NULL; musica = musica->prox) {
        if (musica == NULL)
            break;

        if (!strcmp(musica->nome, m->nome))
            return 1;
    }
    return 0;
}

void destroiMusicas(tMusica* m) {
    if m == NULL
        return;
    
    destroiMusicas(m->prox);
    free(m->artista);
    free(m->nome);
    free(m);
}

void destroiPlaylist(tPlaylist* playlist) {
    tMusica* musica = playlist->ini;
    
    destroiMusicas(musica)
    
    free(playlist->nome);
    free(playlist);
}
