#include <stdio.h>
#include <stdlib.h>

typedef struct {
  int elementX;
  int elementY;
} Relation;

typedef struct {
  int index;
  Relation* closureRelations;
} ClosureParams;

int isReflexive(int numElements, int* setA, int numRelations, Relation* relations, ClosureParams* reflexiveClosure);
int isIrreflexive(int numElements, int* setA, int numRelations, Relation* relations);
int isSymmetric(int numRelations, Relation* relations, ClosureParams* symmetricClosure);
int isAntiSymmetric(int numRelations, Relation* relations);
int isTransitive(int numRelations, Relation* relations);
void prepareClosure(ClosureParams* closure, Relation* relations, int numRelations);
void findTransitiveClosure(int numRelations, Relation* relations, ClosureParams* transitiveClosure);
Relation* getRelations(int* numRelations);

int main() {
  Relation *relations = NULL;
  ClosureParams symmetricClosure;
  ClosureParams reflexiveClosure;
  ClosureParams transitiveClosure;
  symmetricClosure.closureRelations = NULL;
  reflexiveClosure.closureRelations = NULL;
  transitiveClosure.closureRelations = NULL;

  int numElements, numRelations = 0;
  scanf("%d", &numElements);

  int *setA = (int*)malloc((numElements) * sizeof(int));

  for (int i = 0; i < numElements; i++)
    scanf("%d", &(setA)[i]);

  relations = getRelations(&numRelations);

  printf("Propriedades\n");
  
  int reflexive = isReflexive(numElements, setA, numRelations, relations, &reflexiveClosure);
  int irreflexive = isIrreflexive(numElements, setA, numRelations, relations);
  int symmetric = isSymmetric(numRelations, relations, &symmetricClosure);
  int antiSymmetric = isAntiSymmetric(numRelations, relations);
  int assymetric = irreflexive && antiSymmetric;

  printf("5. Assimetrica: %c\n", (assymetric ? 'V' : 'F'));

  int transitive = isTransitive(numRelations, relations);
  int equivalence = reflexive && symmetric && transitive;
  int partialOrder = reflexive && antiSymmetric && transitive;

  printf("\nRelacao de equivalencia: %c\n", (equivalence ? 'V' : 'F'));
  printf("Relacao de ordem parcial: %c\n", (partialOrder ? 'V' : 'F'));

  printf("\nFecho reflexivo da relacao:");
  if (reflexive) {
    printf(" R");
  } else {
    printf("\n");
    prepareClosure(&reflexiveClosure, relations, numRelations);

    for (int i = 0; i < reflexiveClosure.index; i++) {
      int elementX = reflexiveClosure.closureRelations[i].elementX;
      int elementY = reflexiveClosure.closureRelations[i].elementY; 

      if (i != 0) {
        printf(", (%d, %d)", elementX, elementY);
      } else {
        printf("(%d, %d)", elementX, elementY);
      }
    }
  }

  printf("\nFecho simetrico da relacao:");
  if (symmetric) {
    printf(" R");
  } else {
    printf("\n");
    prepareClosure(&symmetricClosure, relations, numRelations);

    for (int i = 0; i < symmetricClosure.index; i++) {
      int elementX = symmetricClosure.closureRelations[i].elementX;
      int elementY = symmetricClosure.closureRelations[i].elementY; 

      if (i != 0) {
        printf(", (%d, %d)", elementX, elementY);
      } else {
        printf("(%d, %d)", elementX, elementY);
      }
    }
  }

  printf("\nFecho transitivo da relacao:");
  if (transitive) {
    printf(" R");
  } else {
    printf("\n");
    findTransitiveClosure(numRelations, relations, &transitiveClosure);

    for (int i = 0; i < transitiveClosure.index; i++) {
      int elementX = transitiveClosure.closureRelations[i].elementX;
      int elementY = transitiveClosure.closureRelations[i].elementY; 

      if (i != 0) {
        printf(", (%d, %d)", elementX, elementY);
      } else {
        printf("(%d, %d)", elementX, elementY);
      }
    }
  }

  printf("\n");

  free(setA);
  free(relations);
  free(reflexiveClosure.closureRelations);
  free(symmetricClosure.closureRelations);

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

int organizeRelations(const void *a, const void *b) {
  Relation *relationA = (Relation *)a;
  Relation *relationB = (Relation *)b;

  if (relationA->elementX != relationB->elementX) {
    return relationA->elementX - relationB->elementX;
  } else {
    return relationA->elementY - relationB->elementY;
  }
}

int isReflexive(int numElements, int* setA, int numRelations, Relation* relations, ClosureParams* reflexiveClosure) {
  int reflexive = 1; 
  int relationFound;

  int capacity = 50;
  Relation *reflexiveRelations = (Relation *)malloc(capacity * sizeof(Relation));
  int index = 0;

  for (int i = 0; i < numElements; i++) {
    int element = setA[i];

    relationFound = findRelation(numRelations, relations, element, element);

    if (!relationFound) {
      reflexive = 0;

      if (index >= capacity) {
        capacity *= 2;
        reflexiveRelations = (Relation *)realloc(reflexiveRelations, capacity * sizeof(Relation));
      }

      reflexiveRelations[index].elementX = element;
      reflexiveRelations[index].elementY = element;
      index++;
    }

  }

  if (reflexive) {
    printf("1. Reflexiva: V");
  } else {
    printf("1. Reflexiva: F\n");
    printf("(%d, %d)", 
          reflexiveRelations[0].elementX, 
          reflexiveRelations[0].elementY);

    for (int i = 1; i < index; i++)
      printf(", (%d, %d)", 
            reflexiveRelations[i].elementX, 
            reflexiveRelations[i].elementY);
  
    reflexiveClosure->closureRelations = (Relation *)malloc(capacity * sizeof(Relation));

    for (int i = 0; i < index; i++) {
      reflexiveClosure->closureRelations[i].elementX = reflexiveRelations[i].elementX;
      reflexiveClosure->closureRelations[i].elementY = reflexiveRelations[i].elementY;
    }
    reflexiveClosure->index = index;
  }

  free(reflexiveRelations);

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
        printf("2. Irreflexiva: F\n");
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

int isSymmetric(int numRelations, Relation* relations, ClosureParams* symmetricClosure) {
  int symmetric = 1; 
  int relationFound;

  int capacity = 50;
  Relation *symmetricRelations = (Relation *)malloc(capacity * sizeof(Relation));
  int index = 0;

  for (int i = 0; i < numRelations; i++) {
    int elementX = relations[i].elementX;
    int elementY = relations[i].elementY;

    relationFound = findRelation(numRelations, relations, elementY, elementX);

    if (!relationFound) {
      symmetric = 0;

      if (index >= capacity) {
        capacity *= 2;
        symmetricRelations = (Relation *)realloc(symmetricRelations, capacity * sizeof(Relation));
      }
      
      symmetricRelations[index].elementX = elementY;
      symmetricRelations[index].elementY = elementX;
      index++;
    }
  }

  qsort(symmetricRelations, index, sizeof(Relation), organizeRelations);

  if (symmetric) {
    printf("3. Simetrica: V");
  } else {
    printf("3. Simetrica: F\n");
    printf("(%d, %d)", 
          symmetricRelations[0].elementX, 
          symmetricRelations[0].elementY);

    for (int i = 1; i < index; i++)
      printf(", (%d, %d)", 
            symmetricRelations[i].elementX, 
            symmetricRelations[i].elementY);

    symmetricClosure->closureRelations = (Relation *)malloc(capacity * sizeof(Relation));

    for (int i = 0; i < index; i++) {
      symmetricClosure->closureRelations[i].elementX = symmetricRelations[i].elementX;
      symmetricClosure->closureRelations[i].elementY = symmetricRelations[i].elementY;
    }

    symmetricClosure->index = index;
  }

  free(symmetricRelations);
  
  printf("\n");
  return symmetric;
}

int isAntiSymmetric(int numRelations, Relation* relations) {
  int antisymmetric = 1;
  int relationFound, relationAnalyzed;

  int capacity = 50;
  Relation *antiSymmetricRelations = (Relation*)malloc((capacity) * sizeof(Relation));
  int index = 0;

  for (int j = 0; j < numRelations; j++){
    
    int elementX = relations[j].elementX;
    int elementY = relations[j].elementY;

    if (elementX != elementY) {
      relationFound = findRelation(numRelations, relations, elementY, elementX);
      relationAnalyzed = findRelation(index, antiSymmetricRelations, elementY, elementX);

      if (relationFound && !(relationAnalyzed)) {
        antisymmetric = 0;

        if (index >= capacity) {
          capacity *= 2;
          antiSymmetricRelations = (Relation *)realloc(antiSymmetricRelations, capacity * sizeof(Relation));
        }

        antiSymmetricRelations[index].elementX = elementX;
        antiSymmetricRelations[index].elementY = elementY;
        index++;
      }
    }
  }

  qsort(antiSymmetricRelations, index, sizeof(Relation), organizeRelations);

  if (antisymmetric) {
    printf("4. Anti-simetrica: V");
  } else {
    printf("4. Anti-simetrica: F\n");
    printf("((%d, %d), (%d, %d))", 
          antiSymmetricRelations[0].elementX,
          antiSymmetricRelations[0].elementY,
          antiSymmetricRelations[0].elementY,
          antiSymmetricRelations[0].elementX);

    for (int i = 1; i < index; i++)
      printf(", ((%d, %d), (%d, %d))", 
            antiSymmetricRelations[i].elementX,
            antiSymmetricRelations[i].elementY,
            antiSymmetricRelations[i].elementY,
            antiSymmetricRelations[i].elementX);
  }

  free(antiSymmetricRelations);

  printf("\n");
  return antisymmetric;
}

int isTransitive(int numRelations, Relation* relations) {
  int transitive = 1;
  int relationFound, relationAnalyzed;

  int capacity = 50;
  Relation *transitiveRelations = (Relation*)malloc((capacity) * sizeof(Relation));
  int index = 0;

  for (int i = 0; i < numRelations; i++) {
    for (int j = 0; j < numRelations; j++) {
      if (relations[i].elementY == relations[j].elementX) {
        int elementX = relations[i].elementX;
        int elementY = relations[j].elementY;

        relationFound = findRelation(numRelations, relations, elementX, elementY);
        relationAnalyzed = findRelation(index, transitiveRelations, elementX, elementY);

        if ((!relationFound) && (!relationAnalyzed)) {
          transitive = 0;

          if (index >= capacity) {
            capacity *= 2;
            transitiveRelations = (Relation *)realloc(transitiveRelations, capacity * sizeof(Relation));
          }

          transitiveRelations[index].elementX = elementX;
          transitiveRelations[index].elementY = elementY;
          index++;
        }
      }
    }
  }

  qsort(transitiveRelations, index, sizeof(Relation), organizeRelations);

  if (transitive) {
    printf("6. Transitiva: V");
  } else {
    printf("6. Transitiva: F\n");
    printf("(%d, %d)", transitiveRelations[0].elementX, transitiveRelations[0].elementY);

    for (int i = 1; i < index; i++)
      printf(", (%d, %d)", transitiveRelations[i].elementX, transitiveRelations[i].elementY);
  }
  
  free(transitiveRelations);
  printf("\n");

  return transitive;
}

void findTransitiveClosure(int numRelations, Relation* relations, ClosureParams* transitiveClosure) {
  int capacity = numRelations;
  transitiveClosure->closureRelations = (Relation*)malloc(capacity * sizeof(Relation));
  int index = 0;

  for (int i = 0; i < numRelations; i++) {
    transitiveClosure->closureRelations[index++] = relations[i];
  }

  int addedNewRelations;
  do {
    addedNewRelations = 0;
    for (int i = 0; i < index; i++) {
      for (int j = 0; j < index; j++) {
        if (transitiveClosure->closureRelations[i].elementY == transitiveClosure->closureRelations[j].elementX) {
          int elementX = transitiveClosure->closureRelations[i].elementX;
          int elementY = transitiveClosure->closureRelations[j].elementY;

          if (!findRelation(index, transitiveClosure->closureRelations, elementX, elementY)) {
            if (index >= capacity) {
              capacity *= 2;
              transitiveClosure->closureRelations = (Relation*)realloc(transitiveClosure->closureRelations, capacity * sizeof(Relation));
            }
            transitiveClosure->closureRelations[index].elementX = elementX;
            transitiveClosure->closureRelations[index].elementY = elementY;
            index++;
            addedNewRelations = 1;
          }
        }
      }
    }
  } while (addedNewRelations);

  transitiveClosure->index = index;
  qsort(transitiveClosure->closureRelations, index, sizeof(Relation), organizeRelations);
}

void prepareClosure(ClosureParams* closure, Relation* relations, int numRelations) {
  int index = closure->index;
  int capacity = index + numRelations;
  closure->closureRelations = (Relation *)realloc(closure->closureRelations, capacity * sizeof(Relation));

  for (int i = 0; i < numRelations; i++) {
    closure->closureRelations[index].elementX = relations[i].elementX;
    closure->closureRelations[index].elementY = relations[i].elementY;
    index++;
  }

  qsort(closure->closureRelations, index, sizeof(Relation), organizeRelations);
  closure->index = index;
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
