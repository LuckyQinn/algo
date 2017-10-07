#include <stdio.h>
#include <limits.h>
#include <assert.h>>>><<<<<<

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

slist_t *slist_new()
{
	return alloc_slist_node(INT_MIN);
}

void slist_free(slist_t *l)
{
	slist_node_t *p = l;
	while(p != NULL)
	{
		slist_node_t *t = p;
		p = p->next;
		delete t;
		t = NULL;
	}
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
	int arr[] = {1, 28, 89, 56, 24, 36, 65, 99};
	slist_t *l = slist_new();

	//insert case
	for(int i=0; i<sizeof(arr)/sizeof(arr[0]); i++)
	{
		slist_insert(l, arr[i]);
	}

	slist_dump(l);

	//delete case 1: delete first element
	slist_delete(l, 1);
	slist_dump(l);

	//delete case 2: delete last element
	slist_delete(l, 99);
	slist_dump(l);
	
	//delete case 3: delete random element
	slist_delete(l, 56);
	slist_dump(l);
	
	slist_free(l);
	
	//insert/delete case
	int k = 56;
	l = slist_new();
	slist_insert(l, k);
	slist_delete(l, k);
	slist_free(l);
}


