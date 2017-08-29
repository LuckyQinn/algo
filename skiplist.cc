#include <stdio.h>
#include <assert.h>
#include <limits.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

typedef struct skiplist_node_tag {
	int key;
	struct skiplist_node_tag *next[0];
} skiplist_node_t;

typedef struct skiplist_tag {
	int max_level;
	skiplist_node_t *head;
	skiplist_node_t *tail;
} skiplist_t;

const int SKIPLIST_MAX_LEVEL = 16;
const int SKIPLIST_KEY_MIN = INT_MIN;
const int SKIPLIST_KEY_MAX = INT_MAX;
const int SKIPLIST_P = 4;

static skiplist_node_t *skiplist_node_new(int key, int level)
{
	level = level < 0 ? 0 : level;
	skiplist_node_t *t = (skiplist_node_t *)malloc(sizeof(skiplist_node_t) + sizeof(skiplist_node_t *)*level);
	memset(t, 0, sizeof(skiplist_node_t) + sizeof(skiplist_node_t *)*level);
	assert(t != NULL);
	t->key = key;
	return t;
}

skiplist_t *skiplist_new(int max_level)
{
    srandom((unsigned int)time(NULL));
	skiplist_t *t = (skiplist_t *)malloc(sizeof(skiplist_t));
    memset(t, 0, sizeof(skiplist_t));
	assert(t != NULL);
	t->head = skiplist_node_new(SKIPLIST_KEY_MIN, max_level);
	t->tail = skiplist_node_new(SKIPLIST_KEY_MAX, max_level);
	for(int i=0; i<max_level; i++)
	{
		t->head->next[i] = t->tail;
	}

	return t;
}

void skiplist_free(skiplist_t *l)
{
    if(l != NULL)
    {
        free(l->head);
        free(l->tail);
        free(l);
    }
}
bool skiplist_find(skiplist_t *l, int key)
{
	int i = l->max_level - 1;
	skiplist_node_t *p = l->head;
	while(i >= 0)
	{
		while(p->next[i]->key < key)
		{
			p = p->next[i];
		}
		if(p->next[i]->key == key)
		{
			return true;
		}
		else
		{
			i--;
		}
	}
	if(i < 0)
	{
		return false;
	}
    return true;
}

static int random_level()
{
    int level = 1;  
	while ((random()&0xFFFF) < (0xFFFF / SKIPLIST_P))
	{		
		level += 1;
	}

	return (level<SKIPLIST_MAX_LEVEL) ? level : SKIPLIST_MAX_LEVEL;
}

// in ascending order
void skiplist_insert(skiplist_t *l, int key)
{
	int i = l->max_level - 1;
	skiplist_node_t *p = l->head;

	skiplist_node_t **update = new skiplist_node_t *[SKIPLIST_MAX_LEVEL];
	for(int j=0; j<SKIPLIST_MAX_LEVEL; j++)
	{
		update[j] = l->head;
	}
	while(i >= 0)
	{
		while(p->next[i]->key < key)
		{
			p = p->next[i];
		}
		if(p->next[i]->key == key)
		{
			return;
		}
		else
		{
			update[i] = p;
			i--;
		}
	}

	int level = random_level();
	printf("key=%d, random level = %d\n", key, level);
	skiplist_node_t *new_node = skiplist_node_new(key, level);
	l->max_level = l->max_level > level ? l->max_level : level;

	i = level - 1;
	while(i >= 0)
	{
		new_node->next[i] = update[i]->next[i];
		update[i]->next[i] = new_node;
		i--;
	}

	delete []update;
}

void skiplist_delete(skiplist_t *l, int key)
{
	int i = l->max_level - 1;
	skiplist_node_t *p = l->head;
    skiplist_node_t *del_node = NULL;

	while(i >= 0)
	{
		while(p->next[i]->key < key)
		{
			p = p->next[i];
		}
		if(p->next[i]->key == key)
		{
            del_node = p->next[i];
            p->next[i] = p->next[i]->next[i];
		}
        
        if (l->head->next[i] == l->tail) {
            l->max_level = i;
        }
		i--;
	}


	if(del_node != NULL)
    {
        delete del_node;
    }
}

void skiplist_dump(skiplist_t *l)
{
	int i = l->max_level - 1;
	while(i >= 0)
	{
		skiplist_node_t *p = l->head;
		while(p != NULL)
		{
			printf("%d\t", p->key);
			p = p->next[i];
		}
		printf("\n");
		i--;
	}
    printf("skiplist max level:%d\n", l->max_level);
}

// test case
int main()
{
	skiplist_t *l = skiplist_new(SKIPLIST_MAX_LEVEL);
    
    // multi element insert/find/delete
    printf("\n===============insert multi element=================\n");
	int arr[] = {25, 14, 8, 34, 31, 58, 9, 66, 77, 42, 99};
	for(int i=0; i<sizeof(arr)/sizeof(arr[0]); i++)
	{
		skiplist_insert(l, arr[i]);
	}
	skiplist_dump(l);
   	printf("=================end===============\n\n\n");
	
	// find true case
	assert(skiplist_find(l, 58) == true);
	// find false case
	assert(skiplist_find(l, 80) == false);

	// delete case 1:first element
    printf("\n============delete first element====================\n");
	skiplist_delete(l, 8);
	skiplist_dump(l);
	printf("===================end=============\n\n");
    
    // delete case 2:last element
    printf("\n============delete last element====================\n");
    skiplist_delete(l, 99);
    skiplist_dump(l);
    printf("===================end=============\n\n");
    
    // delete case 3:middle random element
    printf("\n============delete random element====================\n");
    skiplist_delete(l, 66);
    skiplist_dump(l);
    printf("=====================end===========\n\n");
    
    //delete case 4:unexist element
    printf("\n============delete unexist element====================\n");
    skiplist_delete(l, 57);
    skiplist_dump(l);
    printf("==================end==============\n\n");
    skiplist_free(l);
    
    // one element insert/find/delete
    l = skiplist_new(SKIPLIST_MAX_LEVEL);
    skiplist_insert(l, 30);
    assert(skiplist_find(l, 30) == true);
    assert(skiplist_find(l, 20) == false);
    skiplist_delete(l, 30);
    skiplist_delete(l, 20);
    skiplist_free(l);
}
