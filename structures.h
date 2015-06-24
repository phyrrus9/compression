/* huffman structures */

#ifndef COMPRESS_STRUCTURES_H
#define COMPRESS_STRUCTURES_H

#include <stdint.h>
/*structure definitions*/
struct frequencytable
{
	uint32_t frequency[256];
};
struct frequencymap
{
	double frequency[256];
};
struct huffman_node
{
	uint8_t data;
	uint8_t holdsdata :1;
};
struct binary_node
{
	struct huffman_node *data;
	double value;
	struct binary_node *left, *right;
};
struct dlist_node
{
	struct binary_node *data;
	struct dlist_node *next, *prev;
};
struct dlist
{
	uint16_t size;
	struct dlist_node *head, *tail;
};
/*type definitions*/
typedef struct frequencytable frequencytable_t;
typedef struct frequencymap frequencymap_t;
typedef struct huffman_node huffmannode_t;
typedef struct binary_node binarynode_t;
typedef struct dlist_node dlistnode_t;
typedef struct dlist dlist_t;
/*frequency table*/
frequencytable_t *frequencytable_alloc();
void frequencytable_free(frequencytable_t *table);
void frequencytable_increment(frequencytable_t *table, uint8_t index);
/*frequency map*/
frequencymap_t *frequencymap_alloc();
void frequencymap_free(frequencymap_t *map);
void frequencymap_from_frequencytabe(frequencymap_t *map, frequencytable_t *table);
/*huffman node*/
huffmannode_t *huffmannode_alloc();
void huffmannode_free(huffmannode_t *node);
void huffmannode_set(huffmannode_t *node, uint8_t data); //enables holdsdata
void huffmannode_clear(huffmannode_t *node); //disables holdsdata
/*binary node*/
binarynode_t *binarynode_alloc();
void binarynode_free(binarynode_t *node); //recursive free
void binarynode_set_huffmannode(binarynode_t *node, huffmannode_t *data); //copies the huffmannode
void binarynode_set_value(binarynode_t *node, double value);
void binarynode_insert(binarynode_t *node, binarynode_t *newnode); //copies newnode to left or right
/*dlist node*/
dlistnode_t *dlistnode_alloc();
void dlistnode_free(dlistnode_t *node); //recursive free
void dlistnode_from_binarynode(dlistnode_t *node, binarynode_t *data); //copies data to node->data
void dlistnode_set_next(dlistnode_t *node, dlistnode_t *next); //copies next to node->next
void dlistnode_set_prev(dlistnode_t *node, dlistnode_t *prev); //copies prev to node->prev
dlistnode_t *dlistnode_get_next(dlistnode_t *node); //get node->next
dlistnode_t *dlistnode_get_prev(dlistnode_t *node); //get node->prev
/*dlist*/
dlist_t *dlist_alloc();
void dlist_free(dlist_t *list); //alias for dlistnode_free(list->head)
void dlist_insert(dlist_t *list, dlistnode_t *node); //add a node to list
void dlist_remove(dlist_t *list, dlistnode_t *node); //remove node from list
void dlist_remove_free(dlist_t *list, dlistnode_t *node); //remove node and call dlistnode_free(node)
dlistnode_t *dlist_get(dlist_t *list, uint16_t index); //get a node
uint16_t dlist_size(dlist_t *list); //returns list->size

#endif
