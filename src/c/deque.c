#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct DequeNode {
    void *val;
    struct DequeNode *prev, *next;
};

struct Deque {
    int size;
    struct DequeNode *head, *tail, *cur;
};

struct DequeNode *dequeNodeCreate(void *val){
    struct DequeNode *node=(struct DequeNode *)malloc(sizeof(struct DequeNode));
    node->next=node->prev=NULL;
    node->val=val;
    return node;
}

void dequeNodeSwap(struct Deque *deque, struct DequeNode *node1, struct DequeNode *node2){
	if(node1==deque->head || node1==deque->tail){
		return;
	}
	if(node2==deque->head || node2==deque->tail){
		return;
	}
	node1->prev->next=node2;
	node1->next->prev=node2;
	node2->prev->next=node1;
	node2->next->prev=node1;

	struct DequeNode *tmp=NULL;
	tmp=node1->prev;
	node1->prev=node2->prev;
	node2->prev=tmp;

	tmp=node1->next;
	node1->next=node2->next;
	node2->next=tmp;
}

int dequeSize(struct Deque *deque){
    return deque->size;
}

int dequeEmpty(struct Deque *deque){
    return deque->size==0;
}

void dequeCurPrev(struct Deque *deque){
	if(deque->cur==deque->head){
		return;
	}
	deque->cur=deque->cur->prev;
}

void dequeCurNext(struct Deque *deque){
	if(deque->cur==deque->tail){
		return;
	}
	deque->cur=deque->cur->next;
}

void dequeCurSetHead(struct Deque *deque){
	deque->cur=deque->head;
}

void dequeCurSetTail(struct Deque *deque){
	deque->cur=deque->tail;
}

void dequeCurSetPos(struct Deque *deque, int pos){
	if(pos<0 || deque->size<=pos){
		return;
	}
	if(deque->size/2<=pos){
		dequeCurSetTail(deque);
		for(int i=deque->size; i>pos; --i){
			dequeCurPrev(deque);
		}
		return;
	}
	dequeCurSetHead(deque);
	for(int i=0; i<=pos; ++i){
		dequeCurNext(deque);
	}
}

void dequePushCurFront(struct Deque *deque, void *val){
	struct DequeNode *node=dequeNodeCreate(val);
	node->prev=deque->cur->prev;
	node->next=deque->cur;
	deque->cur->prev->next=node;
	deque->cur->prev=node;
	++deque->size;
}

void dequePushCurBack(struct Deque *deque, void *val){
	struct DequeNode *node=dequeNodeCreate(val);
	node->prev=deque->cur;
	node->next=deque->cur->next;
	deque->cur->next->prev=node;
	deque->cur->next=node;
	++deque->size;
}

void *dequeCur(struct Deque *deque){
	if(deque->cur==deque->head || deque->cur==deque->tail){
		return NULL;
	}
	return deque->cur->val;
}

void dequeCurPopFront(struct Deque *deque, void valueDelete(void *)){
	if(!deque->size){
		return;
	}
	if(deque->cur==deque->head || deque->cur->prev==deque->head){
		return;
	}
	struct DequeNode *node=deque->cur->prev;
	deque->cur->prev->prev->next=node->next;
	deque->cur->prev=node->prev;
	deque->cur=node->prev;
	--deque->size;
	valueDelete(node->val);
	free(node);
}

void dequeCurPopBack(struct Deque *deque, void valueDelete(void *)){
	if(!deque->size){
		return;
	}
	if(deque->cur==deque->tail || deque->cur->next==deque->tail){
		return;
	}
	struct DequeNode *node=deque->cur->next;
	deque->cur->next->next->prev=node->prev;
	deque->cur->next=node->next;
	deque->cur=node->next;
	--deque->size;
	valueDelete(node->val);
	free(node);
}

void dequePushFront(struct Deque *deque, void *val){
    struct DequeNode *node=dequeNodeCreate(val);
	node->prev=deque->head;
	node->next=deque->head->next;
	deque->head->next->prev=node;
	deque->head->next=node;
    ++deque->size;
}

void dequePushBack(struct Deque *deque, void *val){
    struct DequeNode *node=dequeNodeCreate(val);
	node->prev=deque->tail->prev;
	node->next=deque->tail;
	deque->tail->prev->next=node;
	deque->tail->prev=node;
    ++deque->size;
}

void dequePopFront(struct Deque *deque, void valueDelete(void *)){
	if(!deque->size){
		return;
	}
	struct DequeNode *node=deque->head->next;
	deque->head->next->next->prev=node->prev;
	deque->head->next=node->next;
	--deque->size;
	valueDelete(node->val);
	free(node);
}

void dequePopBack(struct Deque *deque, void valueDelete(void *)){
	if(!deque->size){
		return;
	}
	struct DequeNode *node=deque->tail->prev;
	deque->tail->prev->prev->next=node->next;
	deque->tail->prev=node->prev;
	--deque->size;
	valueDelete(node->val);
	free(node);
}

void *dequeFront(struct Deque *deque){
    if(!deque->size){
		return NULL;
	}
	return deque->head->next->val;
}

void *dequeBack(struct Deque *deque){
    if(!deque->size){
		return NULL;
	}
	return deque->tail->prev->val;
}

struct Deque *dequeCreate(){
    struct Deque *deque=(struct Deque *)malloc(sizeof(struct Deque));
    deque->size=0;
    deque->head=dequeNodeCreate(NULL);
    deque->tail=dequeNodeCreate(NULL);
	deque->head->next=deque->tail;
	deque->tail->prev=deque->head;
	deque->cur=deque->head->next;
    return deque;
}

void dequeDelete(struct Deque *deque, void valueDelete(void *)){
	while(deque->size){
		dequePopFront(deque, valueDelete);
	}
	free(deque->head);
	free(deque->tail);
	free(deque);
}

int *intCreate(int x){
	int *res=(int *)malloc(sizeof(int));
	(*res)=x;
	return res;
}

void intDelete(void *val){
	int *rm=(int *)val;
	free(rm);
}

int main(void){
	int N;
	char op[16];

	scanf("%d", &N);
    struct Deque *deque=dequeCreate();
	while(N--){
		scanf("%s", op);
		if(!strcmp(op, "push_back")){
			int x;
			scanf("%d", &x);
			dequePushBack(deque, intCreate(x));
		}
		else if(!strcmp(op, "push_front")){
			int x;
			scanf("%d", &x);
			dequePushFront(deque, intCreate(x));
		}
		else if(!strcmp(op, "pop_front")){
			if(dequeFront(deque)){
				printf("%d\n", *((int *)dequeFront(deque)));
				dequePopFront(deque, intDelete);
			}
			else{
				printf("-1\n");
			}
		}
		else if(!strcmp(op, "pop_back")){
			if(dequeBack(deque)){
				printf("%d\n", *((int *)dequeBack(deque)));
				dequePopBack(deque, intDelete);
			}
			else{
				printf("-1\n");
			}
		}
		else if(!strcmp(op, "size")){
			printf("%d\n", dequeSize(deque));
		}
		else if(!strcmp(op, "empty")){
			printf("%d\n", dequeEmpty(deque));
		}
		else if(!strcmp(op, "front")){
			printf("%d\n", dequeFront(deque) ? *((int *)dequeFront(deque)) : -1);
		}
		else if(!strcmp(op, "back")){
			printf("%d\n", dequeBack(deque) ? *((int *)dequeBack(deque)) : -1);
		}
	}
	dequeDelete(deque, intDelete);

	return 0;
}