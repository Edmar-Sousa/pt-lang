#include "symtab.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>


#define SYMTAB_MAX_SIZE     255
#define SHIFT                 4


static int hash(char * key)
{
    int temp = 0;
    
    for (int index = 0; key[index] != '\0'; index++)
        temp = ((temp << SHIFT) + key[index]) % SYMTAB_MAX_SIZE;
    
    return temp;
}

typedef struct LineListRec {
    int line;
    struct LineListRec * next;
} * LineList;


typedef struct BucketListRec {
    char * name;
    int memloc;

    LineList lines;
    struct BucketListRec * next;
} * BucketList;

static BucketList hashTable[SYMTAB_MAX_SIZE];


void stInsert(char * name, int line, int loc)
{
    int hashResult = hash(name);
    BucketList tableCell = hashTable[hashResult];

    while ((tableCell != NULL) && (strcmp(name, tableCell->name) != 0))
        tableCell = tableCell->next;

    if (!tableCell) 
    {
        tableCell = (BucketList) malloc(sizeof(struct BucketListRec));

        tableCell->name = name;
        tableCell->lines = (LineList) malloc(sizeof(struct LineListRec));
        tableCell->lines->line = line;
        tableCell->memloc = loc;

        tableCell->lines->next = NULL;
        tableCell->next = hashTable[hashResult];

        hashTable[hashResult] = tableCell;
    }

    else 
    {
        LineList t = tableCell->lines;

        while (t->next != NULL) 
            t = t->next;

        t->next = (LineList) malloc(sizeof(struct LineListRec));
        t->next->line = line;
        t->next->next = NULL;
    }
}


int stLookup(char * name)
{
    int hashName = hash(name);
    BucketList tableCell = hashTable[hashName];

    while ((tableCell != NULL) && (strcmp(name, tableCell->name) != 0))
        tableCell = tableCell->next;

    if (!tableCell)
        return -1;

    return tableCell->memloc;
}


