//functions for compression structures

#include <stdlib.h>
#include <stdio.h>
#include <string.h> //for memset
#include <assert.h>
#include "structures.h"

/*frequency table*/
frequencytable_t *frequencytable_alloc()
{
	frequencytable_t *ret = malloc(sizeof(frequencytable_t));
	memset(ret->frequency, 0, sizeof(uint32_t) * 256);
	return ret;
}
void frequencytable_free(frequencytable_t *table)
{
	free(table);
};
void frequencytable_increment(frequencytable_t *table, uint8_t index)
{
	++table->frequency[index];
}
/*frequency map*/
frequencymap_t *frequencymap_alloc()
{
	frequencymap_t *ret = malloc(sizeof(frequencymap_t));
	memset(ret->frequency, 0, sizeof(double) * 256);
	return ret;
}
void frequencymap_free(frequencymap_t *map)
{
	free(map);
}
void frequencymap_from_frequencytabe(frequencymap_t *map, frequencytable_t *table)
{
	uint64_t total = 0;
	uint16_t i;
	double frequency = 0;
	for (i = 0; i < 256; i++)
		total += table->frequency[i];
	for (i = 0; i < 256; i++)
		map->frequency[i] = table->frequency[i] / total;
}
/*huffman node*/
huffmannode_t *huffmannode_alloc()
{
	huffmannode_t *ret = malloc(sizeof(huffmannode_t));
	ret->holdsdata = 0; //if no data held, we don't care what data is
	return ret;
}
void huffmannode_free(huffmannode_t *node)
{
	free(node);
}
void huffmannode_set(huffmannode_t *node, uint8_t data) //enables holdsdata
{
	node->data = data;
	node->holdsdata = 1;
}
void huffmannode_clear(huffmannode_t *node) //disables holdsdata
{
	node->holdsdata = 0;
}
/*binary node*/
binarynode_t *binarynode_alloc()
{
	binarynode_t *ret = malloc(sizeof(binarynode_t));
	ret->data = NULL; /*********************watch out for this!*****************/
	ret->value = 0;
	ret->left = ret->right = NULL; /**********************watch this!*********/
	return ret;
}
void binarynode_free(binarynode_t *node) //recursive free
{
	if (node->right)
		binarynode_free(node->right);
	if (node->left)
		binarynode_free(node->left);
	node->right = NULL;
	node->left = NULL;
	free(node);
}
void binarynode_set_huffmannode(binarynode_t *node, huffmannode_t *data) //copies the huffmannode
{
	node->data = data;
}
void binarynode_set_value(binarynode_t *node, double value)
{
	node->value = value;
}
void binarynode_insert(binarynode_t *node, binarynode_t *newnode) //copies newnode to left or right
{
	assert(newnode->value != node->value);
	if (newnode->value > node->value) //right
		node->right = newnode;
	else
		node->left = newnode;
}
/*dlist node*/
dlistnode_t *dlistnode_alloc()
{
	dlistnode_t *ret = malloc(sizeof(dlistnode_t));
	ret->data = NULL;
	ret->next = ret->prev = NULL; /********watch this*******/
	return ret;
}
void dlistnode_free(dlistnode_t *node) //recursive free
{
	if (dlistnode_get_next(node))
		dlistnode_free(dlistnode_get_next(node));
	if (dlistnode_get_prev(node))
		dlistnode_free(dlistnode_get_prev(node));
	node->prev = node->next = NULL;
	free(node);
}
void dlistnode_from_binarynode(dlistnode_t *node, binarynode_t *data) //copies data to node->data
{
	node->data = data;
}
void dlistnode_set_next(dlistnode_t *node, dlistnode_t *next) //copies next to node->next
{
	node->next = next;
}
void dlistnode_set_prev(dlistnode_t *node, dlistnode_t *prev) //copies prev to node->prev
{
	node->prev = prev;
}
dlistnode_t *dlistnode_get_next(dlistnode_t *node) //get node->next
{
	return node->next;
}
dlistnode_t *dlistnode_get_prev(dlistnode_t *node) //get node->prev
{
	return node->prev;
}
/*dlist*/
dlist_t *dlist_alloc()
{
	dlist_t *ret = malloc(sizeof(dlist_t));
	ret->size = 0;
	ret->head = ret->tail = NULL;
	return ret;
}
void dlist_free(dlist_t *list) //alias for dlistnode_free(list->head)
{
	dlistnode_free(list->head);
	list->size = 0;
	list->head = list->tail = NULL;
}
void dlist_insert(dlist_t *list, dlistnode_t *node) //add a node to list
{
	if (list->size != 0) //list not empty
	{
		dlistnode_set_next(list->tail, node);
		dlistnode_set_prev(node, list->tail);
		list->tail = node;
	}
	else //empty list
		list->head = list->tail = node;
	++list->size;
}
void dlist_remove(dlist_t *list, dlistnode_t *node) //remove node from list
{
	//update the list
	dlistnode_get_prev(node)->next = dlistnode_get_next(node);
	dlistnode_get_next(node)->prev = dlistnode_get_prev(node);
	--list->size;
}
void dlist_remove_free(dlist_t *list, dlistnode_t *node) //remove node and call dlistnode_free(node)
{
	dlist_remove(list, node);
	dlistnode_free(node);
}
dlistnode_t *dlist_get(dlist_t *list, uint16_t index) //get a node
{
	dlistnode_t *ptr = list->head;
	uint16_t i;
	if (index > list->size) return NULL;
	for (i = 0; i < index; i++, ptr = dlistnode_get_next(ptr));
	return ptr;
}
uint16_t dlist_size(dlist_t *list) //returns list->size
{
	return list->size;
}
