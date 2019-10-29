#pragma once
#include "playlist.h"

typedef struct cel tPessoa;
typedef struct lista tListaP;
typedef struct item Item;
typedef struct play tPlay;
typedef struct listaPlaylist tListaPlaylist;


/*Inicializa a sentinela da lista de pessoas
 * inputs: nenhum
 * output: Sentinela inicializada
 * pre-condicao: nenhuma
 * pos-condicao: sentinela da lista de pessoas de retorno existe e os campos primeiro e ultimo apontam para NULL
 */

tListaP* criaListaP();

void inserePessoa(tListaP* listaP, Item* p);

void defineAmigos(tPessoa* p1, tPessoa* p2);

tPessoa* buscaPessoa(tListaP* listaP, char* nome);

void imprimeLista(tListaP* lista);

Item* inicializaItem(char* nome);

tListaPlaylist* criaListaPlaylists();

void insereAmigo(tListaP* listaP, tPessoa* p);

tListaPlaylist* inserePlaylist(tListaPlaylist* lista, tPlay* playlist);

void refatoraPlaylist(tListaP* lista);

tPlay* inicializaPlay(tPlaylist* playlist);

void usuarioRecebeListaPlaylists(tListaPlaylist* listaDePlaylists, tPessoa* pessoa);

tPlaylist* buscaPlaylistLista(tListaPlaylist* listaPlaylist, char* nome);

int buscaMusicaLista(tListaPlaylist* listaPlaylist, tMusica* m);

void destroiListaDePlaylists(tListaPlaylist* listaPlaylist);

void recebePessoasEAmigos(tListaP* lista);

void recebePlaylists(tListaP* lista);

void geraSimilaridades(tListaP* lista);

void exportaArquivosRefatoradas(tListaP* lista, char* diretorio);

char* retornaNomeUsuario(Item* usuario);

int comparaNomePlaylists(char* nomePlay1, char* nomePlay2);

int comparaNomeMusicas(char* nomeMusica1, char* nomeMusica2);

int calculaSimilaridades(tPessoa* pessoa1, tPessoa* pessoa2);

void destroiListaDePessoas(tListaP* lista);

void destroiListaDeAmigos(tListaP* amizades);