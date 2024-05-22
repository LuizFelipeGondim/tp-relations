#include <stdio.h>
#include <stdlib.h>

typedef struct {
  int elementX;
  int elementY;
} Relation;

int isReflexive(int numElements, int* setA, int numRelations, Relation* relations);

int isIrreflexive(int numElements, int* setA, int numRelations, Relation* relations);

int isSymmetric(int numRelations, Relation* relations);

int isAntiSymmetric(int numElements, int* setA, int numRelations, Relation* relations);

int isTransitive(int numRelations, Relation* relations);

Relation* getRelations(int* numRelations);

int main() {
  Relation *relations = NULL;
  int reflexive, irreflexive, symmetric;
  int antiSymmetric, assymetric, transitive;
  int equivalence, partialOrder; 
  int numElements, numRelations = 0;

  scanf("%d", &numElements);

  int *setA = (int*)malloc((numElements) * sizeof(int));

  for (int i = 0; i < numElements; i++)
    scanf("%d", &(setA)[i]);

  relations = getRelations(&numRelations);

  printf("Propriedades\n");
  
  reflexive = isReflexive(numElements, setA, numRelations, relations);
  
  irreflexive = isIrreflexive(numElements, setA, numRelations, relations);

  symmetric = isSymmetric(numRelations, relations);

  antiSymmetric = isAntiSymmetric(numElements, setA, numRelations, relations);

  assymetric = irreflexive && antiSymmetric;

  if (assymetric) {
    printf("5. Assimetrica: V \n");
  } else {
    printf("5. Assimetrica: F \n");
  }

  transitive = isTransitive(numRelations, relations);

  equivalence = reflexive && symmetric && transitive;
  partialOrder = reflexive && antiSymmetric && transitive;

  printf("\nRelacao de equivalencia: %s\n", (equivalence ? "V" : "F"));
  printf("Relacao de ordem parcial: %s\n\n", (partialOrder ? "V" : "F"));
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

int findRelation(int index, Relation* relations, int elementX, int elementY) {
  for (int i = 0; i < index; i++) {
    if (relations[i].elementX == elementX && relations[i].elementY == elementY) {
      return 1;
    }
  }

  return 0;
}

int isReflexive(int numElements, int* setA, int numRelations, Relation* relations) {
  int reflexive = 1; 
  int relationFound;

  for (int i = 0; i < numElements; i++) {
    int element = setA[i];

    relationFound = findRelation(numRelations, relations, element, element);

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
    printf("1. Reflexiva: V");

  printf("\n");
  return reflexive;
}

int isIrreflexive(int numElements, int* setA, int numRelations, Relation* relations) {
  int irreflexive = 1; 
  int relationFound;

  for (int i = 0; i < numElements; i++) {
    int element = setA[i];

    relationFound = findRelation(numRelations, relations, element, element);

    if (relationFound) {
      if (irreflexive) {
        printf("2. Irreflexiva: F \n");
        printf("(%d, %d)", element, element);
        irreflexive = 0;
      } else {
        printf(", (%d, %d)", element, element);
      }
    }
  }

  if (irreflexive)
    printf("2. Irreflexiva: V");

  printf("\n");
  return irreflexive;
}

int isSymmetric(int numRelations, Relation* relations) {
  int symmetric = 1; 
  int relationFound;

  for (int i = 0; i < numRelations; i++) {
    int elementX = relations[i].elementX;
    int elementY = relations[i].elementY;

    relationFound = findRelation(numRelations, relations, elementY, elementX);

    if (!relationFound) {
      if (symmetric) {
        printf("3. Simetrica: F \n");
        printf("(%d, %d)", elementY, elementX);
        symmetric = 0;
      } else {
        printf(", (%d, %d)", elementY, elementX);
      }
    }
  }

  if (symmetric)
    printf("3. Simetrica: V");

  printf("\n");
  return symmetric;
}

int isAntiSymmetric(int numElements, int* setA, int numRelations, Relation* relations) {
  int antisymmetric = 1;
  int relationFound, relationAnalyzed;
  Relation *antiSymmetricRelations = (Relation*)malloc((numRelations) * sizeof(Relation));
  int index = 0;

  for (int i = 0; i < numElements; i++) {

    for (int j = 0; j < numRelations; j++){
      if (setA[i] != relations[j].elementX)
        continue;
      
      int elementX = relations[j].elementX;
      int elementY = relations[j].elementY;

      if (elementX != elementY) {
        relationFound = findRelation(numRelations, relations, elementY, elementX);
        relationAnalyzed = findRelation(index, antiSymmetricRelations, elementY, elementX);

        if (relationFound && !(relationAnalyzed)) {
          if (antisymmetric) {
            printf("4. Anti-simetrica: F \n");
            printf("((%d, %d), (%d, %d))", elementX, elementY, elementY, elementX);
            antisymmetric = 0;
          } else {
            printf(", ((%d, %d), (%d, %d))", elementX, elementY, elementY, elementX);
          }

          antiSymmetricRelations[index].elementX = elementX;
          antiSymmetricRelations[index].elementY = elementY;
          antiSymmetricRelations[index + 1].elementX = elementX;
          antiSymmetricRelations[index + 1].elementY = elementY;
          index += 2;
        }
      }
    }
  }

  free(antiSymmetricRelations);

  if (antisymmetric)
    printf("4. Anti-simetrica: V");

  printf("\n");
  return antisymmetric;
}

int isTransitive(int numRelations, Relation* relations) {
  int transitive = 1;
  int relationFound, relationAnalyzed;
  Relation *transitiveRelations = (Relation*)malloc((numRelations) * sizeof(Relation));
  int index = 0;

  for (int i = 0; i < numRelations; i++) {
    for (int j = 0; j < numRelations; j++) {
      if (relations[i].elementY == relations[j].elementX) {
        int elementX = relations[i].elementX;
        int elementY = relations[j].elementY;

        relationFound = findRelation(numRelations, relations, elementX, elementY);
        relationAnalyzed = findRelation(index, transitiveRelations, elementX, elementY);

        if ((!relationFound) && (!relationAnalyzed)) {
          if (transitive) {
            printf("6. Transitiva: F \n");
            printf("(%d, %d)", elementX, elementY);
            transitive = 0;
          } else {
            printf(", (%d, %d)", elementX, elementY);
          }

          transitiveRelations[index].elementX = elementX;
          transitiveRelations[index].elementY = elementY;
          index++;
        }
      }
    }
  }

  free(transitiveRelations);

  if (transitive)
    printf("6. Transitiva: V");

  printf("\n");
  
  return transitive;
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
