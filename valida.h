#include <stdbool.h>

bool validaLinha(int linha, int tamanho) {
  if (linha >= 0 && linha < tamanho) {
    return true;
  } else {
    return false;
  }
}

bool validaColuna(int coluna, int tamanho) {
  if (coluna >= 0 && coluna < tamanho) {
    return true;
  } else {
    return false;
  }
}