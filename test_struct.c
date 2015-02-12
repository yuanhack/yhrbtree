#include "yhrbtree.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct data {
	int key;
	char buff[1024];
};

const int data_key_cmp(const void* key1,  const void* key2)
{
	int a = *(int*)key1;
	int b = *(int*)key2;
	return a > b ? 1 : a == b ? 0 : -1;
	//return *(int*)key1 - *(int*)key2;
}

const void * data_get_key(const void * data)
{
	return &((struct data*)data)->key;
}

int inorder_action(void *p, void *p2)
{
	struct data *pdata = (struct data*)p;
	return printf("%s inorder action: key %d size %d [%s]\n",  (char*) p2,
			pdata->key, get_node_data_size(pdata), 
			pdata->buff) 
		>= 0;
}

int clear_action(void *p, void *p2)
{
	return printf("%s clear %p, key %d size %d [%s]\n", (char *)p2,
			rb_entry(p, struct rb_node_data, data) ,
			((struct data*)p)->key,
			get_node_data_size(p),
			((struct data*)p)->buff) 
		>= 0;
}

int main(void)
{
	struct rb_root root;
	struct rb_node_data *node;
	struct data * pdata;
	int count = 0;
	int succ_count = 0;	

	rbt_init(&root, data_get_key, data_key_cmp);
	
	int max = 5; 
	int n;

	printf("------------------- insert ----------------\n");
	for (n = 0; n < max; n++) 
	{
		if ((node = rbt_node_new(sizeof(struct data))) == 0) {
			printf("calloc error\n");
			exit(1);
		}

		((struct data*)&node->data)->key = n;
		snprintf(((struct data*)&node->data)->buff
				, sizeof(((struct data*)&node->data)->buff), "%d Original", n);

		if (rbt_insert(&root, node) < 0) 
		{
			printf("rbt_insert key %d error\n", n);
			rbt_node_free(node);
		}
		printf("rbt_insert key %d %p [%s] ok\n", n, node, ((struct data*)&node->data)->buff);
	}



	printf("------------------- delete ----------------\n");
	for (n = 0; n < max; n+=2) {	
		if ((node = rbt_delete(&root, &n)) != 0) {
			printf("rbt_delete key %d %p [%s] ok\n", n, node, ((struct data*)&node->data)->buff);
			rbt_node_free(node);
		}
		else 
			printf("rbt_delete key %d failure \n", n);
	}
	printf("------------------- inorder ----------------\n");
	succ_count = 0;
	count = rbt_inorder(&root, "***", inorder_action, &succ_count);
	printf("inorder count node: %d, succ count: %d\n", count, succ_count);

	printf("---------------- first - next -----------------\n");
	node = rbt_node_first(&root);
	if (node) {
		pdata = (void*)node->data;
		printf("rbt_first: %p, key %d [%s]\n", node, pdata->key, pdata->buff);
		node = rbt_node_next(node);
		if (node) {
			pdata = get_data_pointer(node);
			printf("  first next: %p, key %d [%s]\n", node, pdata->key, pdata->buff);
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
		printf("rbt_last: %p, key %d [%s]\n", node, pdata->key, pdata->buff);
		node = rbt_node_prev(node);
		if (node) {
			pdata = get_data_pointer(node);
			printf("  last prev: %p, key %d [%s]\n", node, pdata->key, pdata->buff);
		}
		else
			printf("  last prev is null\n");
	}
	else
		printf("rbt_last is null\n");	

//	printf("------------------- replace ----------------\n");
//	for (n = 1; n < max; n += 1) 
//	{
//		struct rb_node_data *old;
//		node = 0;
//		if ((node = rbt_node_new(sizeof(struct data))) == 0) {
//			printf("calloc error\n");
//			continue;
//		}
//
//		node->size = sizeof(struct data);
//		((struct data*)&node->data)->key = n;
//		snprintf(((struct data*)&node->data)->buff
//				, sizeof(((struct data*)&node->data)->buff), "%d Replace", n);
//
//		if ((old = rbt_replace(&root, node))) {
//			printf("rbt_replace key %d old:%p new:%p [%s] ok\n", 
//					n, old, node, ((struct data*)&node->data)->buff);
//			rbt_node_free(old);
//		}
//		else 
//			printf("rbt_replace key %d failure\n", n);
//	} 

	printf("------------------- search -----------------\n");
	for (n = 0; n < max; n++)
	{
		node = rbt_search(&root, &n);
		if (node == 0) {
			printf("rbt_search key %d failure\n", n);
			continue;
		}
		printf("rbt_search key %d %p [%s] ok\n", n, node, ((struct data*)&node->data)->buff);
	}

	printf("------------------- inorder ----------------\n");
	count = rbt_inorder(&root, "***",inorder_action, 0);
	printf("inorder count node: %d\n", count);


	printf("------------------- clear -------------------\n");
	// clear test 1 
	//count = rbt_clear(&root, "---", 0, 0);
	//printf("clear count node: %d\n", count);
	
	// clear test 2
	count = rbt_clear(&root, "---", clear_action, &succ_count);
	printf("clear count node: %d, succ count: %d\n", count, succ_count);

	printf("------------------- finish inorder ----------------\n");
	count = rbt_inorder(&root, "***",inorder_action, 0);
	count = rbt_inorder(&root, "***",0, &succ_count);
	printf("finish count node: %d, succ count: %d\n", count, succ_count);

	int key = 9;
	node = rbt_search(&root, &key);
	pdata = (struct data*)node->data;
	if (node) {
		printf("is find %d\n", key);
		printf("key: %d, %s\n", pdata->key, pdata->buff);
	}
	else {
		printf("no find %d\n", key);
		node = rbt_node_new(sizeof(struct data));
		pdata = (struct data*)node->data;
		pdata->key = key;
		snprintf(pdata->buff, sizeof(pdata->buff), "last end %d", key);
		rbt_insert(&root, node);	

		printf("add finish %d\n", key);

	}


		node = rbt_search(&root, &key);
		pdata = (struct data*)node->data;
		if (node) {
			printf("is find %d\n", key);
			printf("key: %d, %s\n", pdata->key, pdata->buff);
		}
		else
			printf("err no find\n");

	return 0;
}	

