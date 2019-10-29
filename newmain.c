#include "pessoa.h"
#include "playlist.h"
#include <unistd.h>
#include <sys/stat.h>

int main(){  
    char diretorio[1000];
    getcwd(diretorio, sizeof(diretorio));
    
    //Inicializa a lista principal do trabalho que é a de pessoas
    tListaP* lista = criaListaP();
    recebePessoasEAmigos(lista); // Lê o arquivos de entrada, insere pessoas na lista, determina os amigos
    recebePlaylists(lista); // Lê os arquivos de playlists e insere as playlists nas pessoas

    refatoraPlaylist(lista); // Separa as playlists atuais de cada pessoa da lista por cantor/banda
    exportaArquivosRefatoradas(lista, diretorio); // Gera o arquivo played-refatorada.txt  
    geraSimilaridades(lista); // Gera o arquivo de similaridades
    destroiListaDePessoas(lista);
}