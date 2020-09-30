



/*
  Name: Akhil Snehal Modi
  UIN: 658281161
  NetID: amodi20@uic.edu

  Me and my friend have discussed on how to approach the pseudo-code and I have mentioned his name, uin and netid below.  	

  Name: Wajahat Khan
  UIN: 674981418
  NetID: wkhan25@uic.edu
*/


#include "memlib.h"
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "mm.h"
#include "hw4.h"
#include "memlib.h"

struct node * root_table[ROOT_NR] = {};

int in_use(unsigned int * hdrp) {
  return (*hdrp) & 0x1u;
}

// the three lowest bits are not part of the size
// 0x1 is already used for free blocks; we use 0x2
#define MARK_BIT (((unsigned int)0x2))

// marking related operations
int is_marked(unsigned int * hdrp) {
  return ((*hdrp) & MARK_BIT) >> 1; // return 1 or 0
}

void mark(unsigned int * hdrp) {
  (*hdrp) |= MARK_BIT;
}

void unmark(unsigned int * hdrp) {
  (*hdrp) &= ~MARK_BIT;
}

// same to GET_SIZE in mm.c
unsigned int block_size(unsigned int * hdrp) {
  return (*hdrp) & (~7u);
}

void* next_hdrp(unsigned int * hdrp) {
  const unsigned int size = block_size(hdrp);
  if (size == 0) {
    fprintf(stderr,"%s Panic, chunk is of zero size.\n", __func__);
    exit(-1);
  }
  hdrp = (unsigned int *)(((char *)hdrp) + size);
  return block_size(hdrp) ? hdrp : NULL;
}

void heap_stat(const char * msg)
{
  void *hdrp = mm_first_hdr();
  size_t nr_inuse = 0;
  size_t sz_inuse = 0;
  size_t nr_free = 0;
  size_t sz_free = 0;

  while (hdrp && block_size(hdrp)) {
    if (in_use(hdrp)) {
      nr_inuse++;
      sz_inuse += block_size(hdrp);
    } else {
      nr_free++;
      sz_free += block_size(hdrp);
    }
    if (is_marked(hdrp))
      printf("%s WARNING: found a mark\n", __func__);
    hdrp = next_hdrp(hdrp);
  }

  printf("%s: %s: heapsize %zu  inuse %zu %zu  free %zu %zu\n", __func__, msg,
      mem_heapsize(), nr_inuse, sz_inuse, nr_free, sz_free);
}

// ========
// add helper functions here if any

// Mark function: Traverses through the roit table and marks all the nodes reachable from the root table
//
//
/*
//int x = 0;
//struct node *currBlock = root_table[0];
//void *hdrp = mm_first_hdr();
void markBlock(void *hdrp, struct node *currBlock){ //, struct node *currBlock){
	int x = 0;
// 	struct node *currBlock = root_table[x];
//	void *hdrp = mm_first_hdr();
//	currBlock = root_table[x];
//	hdrp = mm_first_hdr();
//
//	currBlock = root_table[x];
//

	while(currBlock != NULL){ // && currBlock != 0)){  
			// Traversing through the root table untill the last node and also skipping the first node.
//		struct node *currBlock = root_table[x];
	//	if(currBlock == NULL){
	//		return;
	//	}
		if(is_marked(hdrp) == 1){
			return;
		}
		if(is_marked(hdrp) == 0){
			mark(hdrp);
		}
		int i = 0;
//		x++;
		while(i < mem_heapsize()){
			markBlock(next_hdrp(hdrp), currBlock->next);
			i++;
		}		
//		currBlock = currBlock->next;
//		hdrp = next_hdrp(hdrp);
//		currBlock = currBlock->next;
//		x++;
//		markBlock(hdrp);
	}
//	x++;
//	x++;
//	return;
}    //end of markBlock
*/

void markBlock(void *hdrp){
	int x = 0;
	struct node *currBlock = root_table[x];

	while(currBlock != NULL || currBlock == 0){
		if(currBlock == 0 && hdrp != 0){
			x++;
			if(x != ROOT_NR){
				currBlock = root_table[x];
			}
			hdrp = next_hdrp(hdrp);
			if(hdrp == NULL){
				return;
			}
		}
		else{
			if(is_marked(hdrp) == 1){
				return;
			}
			else if(is_marked(hdrp) == 0){
				mark(hdrp);
			}
			hdrp = next_hdrp(hdrp);
			x++;
			if(currBlock->next != NULL){
				currBlock = currBlock->next;
			}
			else{
				if(x != ROOT_NR){
					currBlock = root_table[x];
				}
				else{
					return;
				}
			}
		}
	}
}           // end of markBlock


void sweepBlock(void *hdrp){
	
	while(hdrp != NULL){
		if(is_marked(hdrp) == 1){
			unmark(hdrp);
		}
		else if(is_marked(hdrp) == 0){
			if(in_use(hdrp)){
				mm_free(hdrp+4);
			}
		}
		hdrp = next_hdrp(hdrp);
	}
}      // end of sweepBlock   


// garbage collection: Mark and Sweep
void gc() {
	struct node * cBlock = root_table[0];
	void *hdrp = mm_first_hdr();
	markBlock(hdrp); //, cBlock);
	sweepBlock(hdrp);
}
