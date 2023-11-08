#include <stdio.h>
#include <stdlib.h>
#include<math.h>

#define TRUE 1
#define FALSE 0

#define MAX_VERTICES 100
#define INF 1000

int parent[MAX_VERTICES];		// 부모 노드

// 초기화
void set_init(int n) {
	for (int i = 0; i < n; i++)
		parent[i] = -1;	
}
// curr가 속하는 집합을 반환한다.
int set_find(int curr) {
	if (parent[curr] == -1)
		return curr; 			// 루트 
	while (parent[curr] != -1) curr = parent[curr];
	return curr;
}
// 두개의 원소가 속한 집합을 합친다.
void set_union(int a, int b) {
	int root1 = set_find(a);	// 노드 a의 루트를 찾는다. 
	int root2 = set_find(b);	// 노드 b의 루트를 찾는다. 
	if (root1 != root2) 	// 합한다. 
		parent[root1] = root2;
}

typedef struct Edge{			// 간선을 나타내는 구조체
	int start, end, weight;
} Edge;


typedef struct GraphType {
	int n;	// 노드의 개수
	int num_edges;
	Edge edges[2 * MAX_VERTICES];
} GraphType;
// 그래프 초기화 
void graph_init(GraphType* g, int n) {
	g->n = n;
	g->num_edges = 0;
	for (int i = 0; i < 2 * MAX_VERTICES; i++) {
		g->edges[i].start = 0;
		g->edges[i].end = 0;
		g->edges[i].weight = INF;
	}
}
// 간선 삽입 연산
void insert_edge(GraphType* g, int start, int end, int w) {
	g->edges[g->num_edges].start = start;
	g->edges[g->num_edges].end = end;
	g->edges[g->num_edges].weight = w;
	g->num_edges++;
}
// qsort()에 사용되는 함수
int compare(const void* a, const void* b) {
	struct Edge* x = (Edge*)a;
	struct Edge* y = (Edge*)b;
	return (x->weight - y->weight);
}
// kruskal의 최소 비용 신장 트리 프로그램
void kruskal(GraphType* g) {
	int edge_accepted = 0;	// 현재까지 선택된 간선의 수	
	int uset, vset;			// 정점 u와 정점 v의 집합 번호
	struct Edge e;

	set_init(g->n);				// 집합 초기화
	qsort(g->edges, g->num_edges, sizeof(Edge), compare);
	
	printf("1. Kruskal MST Algorithm\n");
	int i = 0;
	while (edge_accepted < (g->n - 1))	// 간선의 수 < (n-1)
	{
		e = g->edges[i];
		uset = set_find(e.start);		// 정점 u의 집합 번호 
		vset = set_find(e.end);		// 정점 v의 집합 번호
		if (uset != vset) {			// 서로 속한 집합이 다르면
			printf("Edge (%d, %d) Select %d \n", e.start, e.end, e.weight);
			edge_accepted++;
			set_union(uset, vset);	// 두개의 집합을 합친다.
		}
		i++;
	}
}

// MinHeap 구조체 정의
typedef struct HeapType {
	Edge heap[MAX_VERTICES];
	int heapsize;
} HeapType;

// Heap 생성 함수
HeapType* create() {
	return ((HeapType*)malloc(sizeof(HeapType)));
}

// Heap 초기화 함수
void init_heap(HeapType* h) {
	h->heapsize = 0;
}

// Heap 삽입 함수
void insert_heap(HeapType* h, Edge* node) {
	int i = ++h->heapsize;
	while (i != 1 && node->weight < h->heap[i / 2].weight) {
		h->heap[i] = h->heap[i / 2];
		i = i / 2;
	}
	h->heap[i] = *node;
}

// Heap 삭제 함수
Edge delete_heap(HeapType* h) {
	Edge item, temp;
	int parent = 1, child = 2;
	item = h->heap[1];
	temp = h->heap[h->heapsize--];

	while (child < h->heapsize && temp.weight > h->heap[child].weight) {
		if (h->heap[child].weight > h->heap[child + 1].weight) {
			child++;
		}
		if (temp.weight < h->heap[child].weight) break;
		h->heap[parent] = h->heap[child];
		parent = child;
		child *= 2;
	}
	h->heap[parent] = temp;
	return item;
}

// MinHeap을 사용한 Kruskal MST 알고리즘 함수
void kruskal_minheap(GraphType* g) {
	int edge_accepted = 0;
	int uset, vset;
	Edge e;

	HeapType* h = create();
	init_heap(h);

	set_init(g->n);

	for (int i = 0; i <= g->num_edges; i++) {
		insert_heap(h, &g->edges[i]);
	}

	printf("2. Using MinHeap MST Algorithm\n");

	while (edge_accepted < (g->n - 1)) {
		e = delete_heap(h);

		uset = set_find(e.start);
		vset = set_find(e.end);

		if (uset != vset) {
			printf("Edge (%d, %d) Select %d\n", e.start, e.end, e.weight);
			edge_accepted++;
			set_union(uset, vset);
		}
	}

	free(h);
}

int main(int argc, char* argv[]) {
	GraphType* g;
	g = (GraphType*)malloc(sizeof(GraphType));
	graph_init(g, 10);

	insert_edge(g, 1, 2, 3);
	insert_edge(g, 1, 6, 11);
	insert_edge(g, 1, 7, 12);
	insert_edge(g, 2, 3, 5);
	insert_edge(g, 2, 4, 4);
	insert_edge(g, 2, 5, 1);
	insert_edge(g, 2, 6, 7);
	insert_edge(g, 2, 7, 8);
	insert_edge(g, 3, 4, 2);
	insert_edge(g, 3, 7, 6);
	insert_edge(g, 3, 8, 5);
	insert_edge(g, 4, 5, 13);
	insert_edge(g, 4, 8, 14);
	insert_edge(g, 4, 10, 16);
	insert_edge(g, 5, 9, 18);
	insert_edge(g, 5, 10, 17);
	insert_edge(g, 7, 8, 13);
	insert_edge(g, 8, 10, 15);
	insert_edge(g, 9, 10, 10);

	kruskal(g);
	printf("\n");
	kruskal_minheap(g);
	free(g);
	return 0;
}
