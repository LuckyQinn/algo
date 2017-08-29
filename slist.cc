#include <stdio.h>
#include <limits.h>
#include <assert.h>

typedef struct slist_node_tag {
	int key;
	slist_node_tag *next;
} slist_node_t;

typedef slist_node_t slist_t;

static slist_node_t *alloc_slist_node(int key)
{
	slist_node_t *t = new slist_node_t;
	assert(t != NULL);
	t->key = key;
	t->next = NULL;
	return t;
}

slist_t *slist_new_slist()
{
	return alloc_slist_node(INT_MIN);
}

void slist_delete(slist_t *l, int key)
{
	slist_node_t *p = l;
	slist_node_t *prev = p;
	while(p != NULL)
	{
		if(p->key == key)
		{
			prev->next = p->next;
			delete p;
			break;
		}
		else
		{
			prev = p;
			p = p->next;
		}
	}
}

void slist_insert(slist_t *l, int data)
{
	slist_node_t *p = l;
	slist_node_t *prev = p;
	while(p != NULL)
	{
		if(p->key < data)
		{
			prev = p;
			p = p->next;
		}
		else
		{
			slist_node_t *new_node = alloc_slist_node(data);
			prev->next = new_node;
			new_node->next = p;
			break;
		}
	}

	if(p == NULL)
	{
		slist_node_t *new_node = alloc_slist_node(data);
		prev->next = new_node;
	}
}

void slist_dump(slist_t *l)
{
	slist_node_t *p = l->next;
	while(p != NULL)
	{
		printf("%d\t", p->key);
		p = p->next;
	}
	printf("\n");
}

//test case
int main()
{
	//int arr[] = {1, 28, 89, 56, 24, 36, 65, 99};
	int arr[] = {56};
	slist_t *l = slist_new_slist();

	//insert test
	for(int i=0; i<sizeof(arr)/sizeof(arr[0]); i++)
	{
		slist_insert(l, arr[i]);
	}

	slist_dump(l);

	//delete test
	slist_delete(l, 56);

	slist_dump(l);

	//delete test
	slist_delete(l, 57);

	slist_dump(l);
}


