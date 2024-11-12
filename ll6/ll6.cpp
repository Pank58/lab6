#define _CRT_SECURE_NO_WARNINGS

#include "stdio.h"
#include "stdlib.h"
#include "cstdlib"
#include <windows.h>
#include <locale.h>

struct node {
	int vertex;
	struct node* next;
};

struct Graph {
	int numVertices;
	struct node** adjLists;
};

struct node* createNode(int v) {
	struct node* newNode = (node*)malloc(sizeof(struct node));
	newNode->vertex = v;
	newNode->next = NULL;
	return newNode;
}

struct Graph* createAGraph(int vertices) {
	struct Graph* graph = (Graph*)malloc(sizeof(struct Graph));
	graph->numVertices = vertices;

	graph->adjLists = (node**)malloc(vertices * sizeof(struct node*));

	for (int i = 0; i < vertices; i++) {
		graph->adjLists[i] = NULL;
	}

	return graph;
}


void addEdge(struct Graph* graph, int s, int d) {
	struct node* temp = graph->adjLists[s];

	while (temp) {
		if (temp->vertex == d) {
			return;
		}
		temp = temp->next;
	}

	struct node* newNode = createNode(d);

	if (graph->adjLists[s] == NULL || graph->adjLists[s]->vertex > d) {
		newNode->next = graph->adjLists[s];
		graph->adjLists[s] = newNode;
	}
	else {
		temp = graph->adjLists[s];
		while (temp->next && temp->next->vertex < d) {
			temp = temp->next;
		}
		newNode->next = temp->next;
		temp->next = newNode;
	}

	temp = graph->adjLists[d];
	while (temp) {
		if (temp->vertex == s) {
			return;
		}
		temp = temp->next;
	}

	newNode = createNode(s);
	if (graph->adjLists[d] == NULL || graph->adjLists[d]->vertex > s) {
		newNode->next = graph->adjLists[d];
		graph->adjLists[d] = newNode;
	}
	else {
		temp = graph->adjLists[d];
		while (temp->next && temp->next->vertex < s) {
			temp = temp->next;
		}
		newNode->next = temp->next;
		temp->next = newNode;
	}
}

struct Graph* edges(int** G, int size) {
	struct Graph* graph = createAGraph(size);

	for (int i = 0; i < size; i++) {
		for (int j = i; j < size; j++) {
			if (G[i][j] == 1) {
				addEdge(graph, i, j);
			}
		}
	}
	return graph;
}

void printGraph(struct Graph* graph) {
	for (int v = 0; v < graph->numVertices; v++) {
		struct node* temp = graph->adjLists[v];
		printf("\nVertex %d: ", v + 1);
		while (temp) {
			printf("%d -> ", temp->vertex + 1);
			temp = temp->next;
		}
		printf("\n");
	}
	printf("\n");
}



int** createG(int size) {
	int** G = NULL;
	G = (int**)malloc(size * sizeof(int*));
	for (int i = 0; i < size; i++) {
		G[i] = (int*)malloc(size * sizeof(int));
	}
	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++) {
			G[i][j] = rand() % 2;
			if (i == j) G[i][j] = 0;
			G[j][i] = G[i][j];
		}
	}
	return G;
}

void printG(int** G, int size) {
	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++) {
			printf("%d ", G[i][j]);
		}
		printf("\n");
	}
	printf("\n");
	return;
}


void sortList(struct node* head) {
	if (!head) return;

	int swapped;
	struct node* ptr1;
	struct node* lptr = NULL;

	do {
		swapped = 0;
		ptr1 = head;

		while (ptr1->next != lptr) {
			if (ptr1->vertex > ptr1->next->vertex) {
				int temp = ptr1->vertex;
				ptr1->vertex = ptr1->next->vertex;
				ptr1->next->vertex = temp;
				swapped = 1;
			}
			ptr1 = ptr1->next;
		}
		lptr = ptr1;
	} while (swapped);
}



