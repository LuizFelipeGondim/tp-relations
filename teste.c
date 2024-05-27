#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

// Estrutura de relação
typedef struct {
    int elementX;
    int elementY;
} Relation;

// Estrutura para armazenar fechos de relações
typedef struct {
    int index;
    int capacity;
    Relation* closureRelations;
} ClosureParams;

// Funções auxiliares
int findRelation(int index, Relation* relations, int elementX, int elementY);
void addRelation(ClosureParams* closure, int elementX, int elementY);
void prepareClosure(ClosureParams* closure, Relation* relations, int numRelations);
void findTransitiveClosure(int numRelations, Relation* relations, ClosureParams* transitiveClosure);
void printRelations(Relation* relations, int numRelations);
Relation* getRelations(int* numRelations);

// Funções principais
int isReflexive(int numElements, int* setA, int numRelations, Relation* relations, ClosureParams* reflexiveClosure);
int isIrreflexive(int numElements, int* setA, int numRelations, Relation* relations);
int isSymmetric(int numRelations, Relation* relations, ClosureParams* symmetricClosure);
int isAntiSymmetric(int numRelations, Relation* relations);
int isTransitive(int numRelations, Relation* relations);

int main() {
    Relation *relations = NULL;
    ClosureParams symmetricClosure = {0, 50, malloc(50 * sizeof(Relation))};
    ClosureParams reflexiveClosure = {0, 50, malloc(50 * sizeof(Relation))};
    ClosureParams transitiveClosure = {0, 50, malloc(50 * sizeof(Relation))};

    int numElements, numRelations = 0;
    scanf("%d", &numElements);

    int *setA = (int*)malloc(numElements * sizeof(int));
    for (int i = 0; i < numElements; i++)
        scanf("%d", &setA[i]);

    relations = getRelations(&numRelations);

    printf("Propriedades\n");

    int reflexive = isReflexive(numElements, setA, numRelations, relations, &reflexiveClosure);
    int irreflexive = isIrreflexive(numElements, setA, numRelations, relations);
    int symmetric = isSymmetric(numRelations, relations, &symmetricClosure);
    int antiSymmetric = isAntiSymmetric(numRelations, relations);
    int assymetric = irreflexive && antiSymmetric;

    printf("5. Assimetrica: %s\n", (assymetric ? "V" : "F"));

    int transitive = isTransitive(numRelations, relations);
    int equivalence = reflexive && symmetric && transitive;
    int partialOrder = reflexive && antiSymmetric && transitive;

    printf("\nRelacao de equivalencia: %s\n", (equivalence ? "V" : "F"));
    printf("Relacao de ordem parcial: %s\n", (partialOrder ? "V" : "F"));

    printf("\nFecho reflexivo da relacao:");
    if (reflexive) {
        printf(" R");
    } else {
        printf("\n");
        prepareClosure(&reflexiveClosure, relations, numRelations);
        printRelations(reflexiveClosure.closureRelations, reflexiveClosure.index);
    }

    printf("\nFecho simetrico da relacao:");
    if (symmetric) {
        printf(" R");
    } else {
        printf("\n");
        prepareClosure(&symmetricClosure, relations, numRelations);
        printRelations(symmetricClosure.closureRelations, symmetricClosure.index);
    }

    printf("\nFecho transitivo da relacao:");
    findTransitiveClosure(numRelations, relations, &transitiveClosure);
    printRelations(transitiveClosure.closureRelations, transitiveClosure.index);
    printf("\n");

    free(setA);
    free(relations);
    free(reflexiveClosure.closureRelations);
    free(symmetricClosure.closureRelations);
    free(transitiveClosure.closureRelations);

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

void addRelation(ClosureParams* closure, int elementX, int elementY) {
    if (closure->index >= closure->capacity) {
        closure->capacity *= 2;
        closure->closureRelations = realloc(closure->closureRelations, closure->capacity * sizeof(Relation));
    }
    closure->closureRelations[closure->index].elementX = elementX;
    closure->closureRelations[closure->index].elementY = elementY;
    closure->index++;
}

void prepareClosure(ClosureParams* closure, Relation* relations, int numRelations) {
    for (int i = 0; i < numRelations; i++) {
        addRelation(closure, relations[i].elementX, relations[i].elementY);
    }
    qsort(closure->closureRelations, closure->index, sizeof(Relation), (int (*)(const void*, const void*))findRelation);
}

void findTransitiveClosure(int numRelations, Relation* relations, ClosureParams* transitiveClosure) {
    for (int i = 0; i < numRelations; i++) {
        addRelation(transitiveClosure, relations[i].elementX, relations[i].elementY);
    }

    int addedNewRelations = 1;
    while (addedNewRelations) {
        addedNewRelations = 0;
        for (int i = 0; i < transitiveClosure->index; i++) {
            for (int j = 0; j < transitiveClosure->index; j++) {
                if (transitiveClosure->closureRelations[i].elementY == transitiveClosure->closureRelations[j].elementX) {
                    int elementX = transitiveClosure->closureRelations[i].elementX;
                    int elementY = transitiveClosure->closureRelations[j].elementY;
                    if (!findRelation(transitiveClosure->index, transitiveClosure->closureRelations, elementX, elementY)) {
                        addRelation(transitiveClosure, elementX, elementY);
                        addedNewRelations = 1;
                    }
                }
            }
        }
    }
    qsort(transitiveClosure->closureRelations, transitiveClosure->index, sizeof(Relation), (int (*)(const void*, const void*))findRelation);
}

void printRelations(Relation* relations, int numRelations) {
    for (int i = 0; i < numRelations; i++) {
        if (i != 0) {
            printf(", ");
        }
        printf("(%d, %d)", relations[i].elementX, relations[i].elementY);
    }
}

Relation* getRelations(int* numRelations) {
    Relation *relations = NULL;
    int elementX, elementY;
    int capacity = 10;
    relations = malloc(capacity * sizeof(Relation));

    while (scanf("%d %d", &elementX, &elementY) != EOF) {
        if (*numRelations >= capacity) {
            capacity *= 2;
            relations = realloc(relations, capacity * sizeof(Relation));
        }
        relations[*numRelations].elementX = elementX;
        relations[*numRelations].elementY = elementY;
        (*numRelations)++;
    }

    return relations;
}

int isReflexive(int numElements, int* setA, int numRelations, Relation* relations, ClosureParams* reflexiveClosure) {
    bool reflexive = true;
    for (int i = 0; i < numElements; i++) {
        int element = setA[i];
        if (!findRelation(numRelations, relations, element, element)) {
            reflexive = false;
            addRelation(reflexiveClosure, element, element);
        }
    }

    printf("1. Reflexiva: %s\n", reflexive ? "V" : "F");
    return reflexive;
}

int isIrreflexive(int numElements, int* setA, int numRelations, Relation* relations) {
    bool irreflexive = true;
    for (int i = 0; i < numElements; i++) {
        int element = setA[i];
        if (findRelation(numRelations, relations, element, element)) {
            irreflexive = false;
            break;
        }
    }

    printf("2. Irreflexiva: %s\n", irreflexive ? "V" : "F");
    return irreflexive;
}

int isSymmetric(int numRelations, Relation* relations, ClosureParams* symmetricClosure) {
    bool symmetric = true;
    for (int i = 0; i < numRelations; i++) {
        int elementX = relations[i].elementX;
        int elementY = relations[i].elementY;
        if (elementX != elementY && !findRelation(numRelations, relations, elementY, elementX)) {
            symmetric = false;
            addRelation(symmetricClosure, elementX, elementY);
        }
    }

    printf("3. Simetrica: %s\n", symmetric ? "V" : "F");
    return symmetric;
}

int isAntiSymmetric(int numRelations, Relation* relations) {
    bool antiSymmetric = true;
    for (int i = 0; i < numRelations; i++) {
        int elementX = relations[i].elementX;
        int elementY = relations[i].elementY;
        if (elementX != elementY && findRelation(numRelations, relations, elementY, elementX)) {
            antiSymmetric = false;
            break;
        }
    }

    printf("4. Anti-simetrica: %s\n", antiSymmetric ? "V" : "F");
    return antiSymmetric;
}

int isTransitive(int numRelations, Relation* relations) {
    bool transitive = true;
    for (int i = 0; i < numRelations; i++) {
        for (int j = 0; j < numRelations; j++) {
            if (relations[i].elementY == relations[j].elementX) {
                int elementX = relations[i].elementX;
                int elementY = relations[j].elementY;
                if (!findRelation(numRelations, relations, elementX, elementY)) {
                    transitive = false;
                    break;
                }
            }
        }
        if (!transitive) {
            break;
        }
    }

    printf("6. Transitiva: %s\n", transitive ? "V" : "F");
    return transitive;
}
