#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "utils.h"
#include "btree.h"

void freeBNode(t_bnode *node){
    if(node){
        freeBNode(node->left);
        freeBNode(node->right);
        free_(node);
    }
    return;
}

void destroyBTree(t_btree *btree){
    if(btree){
        freeBNode(btree->root);
        free_(btree);
    }
    return;
}

t_bnode *maxBTree(t_bnode *node){
    if(node->right == NULL)
        return (node);
        
    return maxBTree(node->right);
}

t_bnode *minBTree(t_bnode *node){
    if(node == NULL)
        return NULL;
        
    if(node->left == NULL)
        return (node);
        
    return minBTree(node->left);
}

int heightBTree(t_bnode *node){
    if(node->left == NULL && node->right == NULL)
        return (node->level)+1;
    
    int left = 0, right = 0;
    if(node->left)
        left = heightBTree(node->left);
    if(node->right)
        right = heightBTree(node->right);
        
    return (right>left)?(right):(left);
}

int sizeBTree(t_bnode *root){
    if(!root)
        return 0;
    
    return 1 + sizeBTree(root->left) + sizeBTree(root->right);
}

static void createBFSMatrix(t_bnode *node, int ***matrix, int **sizevec){
    if(node == NULL)
        return;
        
    int l = node->level;
    int t = (*sizevec)[l];
    (*matrix)[l] = (int *)realloc((*matrix)[l],sizeof(int)*(t+1));
    (*matrix)[l][t] = node->elem;
    (*sizevec)[l] += 1;
    
    createBFSMatrix(node->left,matrix,sizevec);
    createBFSMatrix(node->right,matrix,sizevec);
    return;
}

void printBFS(t_bnode *root){
    if(!root)
        return;
        
    //0. Variables...
    int i = 0, j = 0, maxlevel = 0;
    int *sizevec = NULL, **lmatrix = NULL;
        
    //1. Creating a preference level matrix...
    maxlevel = heightBTree(root);
    sizevec = (int *)calloc(maxlevel+1,sizeof(int));
    lmatrix = (int **)calloc(maxlevel+1,sizeof(int *));
    
    createBFSMatrix(root,&lmatrix,&sizevec);
    
    //2. Printing in BFS...
    for(i = 0 ; i < maxlevel ; i++){
        if(sizevec[i] != 0){
            for(j = 0 ; j < sizevec[i] ; j++)
                printf("%d ",lmatrix[i][j]);
        }
    }
    
    //3. Freeing used memory...
    for(i = 0 ; i < maxlevel ; i++){
        if(sizevec[i] != 0){
            free_(lmatrix[i]);
        }
    }free_(lmatrix);
    free_(sizevec);
    return;
}

void printPosOrder(t_bnode *btree){
    if(btree){
        printPosOrder(btree->left);
        printPosOrder(btree->right);
        printf("%d ",btree->elem);
    }
    return;
}

void printInOrder(t_bnode *btree, int *nelem){
    if(btree){
        printInOrder(btree->left,nelem);
        if(*nelem > 1)
            printf("%d, ",btree->elem);
        else
            printf("%d",btree->elem);
        (*nelem)--;
        printInOrder(btree->right,nelem);
    }
    return;
}

void printPreOrder(t_bnode *btree){
    if(btree){
        printf("%d ",btree->elem);
        printPreOrder(btree->left);
        printPreOrder(btree->right);
    }
    return;
}

t_bnode *createBNode(int elem, int level){
    t_bnode *node = (t_bnode *)malloc(sizeof(t_bnode));
        node->elem = elem;
        node->level = level;
        node->left = NULL;
        node->right = NULL;
    return node;
}

void searchBTree(int elem, t_bnode *node){
    if(node == NULL){
        printf("nao encontrado\n");
        return;
    }
    if(node->elem == elem){
        printf("encontrado\n");
        return;
    }
    else if(node->elem < elem)
        searchBTree(elem,node->right);
    else
        searchBTree(elem,node->left);
    return;
}

static void rectifyLevel(t_bnode **node,int level){
    if(!(*node))
        return;
    
    (*node)->level = level;
    rectifyLevel(&((*node)->left),level+1);
    rectifyLevel(&((*node)->right),level+1);
    return;
}

boolean deleteBTNode(int elem, t_bnode **node){
    if((*node) == NULL){
        printf("nao encontrado\n");
        return FALSE;
    }
	
	t_bnode *aux = NULL;
	if((*node)->elem < elem)  
        return deleteBTNode(elem,&((*node)->right));   
	else if((*node)->elem > elem)  
	    return deleteBTNode(elem,&((*node)->left));
    else{ 
	    if((*node)->left != NULL && (*node)->right != NULL ){    
	       aux = minBTree((*node)->right);
	       (*node)->elem = aux->elem;
	       deleteBTNode((*node)->elem,&((*node)->right));   
        }
        else{    
            aux = (*node);    
	       if((*node)->left == NULL){
	            (*node) = (*node)->right;
	            rectifyLevel(node,aux->level);
	       }
	       else{
	            (*node) = (*node)->left;
	            rectifyLevel(node,aux->level);
	       }
	       free(aux);  
	   }   
    }
    return TRUE;
}

void insertBTree(int elem, int level, t_bnode **node){
    if(!(*node)){
        *node = createBNode(elem,level);
        return;
    }
    if((*node)->elem > elem)
        insertBTree(elem,level+1,&((*node)->left));
    else if((*node)->elem < elem)
        insertBTree(elem,level+1,&((*node)->right));
    else
        printf("Chave existente\n");
    return;
}

t_btree *createBTree(){
    t_btree *tree = (t_btree *)malloc(sizeof(t_btree));
        tree->root = NULL;
    return tree;
}