void unionV(struct Graph* graph, int v1, int v2) {
	if (v1 == v2) return;

	struct node* temp = graph->adjLists[v2];
	while (temp) {
		addEdge(graph, v1, temp->vertex);
		temp = temp->next;
	}

	for (int i = 0; i < graph->numVertices; i++) {
		struct node* adj = graph->adjLists[i];
		struct node* p = NULL;

		while (adj) {
			if (adj->vertex == v2) {
				if (p) {
					p->next = adj->next;
				}
				else {
					graph->adjLists[i] = adj->next;
				}
				struct node* del = adj;
				adj = adj->next;
				free(del);
			}
			else {
				p = adj;
				adj = adj->next;
			}
		}
	}

	graph->adjLists[v2] = NULL;

	for (int i = v2 + 1; i < graph->numVertices; i++) {
		graph->adjLists[i - 1] = graph->adjLists[i];
	}
	graph->adjLists[graph->numVertices - 1] = NULL;

	graph->numVertices--;

	for (int i = 0; i < graph->numVertices; i++) {
		struct node* adj = graph->adjLists[i];
		while (adj) {
			if (adj->vertex > v2) {
				adj->vertex--;
			}
			adj = adj->next;
		}
	}
}

void styag(struct Graph* graph, int v1, int v2) {
	if (v1 == v2) return;

	int flag = 0;
	struct node* temp = graph->adjLists[v1];
	while (temp) {
		if (temp->vertex == v2) {
			flag = 1;
		}
		temp = temp->next;
	}

	if (flag) {
		struct node* adj = graph->adjLists[v1];
		struct node* p = NULL;

		while (adj) {
			if (adj->vertex == v2) {
				if (p) {
					p->next = adj->next;
				}
				else {
					graph->adjLists[v1] = adj->next;
				}
				struct node* del = adj;
				adj = adj->next;
				free(del);
				break;
			}
			else {
				p = adj;
				adj = adj->next;
			}
		}

		adj = graph->adjLists[v2];
		p = NULL;

		while (adj) {
			if (adj->vertex == v1) {
				if (p) {
					p->next = adj->next;
				}
				else {
					graph->adjLists[v2] = adj->next;
				}
				struct node* del = adj;
				adj = adj->next;
				free(del);
				break;
			}
			else {
				p = adj;
				adj = adj->next;
			}
		}

		struct node* temp = graph->adjLists[v2];
		while (temp) {
			addEdge(graph, v1, temp->vertex);
			temp = temp->next;
		}

		for (int i = 0; i < graph->numVertices; i++) {
			adj = graph->adjLists[i];
			p = NULL;

			while (adj) {
				if (adj->vertex == v2) {
					if (p) {
						p->next = adj->next;
					}
					else {
						graph->adjLists[i] = adj->next;
					}
					struct node* del = adj;
					adj = adj->next;
					free(del);
				}
				else {
					p = adj;
					adj = adj->next;
				}
			}
		}

		graph->adjLists[v2] = NULL;

		for (int i = v2 + 1; i < graph->numVertices; i++) {
			graph->adjLists[i - 1] = graph->adjLists[i];
		}
		graph->adjLists[graph->numVertices - 1] = NULL;

		graph->numVertices--;

		for (int i = 0; i < graph->numVertices; i++) {
			struct node* adj = graph->adjLists[i];
			while (adj) {
				if (adj->vertex > v2) {
					adj->vertex--;
				}
				adj = adj->next;
			}
		}
	}
	else { return; }
}


void splitV(struct Graph* graph, int v) {
	if (v < 0 || v >= graph->numVertices) return;

	int newV1 = graph->numVertices;
	int newV2 = graph->numVertices + 1;

	graph->numVertices += 1;
	graph->adjLists = (struct node**)realloc(graph->adjLists, graph->numVertices * sizeof(struct node*));
	graph->adjLists[newV1] = NULL;

	struct node* temp = graph->adjLists[v];
	while (temp) {
		if (temp->vertex != v) {
			addEdge(graph, newV1, temp->vertex);
		}
		temp = temp->next;
	}
}

