#include "valida.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#define TAM 10
#define MINAS 10

char comecar = 'N';
char parar = 'N';
int minasFechadas = MINAS;
int minasMarcadas = 0;
int celulasVaziasAbertas = 0;
int contadorMinasProximas = 0;

int campo[TAM][TAM];

void abreAdjacentesSemMina(int lin, int col);
void abrirCelula(int lin, int col);
void pedirPosicaoCelula(char opcao, char *palavra);
int contadorMinas(int lin, int col);

void introducao() {
  printf("\n * CAMPO MINADO * \n\nVocê deverá "
         "selecionar uma opção (abrir, marcar ou desmarcar) \ne depois informar "
         "as coordenadas (linha e coluna) onde deseja realizar a ação.\nO jogo "
         "imprime: '-' para células fechadas\n'>' para células marcadas e "
         "'*' para minas abertas.\nQuando uma célula tiver minas em sua "
         "vizinhança, ela imprimirá a quantidade de minas que possui ao "
         "redor\nBoa sorte!\n\n\nDigite 's' para continuar: ");
  scanf("%c", &comecar);
  getchar();
}

void inicializaCampo() {
  for (int linhas = 0; linhas < TAM; linhas++) {
    for (int colunas = 0; colunas < TAM; colunas++) {
      campo[linhas][colunas] = 0;
    }
  }
}

void imprimeCampoVisivel() {
  printf("\n");

  for (int linhas = 0; linhas < TAM; linhas++) {
    for (int colunas = 0; colunas < TAM; colunas++) {
      printf(" %d ", campo[linhas][colunas]);
    }
    printf("\n");
  }
}

void imprimeCampoSecreto() {
  printf("\n");
  for (int linhas = 0; linhas < TAM; linhas++) {
    for (int colunas = 0; colunas < TAM; colunas++) {
      int vizinhos = contadorMinas(linhas, colunas);
      if (campo[linhas][colunas] == 0 || campo[linhas][colunas] == 5) {
        printf(" - ");
      } else if (campo[linhas][colunas] == 1 || campo[linhas][colunas] == 2) {
        printf(" > ");
      } else if (campo[linhas][colunas] == 3) {
        printf(" * ");
      } else if (campo[linhas][colunas] == 4) {
        printf(" %d ", vizinhos);
      }
    }
    printf("\n");
  }
}

void imprimeCampoFinal() {
  printf("\n");
  for (int linhas = 0; linhas < TAM; linhas++) {
    for (int colunas = 0; colunas < TAM; colunas++) {
      int vizinhos = contadorMinas(linhas, colunas);
      if (campo[linhas][colunas] == 0) {
        printf(" - ");
      } else if (campo[linhas][colunas] == 1) {
        printf(" x ");
      } else if (campo[linhas][colunas] == 2) {
        printf(" > ");
      } else if (campo[linhas][colunas] == 3 || campo[linhas][colunas] == 5) {
        printf(" * ");
      } else if (campo[linhas][colunas] == 4) {
        printf(" %d ", vizinhos);
      }
    }
    printf("\n");
  }
}

void confereSeGanhou() {
  if (celulasVaziasAbertas == (TAM * TAM - (minasFechadas + minasMarcadas)) ||
      minasMarcadas == MINAS) {
    printf("\n\n*** VENCEU ***\n\n");
    parar = 'S';
    imprimeCampoFinal();
  }
}

void colocaMinas() {
  srand(time(NULL));
  for (int i = 0; i < MINAS; i++) {
    int linha = rand() % TAM;
    int coluna = rand() % TAM;

    if (campo[linha][coluna] == 0) {
      campo[linha][coluna] = 5;
    } else {
      i--;
    }
  }
}

void marcarCelula(int lin, int col) {
  if (campo[lin][col] == 0) {
    campo[lin][col] = 1;
  } else if (campo[lin][col] == 5) {
    campo[lin][col] = 2;
    minasMarcadas++;
    minasFechadas--;
    confereSeGanhou();
  } else {
    printf("\nOpção inválida");
    pedirPosicaoCelula('m', "marcar");
  }
}

void desmarcarCelula(int lin, int col) {
  if (campo[lin][col] == 1) {
    campo[lin][col] = 0;
  } else if (campo[lin][col] == 2) {
    campo[lin][col] = 5;
    minasMarcadas--;
    minasFechadas++;
  } else {
    printf("\nOpção inválida");
    pedirPosicaoCelula('d', "desmarcar");
  }
}

void abrirCelula(int lin, int col) {
  bool linhaValida = validaLinha(lin, TAM);
  bool colunaValida = validaColuna(col, TAM);

  if (campo[lin][col] == 5) {
    campo[lin][col] = 3;
    printf("\n!!! KABOM !!!\n");
    parar = 'S';
    imprimeCampoFinal();

  } else if (campo[lin][col] == 0 && linhaValida && colunaValida) {
    campo[lin][col] = 4;
    celulasVaziasAbertas++;
    abreAdjacentesSemMina(lin, col);
    confereSeGanhou();
  } else {
    printf("\nOpção inválida");
    pedirPosicaoCelula('a', "abrir");
  }
}

