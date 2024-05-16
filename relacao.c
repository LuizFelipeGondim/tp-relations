#include <stdio.h>
#include <stdlib.h>

// Estrutura para armazenar as relações
typedef struct {
  int elementX;
  int elementY;
} Relation;

Relation* getRelations();

int main() {
  Relation *relations = NULL;

  int tam;
  scanf("%d", &tam);

  int *setA = (int*)malloc((tam) * sizeof(int));

  for (int i = 0; i < tam; i++) {
    scanf("%d", &(setA)[i]);
  }

  relations = getRelations();

  printf("Conjunto A:\n");
  for (int i = 0; i < tam; i++) {
    printf("%d ", setA[i]);
  }

  printf("\nRelações:\n");
  for (int i = 0; i < 4; i++) {
    printf("%d %d\n", relations[i].elementX, relations[i].elementY);
  }

  // Liberar memória alocada
  free(setA);
  free(relations);

  return 0;
}

Relation* getRelations() {
  // Ler as relações até que um valor inválido seja inserido
  Relation *aux = NULL;
  int elementX, elementY;
  int numRelations = 0;


  while (scanf("%d %d", &elementX, &elementY) == 2) {
    numRelations++;

    aux = (Relation*)realloc(aux, numRelations * sizeof(Relation)); // Aloca memória inicial para as relações
    aux[numRelations - 1].elementX = elementX;
    aux[numRelations - 1].elementY = elementY;
  }

  return aux;
}