int main(void) {
	setlocale(LC_ALL, "");
	int sizeG1 = 0, sizeG2 = 0, choice;
	int** G1 = NULL;
	int** G2 = NULL;



	printf("Введите количество вершин 1 графа: ");
	scanf("%d", &sizeG1);
	G1 = createG(sizeG1);
	printf("1 граф\n");
	printG(G1, sizeG1);
	struct Graph* graph1 = edges(G1, sizeG1);
	printGraph(graph1);


	printf("Введите количество вершин 2 графа: ");
	scanf("%d", &sizeG2);
	G2 = createG(sizeG2);
	printf("2 граф\n");
	printG(G2, sizeG2);
	struct Graph* graph2 = edges(G2, sizeG2);
	printGraph(graph2);


	
	while (1) {
		printf("\nВыберите операцию:\n");
		printf("1. Объединение (слияние) вершин\n");
		printf("2. Стягивание вершин\n");
		printf("3. Расщепление вершины\n");
		printf("4. Выход\n");
		scanf("%d", &choice);

		int v1, v2;

		switch (choice) {
		case 1:
			printf("Введите вершины для объединения: ");
			scanf("%d %d", &v1, &v2);
			unionV(graph1, v1 - 1, v2 - 1);
			printGraph(graph1);
			break;
		case 2:
			printf("Введите вершины для стягивания: ");
			scanf("%d %d", &v1, &v2);
			styag(graph1, v1 - 1, v2 - 1);
			printGraph(graph1);
			break;
		case 3:
			printf("Введите вершину для расщепления: ");
			scanf("%d", &v1);
			splitV(graph1, v1 - 1);
			printGraph(graph1);
			break;
		case 4:
			printf("Выход из программы.\n");
			return 0;
		default:
			printf("Неверный выбор. Попробуйте снова.\n");
		}
	}
}