int contadorMinas(int lin, int col) {
  int quantMinas = 0;
  bool linMenosUm = validaLinha(lin - 1, TAM);
  bool linMaisUm = validaLinha(lin + 1, TAM);
  bool colMenosUm = validaLinha(col - 1, TAM);
  bool colMaisUm = validaLinha(col + 1, TAM);

  if (linMenosUm && colMenosUm &&
      (campo[lin - 1][col - 1] == 5 || campo[lin - 1][col - 1] == 2)) {
    quantMinas++;
  }
  if (linMenosUm && (campo[lin - 1][col] == 5 || campo[lin - 1][col] == 2)) {
    quantMinas++;
  }
  if (linMenosUm && colMaisUm &&
      (campo[lin - 1][col + 1] == 5 || campo[lin - 1][col + 1] == 2)) {
    quantMinas++;
  }
  if (colMenosUm && (campo[lin][col - 1] == 5 || campo[lin][col - 1] == 2)) {
    quantMinas++;
  }
  if (colMaisUm && (campo[lin][col + 1] == 5 || campo[lin][col + 1] == 2)) {
    quantMinas++;
  }
  if (linMaisUm && colMenosUm &&
      (campo[lin + 1][col - 1] == 5 || campo[lin + 1][col - 1] == 2)) {
    quantMinas++;
  }
  if (linMaisUm && (campo[lin + 1][col] == 5 || campo[lin + 1][col] == 2)) {
    quantMinas++;
  }
  if (linMaisUm && colMaisUm &&
      (campo[lin + 1][col + 1] == 5 || campo[lin + 1][col + 1] == 2)) {
    quantMinas++;
  }

  return quantMinas;
}

void abreAdjacentesSemMina(int lin, int col) {
  int vizinhos = contadorMinas(lin, col);
  bool linMenosUm = validaLinha(lin - 1, TAM);
  bool linMaisUm = validaLinha(lin + 1, TAM);
  bool colMenosUm = validaLinha(col - 1, TAM);
  bool colMaisUm = validaLinha(col + 1, TAM);

  if (vizinhos == 0) {
    if (linMenosUm && colMenosUm && campo[lin - 1][col - 1] == 0) {
      abrirCelula(lin - 1, col - 1);
    }
    if (linMenosUm && campo[lin - 1][col] == 0) {
      abrirCelula(lin - 1, col);
    }
    if (linMenosUm && colMaisUm && campo[lin - 1][col + 1] == 0) {
      abrirCelula(lin - 1, col + 1);
    }
    if (colMenosUm && campo[lin][col - 1] == 0) {
      abrirCelula(lin, col - 1);
    }
    if (colMaisUm && campo[lin][col + 1] == 0) {
      abrirCelula(lin, col + 1);
    }
    if (linMaisUm && colMenosUm && campo[lin + 1][col - 1] == 0) {
      abrirCelula(lin + 1, col - 1);
    }
    if (linMaisUm && campo[lin + 1][col] == 0) {
      abrirCelula(lin + 1, col);
    }
    if (linMaisUm && colMaisUm && campo[lin + 1][col + 1] == 0) {
      abrirCelula(lin + 1, col + 1);
    }
  }
}

void pedirPosicaoCelula(char opcao, char *palavra) {
  int lin, col;
  bool linhaValida = false;
  bool colunaValida = false;

  while (!linhaValida || !colunaValida) {

    printf("\nInforme as coordenadas entre 0 e %d\n", TAM - 1);
    printf("Informe a linha da célula que deseja %s: ", palavra);
    scanf("%d", &lin);
    getchar();
    printf("Informe a coluna da célula que deseja %s: ", palavra);
    scanf("%d", &col);
    getchar();

    linhaValida = validaLinha(lin, TAM);
    colunaValida = validaColuna(col, TAM);
  }

  if (opcao == 'a') {
    abrirCelula(lin, col);
  } else if (opcao == 'm') {
    marcarCelula(lin, col);
  } else if (opcao == 'd') {
    desmarcarCelula(lin, col);
  }
}

void jogar() {
  char opcao = 'n';
  char *palavra;
  imprimeCampoSecreto();

  while (opcao != 'a' && opcao != 'm' && opcao != 'd') {
    printf("\nDigite:\n'a' para abrir uma célula\n'm' para marcar uma "
           "célula\n'd' para desmarcar uma célula\n");
    scanf("%c", &opcao);
    getchar();
  }

  if (opcao == 'a') {
    palavra = "abrir";
  } else if (opcao == 'm') {
    palavra = "marcar";
  } else if (opcao == 'd') {
    palavra = "desmarcar";
  }

  pedirPosicaoCelula(opcao, palavra);
}