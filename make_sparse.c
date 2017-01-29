/*
 * PROJ1-1: YOUR TASK B CODE HERE
 * 
 * Feel free to define additional helper functions.
 */

#include <stdlib.h>
#include <stdio.h>
#include "sparsify.h"
#include "utils.h"

#define WHITE_PIXEL 255

/* Returns a NULL-terminated list of Row structs, each containing a NULL-terminated list of Elem structs.
 * See sparsify.h for descriptions of the Row/Elem structs.
 * Each Elem corresponds to an entry in dense_matrix whose value is not 255 (white).
 * This function can return NULL if the dense_matrix is entirely white.
 */
void free_sparse(Row *sparse);
void free_spares_helper(Elem* elem);
Row *dense_to_sparse(unsigned char *dense_matrix, int width, int height) {\
    //should there be a row if there is NO elements?
    // we will be adding element in front    
    Row* head = NULL; //sentinel
    Row* tail = NULL; //sentinel
    Row* currRow = NULL;
    Row* backward = NULL;
    Elem* currEle = NULL;
    Elem* tailEle = NULL;

    for(int y = 0; y < height ; y++){ 
	
	// Creating new row
	currRow = (Row*)malloc(1*sizeof(Elem)); // My question: what happen unsinged <-> signed, type conversion?
	if(!currRow) allocation_failed();	
	currRow->next = NULL;
	currRow->elems = NULL;
	currRow->y = y;
	
	// First initialization of sentinel
	if (head == NULL) head = currRow;
	
	backward = tail;
	if (tail == NULL) tail = currRow;
	else{
		// Linking new row
		tail->next = currRow;
		tail = currRow;
	}
	
	// Creating elements in the list
	for(int x = 0; x < width; x++){
	    if(dense_matrix[y*width + x] < WHITE_PIXEL){

		// Creating node		
		currEle = (Elem*)malloc(1 * sizeof(Elem)); // explictly says this is for Elem
		if(!currEle) allocation_failed();
		currEle->value = dense_matrix[y*width + x];
		currEle->next = NULL;
		currEle->x = x;

		//First initialization
		if(!currRow->elems){
		currRow->elems = currEle;
		tailEle = currEle;
		}else{
			tailEle->next = currEle;
			tailEle = currEle;
		}			
	    }
	}
	if(!currRow->elems) // no element, should free and adjust tail and head
	{
		free(currRow);
		tail = backward;
		if(!tail) head = tail; //if it was the only row;
	}
    }
    return head;
}

/* Frees all memory associated with SPARSE. SPARSE may be NULL. */
void free_sparse(Row *sparse) {
    if(!sparse) // sparse is null
	return;
    free_spares_helper(sparse->elems);
    free_sparse(sparse->next);
    free(sparse);
}

void free_spares_helper(Elem* elem){
    if(!elem) //elem is null
	return;
     free_spares_helper(elem->next);
     free(elem);

}

