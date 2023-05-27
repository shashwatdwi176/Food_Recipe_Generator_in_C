#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define MAX_RECIPES 10

typedef struct {
    char name[50];
    char steps[10][100];
    int numSteps;
} Recipe;

typedef struct {
    int numVertices;
    bool adjacencyMatrix[MAX_RECIPES][MAX_RECIPES];
    Recipe recipes[MAX_RECIPES];
} RecipeGraph;

RecipeGraph createRecipeGraph() {
    RecipeGraph graph;
    graph.numVertices = 0;
    for (int i = 0; i < MAX_RECIPES; i++) {
        for (int j = 0; j < MAX_RECIPES; j++) {
            graph.adjacencyMatrix[i][j] = false;
        }
    }
    return graph;
}

void addRecipe(RecipeGraph* graph, const Recipe* recipe) {
    if (graph->numVertices < MAX_RECIPES) {
        graph->recipes[graph->numVertices] = *recipe;
        graph->numVertices++;
    }
}

void addDependency(RecipeGraph* graph, int recipeIndex, int dependencyIndex) {
    if (recipeIndex >= 0 && recipeIndex < graph->numVertices &&
        dependencyIndex >= 0 && dependencyIndex < graph->numVertices) {
        graph->adjacencyMatrix[recipeIndex][dependencyIndex] = true;
    }
}

void generateRecipeHelper(const RecipeGraph* graph, int recipeIndex, bool* visited) {
    visited[recipeIndex] = true;
    printf("Recipe: %s\n", graph->recipes[recipeIndex].name);
    printf("Steps:\n");
    for (int i = 0; i < graph->recipes[recipeIndex].numSteps; i++) {
        printf("%d. %s\n", i + 1, graph->recipes[recipeIndex].steps[i]);
    }
    printf("\n");

    for (int i = 0; i < graph->numVertices; i++) {
        if (graph->adjacencyMatrix[recipeIndex][i] && !visited[i]) {
            generateRecipeHelper(graph, i, visited);
        }
    }
}

void generateRecipe(const RecipeGraph* graph, const char* recipeName) {
    int recipeIndex = -1;
    for (int i = 0; i < graph->numVertices; i++) {
        if (strcmp(graph->recipes[i].name, recipeName) == 0) {
            recipeIndex = i;
            break;
        }
    }

    if (recipeIndex == -1) {
        printf("Recipe '%s' not found.\n", recipeName);
        return;
    }

    bool visited[MAX_RECIPES] = { false };
    generateRecipeHelper(graph, recipeIndex, visited);
}

void printMenu() {
    printf("\nMenu:\n");
    printf("1. Add Recipe\n");
    printf("2. Add Dependency\n");
    printf("3. Generate Recipe\n");
    printf("4. Exit\n");
}

int main() {
    RecipeGraph graph = createRecipeGraph();
    int choice;

    do {
        printMenu();
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1: {
                if (graph.numVertices >= MAX_RECIPES) {
                    printf("Maximum number of recipes reached.\n");
                } else {
                    Recipe newRecipe;
                    printf("Enter recipe name: ");
                    scanf(" %[^\n]", newRecipe.name);
                    printf("Enter number of steps: ");
                    scanf("%d", &newRecipe.numSteps);
                    printf("Enter the steps:\n");
                    for (int i = 0; i < newRecipe.numSteps; i++) {
                        printf("Step %d: ", i + 1);
                        scanf(" %[^\n]", newRecipe.steps[i]);
                    }
                    addRecipe(&graph, &newRecipe);
                    printf("Recipe added successfully.\n");
                }
                break;
            }

            case 2: {
                if (graph.numVertices < 2) {
                    printf("Not enough recipes to add dependencies.\n");
                } else {
                    int recipeIndex, dependencyIndex;
                    printf("Enter recipe index: ");
                    scanf("%d", &recipeIndex);
                    printf("Enter dependency index: ");
                    scanf("%d", &dependencyIndex);

                    if (recipeIndex < 0 || recipeIndex >= graph.numVertices ||
                        dependencyIndex < 0 || dependencyIndex >= graph.numVertices) {
                        printf("Invalid recipe or dependency index.\n");
                    } else {
                        addDependency(&graph, recipeIndex, dependencyIndex);
                        printf("Dependency added successfully.\n");
                    }
                }
                break;
            }

            case 3: {
                if (graph.numVertices < 1) {
                    printf("No recipes added.\n");
                } else {
                    char recipeName[50];
                    printf("Enter recipe name: ");
                    scanf(" %[^\n]", recipeName);
                    generateRecipe(&graph, recipeName);
                }
                break;
            }

            case 4:
                printf("Exiting program.\n");
                break;

            default:
                printf("Invalid choice. Please try again.\n");
                break;
        }
    } while (choice != 4);

    return 0;
}
