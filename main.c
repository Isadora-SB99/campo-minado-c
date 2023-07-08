#include "campoMinado.h"

int main(void) {

  while(comecar!= 's'){
    introducao();
  }
  
  inicializaCampo();
  colocaMinas();
  // imprimeCampoVisivel();
  while (parar != 'S') {
    jogar();
  }

  return 0;
}