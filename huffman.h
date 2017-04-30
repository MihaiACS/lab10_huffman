#ifndef __HUFFMAN_H__
#define __HUFFMAN_H__

#include <string.h>

typedef struct {
    unsigned char value;
    int left, right;
} HuffmanNode;

#define MAX 256
#define Item int
#include "heap.h"

void computeFreqs(char *text, int size, int freqs[MAX]) {
    int i, j;

    // initial frecventa fiecarui caracter e 0
    for(i = 0; i < MAX; i++)
        freqs[i] = 0;

    for(i = 0; i < size; i++) {
        // test daca a fost calculata frecventa inainte
        if(freqs[(unsigned)text[i]] != 0)
            continue;

        // numar de aparitii al caracterului in sir
        for(j = 0; text[j] != '\0'; j++) {
            if(text[i] == text[j])
                freqs[(unsigned)text[i]]++;
        }
    }
}

HuffmanNode *makeTree(int freqs[MAX], int *size) {
    int i, leaf = 0;

    for(i = 0; i < MAX; i++) {
        if(freqs[i] != 0)
            leaf++;
    }

    if(leaf == 0) {
        return NULL;
    }

    APriQueue queue = makeQueue(leaf);
    HuffmanNode *tree = malloc(leaf * sizeof(HuffmanNode));
    ItemType node;

    for(i = 0; i < MAX; i++) {
        // adaugare frunze in coada si vectorul Huffman
        if(freqs[i] != 0) {
            // i - caracterul din text
            tree[*size].value = i;
            tree[*size].left = tree[*size].right = -1;

            node.prior = freqs[i];
            // content = pozitia nodului in vectorul Huffman
            node.content = *size;

            insert(queue, node);
            (*size)++;
        }
    }

    ItemType left, right;

    while(queue->size > 1) {
        left = removeMin(queue);
        right = removeMin(queue);

        node.prior = left.prior + right.prior;
        // content = pozitia nodului in vectorul Huffman
        node.content = *size;

        tree = realloc(tree, ((*size) + 1) * sizeof(HuffmanNode));
        tree[*size].value = node.prior;
        tree[*size].left = left.content;
        tree[*size].right = right.content;

        insert(queue, node);
        (*size)++;
    }

    //for(i = 0; i < (*size); i++) {
    //    printf("Pe pozitia %d este  %c/%d stanga - %d dreapta - %d\n",
    //    i, tree[i].value, tree[i].value, tree[i].left, tree[i].right);
    //}

    free(queue->elem);
    free(queue);
    return tree;
}

void makeCodes(int index, HuffmanNode *nodes, char **codes, char *code) {
    if (nodes[index].left == -1 && nodes[index].right == -1) {
        codes[index] = code;
    }

    if (nodes[index].left != -1) {
        char *temp = strdup(code);
        strcat(temp, "0");
        makeCodes(nodes[index].left, nodes, codes, temp);
    }

    if (nodes[index].right != -1) {
        char *temp = strdup(code);
        strcat(temp, "1");
        makeCodes(nodes[index].right, nodes, codes, temp);
    }
}

char *compress(char *text, int textSize, HuffmanNode *nodes, int numNodes) {
    char **codes, *code = "", compress[MAX];
    int i, j, leaf = 0;

    // daca textul este gol, implicit niciun nod in vector Huffman
    if(numNodes == 0) {
        return "-";
    }

    // daca avem doar un nod in vectorul Huffman
    if(numNodes == 1) {
        for(i = 0; i < textSize; i++) {
            strcat(compress, "0");
        }
        char *temp = strdup(compress);
        return temp;
    }

    for (i = 0; i < numNodes; i++) {
        if(nodes[i].left == -1 && nodes[i].right == -1)
            leaf++;
    }

    codes = malloc(leaf * sizeof(char*));
    for(i = 0; i < leaf; i++)
      codes[i] = malloc(MAX * sizeof(char));

    // introducem codurile in codes pentru fiecare frunza
    makeCodes(numNodes - 1, nodes, codes, code);

    for(i = 0; i < textSize; i++) {
        for(j = 0; j < leaf; j++) {
            if(text[i] == nodes[j].value) {
                strcat(compress, codes[j]);
                strcat(compress, " ");
            }
        }
    }

    for(i = 0; i < leaf; i++)
      free(codes[i]);
    free(codes);

    char *temp = strdup(compress);
    return temp;
}

char *decompress(char *text, int textSize, HuffmanNode *nodes, int numNodes) {
    int i = 0, j;
    char *str, *decompress, temp[2];
    decompress = malloc(MAX * 2 * sizeof(char));

    if(numNodes == 0) {
        return "";
    }

    str = strtok(text, " ");
    while(str != NULL && i < textSize){
        j = numNodes - 1;
        while(nodes[j].left != -1 || nodes[j].right != -1) {
            if(str[i] == '0')
                j = nodes[j].left;
            else if(str[i] == '1')
                j = nodes[j].right;
            i++;
        }

        sprintf(temp,"%c",nodes[j].value);
        strcat(decompress, temp);

        if(numNodes == 1) {
            i++;
        }
        else {
            i = 0;
            str = strtok(NULL, " ");
        }
    }

    //printf("\n%s\n\n", decompress);
    return decompress;
}
#endif
