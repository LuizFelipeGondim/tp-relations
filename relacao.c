#include <stdio.h>
#include <stdlib.h>

typedef struct {
  int elementX;
  int elementY;
} Relation;

int isReflexive(int numElements, int* setA, int numRelations, Relation* relations);

Relation* getRelations(int* numRelations);

int main() {
  Relation *relations = NULL;
  int reflexive = 0;
  int numElements, numRelations = 0;

  scanf("%d", &numElements);

  int *setA = (int*)malloc((numElements) * sizeof(int));

  for (int i = 0; i < numElements; i++)
    scanf("%d", &(setA)[i]);

  relations = getRelations(&numRelations);

  reflexive = isReflexive(numElements, setA, numRelations, relations);
  
  /*
  printf("Conjunto A:\n");
  for (int i = 0; i < numElements; i++) {
    printf("%d ", setA[i]);
  }

  printf("\nRelações:\n");
  for (int i = 0; i < 4; i++) {
    printf("%d %d\n", relations[i].elementX, relations[i].elementY);
  }*/

  free(setA);
  free(relations);

  return 0;
}

int isReflexive(int numElements, int* setA, int numRelations, Relation* relations) {
  int reflexive = 1; 

  for (int i = 0; i < numElements; i++) {
    int element = setA[i];
    int relationFound = 0;

    for (int j = 0; j < numRelations; j++) {
      if (relations[j].elementX == element && relations[j].elementY == element) {
        relationFound = 1;
        break;
      }
    }

    if (!relationFound) {
      if (reflexive) {
        printf("1. Reflexiva: F \n");
        printf("(%d, %d)", element, element);
        reflexive = 0;
      } else {
        printf(", (%d, %d)", element, element);
      }
    }
  }

  if (reflexive)
    printf("1. Reflexiva: T");

  printf("\n");
  return reflexive;
}

int isIrreflexive() {
  return 1;
}

int isSymmetric() {
  return 1;
}

int isAntisymmetric() {
  return 1;
}

int isTransitive() {
  return 1;
}

Relation* getRelations(int* numRelations) {

  Relation *aux = NULL;
  int elementX, elementY;

  while (scanf("%d %d", &elementX, &elementY) != EOF) {
    (*numRelations)++;

    aux = (Relation*)realloc(aux, (*numRelations) * sizeof(Relation));
    aux[(*numRelations) - 1].elementX = elementX;
    aux[(*numRelations) - 1].elementY = elementY;
  }

  return aux;
}
