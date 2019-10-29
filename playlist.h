
typedef struct musica tMusica;
typedef struct playlist tPlaylist;

tPlaylist* criaPlaylist(char* nomePlaylist);

void insereMusica(tPlaylist* playlist, tMusica* m);

tMusica* inicializaMusica (char* nome, char* artista);

char* retornaNomePlaylist(tPlaylist* playlist);

tMusica* retornaMusicaIni(tPlaylist* playlist);

tMusica* retornaMusicaProx(tMusica* m);

char* retornaNomeMusica(tMusica* m);

char* retornaArtista(tMusica* m);

int buscaMusicaPlaylist (tPlaylist* playlist, tMusica* m);

void destroiPlaylist(tPlaylist* playlist);

tMusica* desencadeiaMusica(tMusica* m);

void destroiMusicas(tMusica* m);