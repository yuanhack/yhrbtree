#include "yhrbtree.h"

#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct data {
	int key1;
	int key2;
};

const int data_key_cmp(const void* key1,  const void* key2)
{
//	int a = *(int*)key1;
//	int b = *(int*)key2;
//	return a > b ? 1 : a == b ? 0 : -1;
	struct data *a, *b;
	a = (struct data*)key1;
	b = (struct data*)key2;
	if (a->key1 != b->key1)
		return a->key1 - b->key1;
	if (a->key2 != b->key2)
		return a->key2 - b->key2;
	return 0;
	//return *(int*)key1 - *(int*)key2;
}

const void * data_get_key(const void * data)
{
	return (struct data*)data;
}

int inorder_action(void *p, void *p2)
{
	struct data *pdata = (struct data*)p;
	return printf("%s inorder action: key1 %d  key2 %d size %d\n",  (char*) p2,
			pdata->key1, pdata->key2, get_node_data_size(pdata))
		>= 0;
}

int clear_action(void *p, void *p2)
{
	return printf("%s clear %p, key1 %d key2 %d size %d\n", (char *)p2,
			rb_entry(p, struct rb_node_data, data) ,
			((struct data*)p)->key1,
			((struct data*)p)->key2,
			get_node_data_size(p))
		>= 0;
}

int main(void)
{
	struct rb_root root;
	struct rb_node_data *node;
	struct data * pdata;
	int count = 0;
	int succ_count = 0;	

	srand(time(0));
	rbt_init(&root, data_get_key, data_key_cmp);
	
	int max = 5; 
	int n, m, l;

	printf("------------------- insert ----------------\n");
	for (n = 0; n < max; n++) 
	{
		for (l = 0; l < 20; l++) {
			if ((node = rbt_node_new(sizeof(struct data))) == 0) {
				printf("calloc error\n");
				exit(1);
			}

			node->size = sizeof(struct data);
			((struct data*)&node->data)->key1 = n;
			m = rand() % 10;
			((struct data*)&node->data)->key2 = m;

			if (rbt_insert(&root, node) < 0) 
			{
				printf("rbt_insert key1 %d key2 %d %p error\n", n, m, node);
				rbt_node_free(node);
				continue;
			}
			printf("rbt_insert key1 %d key2 %d %p ok\n", n, m, node);
		}
	}

	printf("------------------- inorder ----------------\n");
	succ_count = 0;
	count = rbt_inorder(&root, "***", inorder_action, &succ_count);
	printf("inorder count node: %d, succ count: %d\n", count, succ_count);

	printf("---------------- first - next -----------------\n");
	node = rbt_node_first(&root);
	if (node) {
		pdata = (void*)node->data;
		printf("rbt_first: %p, key1 %d key2 %d\n", node, pdata->key1, pdata->key2);
		node = rbt_node_next(node);
		if (node) {
			pdata = get_data_pointer(node);
			printf("  first next: %p, key1 %d key2 %d\n", node, pdata->key1,pdata->key2);
		}
		else
			printf("  first next is null\n");
	}
	else 
		printf("rbt_first is null\n");

	printf("---------------- last - prev ------------------\n");
	node = rbt_node_last(&root);
	if (node) {
		pdata = (struct data*)node->data;
		printf("rbt_last: %p, key1 %d key2 %d\n", node, pdata->key1, pdata->key2);
		node = rbt_node_prev(node);
		if (node) {
			pdata = get_data_pointer(node);
			printf("  last prev: %p, key1 %d key2 %d\n", node, pdata->key1, pdata->key2);
		}
		else
			printf("  last prev is null\n");
	}
	else
		printf("rbt_last is null\n");	

//	printf("------------------- inorder ----------------\n");
//	count = rbt_inorder(&root, "***",inorder_action, 0);
//	printf("inorder count node: %d\n", count);


	printf("------------------- clear -------------------\n");
	// clear test 1 
	count = rbt_clear(&root, "---", 0, 0);
	printf("clear count node: %d\n", count);
	
	// clear test 2
	//count = rbt_clear(&root, "---", clear_action, &succ_count);
	//printf("clear count node: %d, succ count: %d\n", count, succ_count);

	printf("------------------- finish inorder ----------------\n");
	count = rbt_inorder(&root, "***",inorder_action, 0);
	count = rbt_inorder(&root, "***",0, &succ_count);
	printf("finish count node: %d, succ count: %d\n", count, succ_count);

	return 0;
}	