// #define _CRT_SECURE_NO_WARNINGS
//#include <stdio.h>
//#include <stdlib.h>
//#include <stdbool.h>
//#include <time.h>
//#include <windows.h>
//#include <locale.h>
//
//
//int** createG(int size) {
//
//
// int** G = NULL;
// G = (int**)malloc(size * sizeof(int*));
// for (int i = 0; i < size; i++) {
// G[i] = (int*)malloc(size * sizeof(int));
// }
// for (int i = 0; i < size; i++) {
// for (int j = 0; j < size; j++) {
// G[i][j] = 0;
// }
// }
// for (int i = 0; i < size; i++) {
// for (int j = i + 1; j < size; j++) {
// int edge = rand() % 2;
// G[i][j] = edge;
// G[j][i] = edge; // Для неориентированного графа
// }
// }
// return G;
//
//}
//
//
//int** printG(int** G, int size) {
// printf("Матрица смежности для неориентированного графа:\n");
// for (int i = 0; i < size; i++) {
// for (int j = 0; j < size; j++) {
// printf("%d ", G[i][j]);
// }
// printf("\n");
// }
// return G;
//}
//
//
//
//
//int** delv(int** G, int size, int v) {
// int** Gtemp = createG(size - 1);
//
// for (int i = 0; i < size; i++) {
// for (int j = 0; j < size; j++) {
// if (i < v && j < v) Gtemp[i][j] = G[i][j];
// if (i > v && j > v) Gtemp[i - 1][j - 1] = G[i][j];
// if (i > v && j < v) Gtemp[i - 1][j] = G[i][j];
// if (i < v && j > v) Gtemp[i][j - 1] = G[i][j];
//
//
// }
// }
// for (int i = 0; i < size; i++) {
// free(G[i]);
// }
// free(G);
// G = NULL;
// return Gtemp;
//}
//
//int** unionv(int** G, int size, int v1, int v2) {
// for (int i = 0; i < size; i++) {
// if (G[v2][i] == 1) {
// G[v1][i] = G[v2][i];
// G[i][v1] = G[i][v2];
// }
// }
// G = delv(G, size, v2);
// return G;
//
//}
//
//
//
//
//int** contrE(int** G, int size, int v1, int v2) {
// if (G[v1][v2] != 1) {
// printf("нет ребра");
// return G;
//
// }
// else {
// G[v1][v2] = 0;
// G[v2][v1] = 0;
// for (int i = 0; i < size; i++) {
// if (G[v2][i] == 1) {
// G[v1][i] = G[v2][i];
// G[i][v1] = G[i][v2];
// }
// }
// G = delv(G, size, v2);
// return G;
//
//
// }
//}
//
//int** splitV(int** G, int size, int v) {
// int** Gtemp = createG(size + 1);
//
// for (int i = 0; i < size; i++) {
// if (G[v][i] == 1) {
// Gtemp[i][size] = G[i][v];
// Gtemp[size][i] = G[i][v];
//
//
// }
// }
//
// for (int i = 0; i < size; i++) {
// for (int j = 0; j < size; j++) {
// Gtemp[i][j] = G[i][j];
// }
// }
//
// Gtemp[v][v] = 0;
// Gtemp[size][size] = 0;
// for (int i = 0; i < size; i++) {
// free(G[i]);
// }
//
// free(G);
// G = NULL;
//
// return Gtemp;
//}
//
//int** unionG(int** G1, int** G2, int size1, int size2) {
// int sizemax = (size1 > size2) ? size1 : size2;
// int sizemin = (size1 < size2) ? size1 : size2;
//
// int** Gmax = (size1 > size2) ? G1 : G2;
// int** Gmin = (size1 < size2) ? G1 : G2;
//
// int** Gtemp = createG(sizemax);
//
// for (int i = 0; i < sizemin; i++) {
// for (int j = 0; j < sizemin; j++) {
// Gtemp[i][j] = Gmin[i][j] | Gmax[i][j];
// }
// }
// for (int i = 0; i < sizemax; i++) {
// for (int j = sizemin; j < sizemax; j++) {
// Gtemp[i][j] = Gmax[i][j];
// Gtemp[j][i] = Gtemp[i][j];
// }
// }
//
// return Gtemp;
//
//}
//
//int** intersectoinG(int** G1, int** G2, int size1, int size2) {
// int sizemin = (size1 < size2) ? size1 : size2;
//
// int** Gtemp = createG(sizemin);
//
// for (int i = 0; i < sizemin; i++) {
// for (int j = 0; j < sizemin; j++) {
// Gtemp[i][j] = G1[i][j] & G2[i][j];
// }
// }
// return Gtemp;
//}
//
//int** xorG(int** G1, int** G2, int size1, int size2) {
// int sizemax = (size1 > size2) ? size1 : size2;
// int sizemin = (size1 < size2) ? size1 : size2;
//
// int** Gmax = (size1 > size2) ? G1 : G2;
// int** Gmin = (size1 < size2) ? G1 : G2;
//
// int** Gtemp = createG(sizemax - sizemin);
//
// for (int i = sizemin; i < sizemax; i++) {
// for (int j = sizemin; j < sizemax; j++) {
// Gtemp[i - sizemin][j - sizemin] = Gmax[i][j];
// }
// }
// return Gtemp;
//
//}
//
//
//
//
//int main(void) {
//
// srand(54352345342);
// int sizeG1 = 3,
// sizeG2 = 5;
// setlocale(LC_ALL, "");
//
// printf("Введите количество вершин графа 1: ");
// scanf("%d", &sizeG1);
// printf("Введите количество вершин графа 2: ");
// scanf("%d", &sizeG2);
//
// int** G1 = createG(sizeG1);
// int** G2 = createG(sizeG2);
// int** G3 = createG(sizeG1*sizeG2);
//
// //printf("граф 1 ");
// //printG(G1, sizeG1);
//
//
// //printf("граф 2 ");
// //printG(G2, sizeG2);
//
//
// printf("Граф 1 (матрица смежности):\n");
// printG(G1, sizeG1);
//
// printf("Граф 2 (матрица смежности):\n");
// printG(G2, sizeG2);
//
//
//
//
//
// int** G3 = unionG(G1, G2, sizeG1, sizeG2);
// int sizeG3 = (sizeG1 > sizeG2) ? sizeG1 : sizeG2;
// printf("\nОбъединение графов\n");
//
// printG(G3, sizeG3);
//
// int** G4 = intersectoinG(G1, G2, sizeG1, sizeG2);
// int sizeG4 = (sizeG1 < sizeG2) ? sizeG1 : sizeG2;
// printf("\nПересечение графов\n");
//
// printG(G4, sizeG4);
//
// G2 = delV(G2, g2, 2);
// nG2--;
// printf("\nГраф Godel\n");
// printf(G2, sizeG2);
// G2 = unionv(G2, sizeG2, 2, 3);
// sizeG2--;
// printG(G2, sizeG2);
// G2 = contrE(G2, sizeG2, 2, 3);
// sizeG2--;
// printG(G2, sizeG2);
// return 0;
// int** G5 = xorG(G1, G2, sizeG1, sizeG2);
// int sizeG5 = (sizeG1 < sizeG2) ? sizeG2 - sizeG1 : sizeG1 - sizeG2;
// printf("\nКольцевая сумма\n");
//
// printG(G5, sizeG5);
//
//
//
//
// return 0;
//
//
//
//
//}