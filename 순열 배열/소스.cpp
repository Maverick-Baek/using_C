#pragma warning(disable : 4996)
#include <stdio.h>
#include <stdlib.h>
#define MAX 7

//팩토리얼 함수(순열개수 찾기)
int factorial(int x) {
	if (x == 1) {
		return x;
	}
	return factorial(x - 1) * x;
}

int a[7] = { 0, 1, 2, 3, 4, 5, 6 };

int used[7]; // 현재 사용한 숫자를 표시

int p[7]; // 순열을 저장할 배열

int c = 0;
//[0,1,2,...,n-1]을 순열로 돌릴 때 k번째 리스트 출력
void perm(int n, int k, int l, int* permm) {
	if (k == n) {
		if (c == l) {
			for (int q = 0; q < n; q++) {
				permm[q] = p[q];
				p[q] = NULL;
			}
		}
		c++;
	}

	else {
		for (int i = 0; i < n; i++) {
			if (used[i] == 0) {
				used[i] = 1;
				p[k] = a[i];
				perm(n, k + 1, l, permm);
				used[i] = 0;
			}
		}
	}
}
//node struct 생성
typedef struct Node {
	int index;
	struct Node* next;
}node;

//새로운 노드 생성
node* init_node(int ind) {
	node* new_node = (node*)malloc(sizeof(node));
	new_node->index = ind;
	new_node->next = NULL;
	return new_node;
}

//push_front(근접 연결리스트에 사용)
void push(node* head, int ind) {
	node* new_node = init_node(ind);
	if (head->next == NULL) {
		head->next = new_node;
	}
	else if (head->next->index > ind) {
		push(head->next, ind);
	}
	else {
		new_node->next = head->next;
		head->next = new_node;
	}
}



//vertices가 n개인 두 그래프가 동형인지 파악(동형이면 1, 아니면 0)
int isisomorphism(node* g1[], node* g2[], int n) {
	node* n1;
	node* n2;
	int c = 0;//g1과 g2가 같은지 판단하는 수. 다르면 0보다 커짐
	printf("a\n");
	for (int l = 0; l < n; l++) {
		printf("as\n");
		n1 = g1[l];
		for (int m = 0; m < n; m++) {
			printf("asd    %d\n",m);
			n2 = g2[m];
			if (n1->index == n2->index) {
				printf("%d %d\n", n1->index, n2->index);
				printf("asdf\n");
				while (n1 != NULL || n2 != NULL) {//g1[l]이나 g2[m]가 끝까지 도착하면 마무리
					if ((n1 == NULL) || (n2 == NULL)) {
						c++;
						break;
					}
					printf("asdfg\n");
					if (n1->index != n2->index) {
						c++;
						break;
					}
					n1 = n1->next;
					n2 = n2->next;
				}
				n1 = g1[l];
				if (c >= 1)
					return 0;
			}
		}
	}
	return 1;
}

int main() {
	int n;
	int success;
	int per[MAX];
	int nlist1[MAX][MAX];
	int nlist2[MAX][MAX];
	node* gr1[MAX] = { init_node(0),init_node(1),init_node(2),init_node(3),init_node(4),init_node(5),init_node(6) };
	node* gr2[MAX] = { init_node(0),init_node(1),init_node(2),init_node(3),init_node(4),init_node(5),init_node(6) };
	scanf("%d\n", &n);

	//첫번째 리스트
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			scanf("%d", &nlist1[i][j]);
		}
	}
	printf("\n");

	//두번째 리스트
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			scanf("%d", &nlist2[i][j]);
		}
	}

	//그래프1 저장
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			if (nlist1[i][j] == 1) {
				push(gr1[i], j);
			}
		}
	}

	//그래프 2 순열개수만큼 돌리며 저장해보고 바로 같은지 판단하기
	//같은거 찾으면 success라는 변수에 1 대입하고 break
	for (int k = 0; k < factorial(n); k++) {
		perm(n, 0, k, per);
		for (int a = 0; a < n; a++) {
			for (int b = 0; b < n; b++) {
				if (nlist2[a][b] == 1) {
					push(gr2[per[a]], per[b]);
				}
			}
		}
	
		success = isisomorphism(gr1, gr2, n);
		for (int cv = 0; cv < n; cv++) {
			gr2[cv]->next = NULL;
			used[cv] = NULL;
		}
		c = 0;
		if (success == 1) {
			printf("True\n");
			return 0;
		}
	}
	printf("False\n");
	return 0;

}
	

