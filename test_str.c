#include "yhrbtree.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const int size = 1024;

struct data {
	char buff[1024];	
};

const int data_key_cmp(const void* key1,  const void* key2)
{
	return strcmp(key1, key2);
}

const void * data_get_key(const void * data)
{
	return ((struct data*)data)->buff;
}


int main(void)
{
	struct rb_root root;
	struct rb_node_data *node;

	rbt_init(&root, data_get_key, data_key_cmp);
	
	int max = 10;
	int n;

	// insert
	printf("------------------- insert ----------------\n");
	for (n = 0; n < max; n++) 
	{
		node = calloc(1, sizeof(struct rb_node_data) + size);
		if (node == NULL)
		{
			printf("calloc error\n");
			exit(1);
		}

		node->size = sizeof(struct data);
		snprintf((char *)node->data, size, "Original %d", n);

		if (rbt_insert(&root, node) < 0) 
		{
			printf("rbt_insert %s error\n", node->data);
			free(node);
		}
		else 
			printf("rbt_insert %s ok\n", node->data);
	}

	// delete
	printf("------------------- delete ----------------\n");
	char buff[1024];
	for (n = 0; n < max; n += 2){
		snprintf(buff, size, "Original %d", n);
		if ((node = rbt_delete(&root, buff)) != 0) 
		{
			printf("rbt_delete %s ok\n", node->data);
			free(node);
		}
		else
			printf("rbt_delete %s failure\n", buff);
	}
	
	// search
	printf("------------------- search ----------------\n");
	for (n = 0; n < max; n++)
	{
		snprintf(buff, size, "Original %d", n);
		node = rbt_search(&root, buff);
		if (node == 0)  {
			printf("rbt_search %s failure\n", buff);
			continue;
		}
		else
			printf("rbt_search %s ok\n", buff);
		rbt_erase(&root, node);
		rbt_node_free(node);
		printf("rbt_erase %s ok\n", buff);
	}
	// search
	printf("------------------- search ----------------\n");
	for (n = 0; n < max; n++)
	{
		snprintf(buff, size, "Original %d", n);
		node = rbt_search(&root, buff);
		if (node == 0)  {
			printf("rbt_search %s failure\n", buff);
			continue;
		}
		else
			printf("rbt_search %s ok\n", buff);
	}
	
	return 0;
}	

