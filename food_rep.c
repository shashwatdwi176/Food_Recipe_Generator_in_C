#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

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

void generateRecipesDFS(const RecipeGraph* graph, int currentRecipe, bool* visited) {
    visited[currentRecipe] = true;

    printf("Recipe: %s\n", graph->recipes[currentRecipe].name);
    printf("Steps:\n");
    for (int i = 0; i < graph->recipes[currentRecipe].numSteps; i++) {
        printf("%d. %s\n", i + 1, graph->recipes[currentRecipe].steps[i]);
    }
    printf("\n");

    for (int i = 0; i < graph->numVertices; i++) {
        if (graph->adjacencyMatrix[currentRecipe][i] && !visited[i]) {
            generateRecipesDFS(graph, i, visited);
        }
    }
}

void generateRecipes(const RecipeGraph* graph) {
    bool visited[MAX_RECIPES];
    for (int i = 0; i < graph->numVertices; i++) {
        visited[i] = false;
    }

    printf("Generated Recipes:\n");
    for (int i = 0; i < graph->numVertices; i++) {
        if (!visited[i]) {
            generateRecipesDFS(graph, i, visited);
        }
    }
}

int main() {
    RecipeGraph graph = createRecipeGraph();

    Recipe pizzaRecipe = {
        .name = "Pizza",
        .steps = {
            "Prepare the dough",
            "Add sauce and toppings",
            "Bake in the oven",
            "Slice and serve"
        },
        .numSteps = 4
    };

    Recipe pastaRecipe = {
        .name = "Pasta",
        .steps = {
            "Boil water",
            "Add pasta and cook",
            "Drain the pasta",
            "Add sauce and serve"
        },
        .numSteps = 4
    };

    Recipe saladRecipe = {
        .name = "Salad",
        .steps = {
            "Wash and chop the vegetables",
            "Mix the ingredients",
            "Add dressing",
            "Serve"
        },
        .numSteps = 4
    };

    addRecipe(&graph, &pizzaRecipe);
    addRecipe(&graph, &pastaRecipe);
    addRecipe(&graph, &saladRecipe);

    addDependency(&graph, 0, 1); // Pizza depends on Pasta
    addDependency(&graph, 0, 2); // Pizza depends on Salad

    generateRecipes(&graph);

    return 0;
}
