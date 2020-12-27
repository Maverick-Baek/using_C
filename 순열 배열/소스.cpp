#pragma warning(disable : 4996)
#include <stdio.h>
#include <stdlib.h>
#define MAX 7

//���丮�� �Լ�(�������� ã��)
int factorial(int x) {
	if (x == 1) {
		return x;
	}
	return factorial(x - 1) * x;
}

int a[7] = { 0, 1, 2, 3, 4, 5, 6 };

int used[7]; // ���� ����� ���ڸ� ǥ��

int p[7]; // ������ ������ �迭

int c = 0;
//[0,1,2,...,n-1]�� ������ ���� �� k��° ����Ʈ ���
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
//node struct ����
typedef struct Node {
	int index;
	struct Node* next;
}node;

//���ο� ��� ����
node* init_node(int ind) {
	node* new_node = (node*)malloc(sizeof(node));
	new_node->index = ind;
	new_node->next = NULL;
	return new_node;
}

//push_front(���� ���Ḯ��Ʈ�� ���)
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



//vertices�� n���� �� �׷����� �������� �ľ�(�����̸� 1, �ƴϸ� 0)
int isisomorphism(node* g1[], node* g2[], int n) {
	node* n1;
	node* n2;
	int c = 0;//g1�� g2�� ������ �Ǵ��ϴ� ��. �ٸ��� 0���� Ŀ��
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
				while (n1 != NULL || n2 != NULL) {//g1[l]�̳� g2[m]�� ������ �����ϸ� ������
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

	//ù��° ����Ʈ
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			scanf("%d", &nlist1[i][j]);
		}
	}
	printf("\n");

	//�ι�° ����Ʈ
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			scanf("%d", &nlist2[i][j]);
		}
	}

	//�׷���1 ����
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			if (nlist1[i][j] == 1) {
				push(gr1[i], j);
			}
		}
	}

	//�׷��� 2 ����������ŭ ������ �����غ��� �ٷ� ������ �Ǵ��ϱ�
	//������ ã���� success��� ������ 1 �����ϰ� break
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
	

