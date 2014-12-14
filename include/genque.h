/*
 *	genque.h
 *	description:
 *		defines node and queue classes that implement
 *		a reverse, singly-linked generic queue.
 *		node class allows for generic types by making
 *		use of compiler's void* casting rules.
 *
 *	created on: Nov 20, 2014
 *		author: jason
 */

#ifndef GENQUEUE_H_
#define GENQUEUE_H_

#include <stdlib.h>	
#include <stdio.h>

typedef struct Node node;
struct Node {
	void* info;
	node* link;
};
	
typedef struct {
	node* front;
	node* rear;
	char* err_msg;
} queue;

void  queue_init(queue*);		// Initializes queue of max int to empty state
void  queue_destroy(queue*);	// Deletes all queue elements cleans up memory
int	  queue_isempty(queue*);	// Returns 1 if queue is empty 0 otherwise
int   queue_enq(queue*, void*);	// Appends object to the rear of the queue
int   queue_deq(queue*);		// Removes object at the front of queue
void* queue_front(queue*);		// Returns front character, NULL if empty
void* queue_rear(queue*);		// Returns rear character, NULL if empty


void queue_init(queue* this) {
	// delete current queue elements
	if(!queue_isempty(this))
		queue_destroy(this);
}

void queue_destroy(queue* this) {
	while(this->front != NULL)
		queue_deq(this);
}

int queue_isempty(queue* this) {
	return (this->front == NULL);
}	

int queue_enq(queue* this, void* input) {

	node* new_node = malloc(sizeof(node*));
	if(new_node == NULL) {
		this->err_msg = "Error: failed to assign new memory";
		return 1;
	}
	else {
		new_node->info = input;
		new_node->link = NULL;

		if(queue_isempty(this)) {
			this->front = new_node;
			this->rear = new_node;
		}
		else {
			this->rear->link = new_node;
			this->rear = this->rear->link;
		}
	}
	
	return 0;
}

int queue_deq(queue* this) {
	if(queue_isempty(this)) {
		this->err_msg = "Error: Queue underflow";
		return 1;
	}
	else {
		node* temp;
		
		temp = this->front;
		this->front = this->front->link; // next in line is now at front
		
		free(temp); 
		
		if(queue_isempty(this)) // that was last element
			this->rear = NULL;
	}
	
	return 0;
}

void* queue_front(queue* this) {
	if(queue_isempty(this)) {
		this->err_msg = "Error: cannot access front, queue is empty";
		return NULL;
	}
	else	
		return(this->front->info);
}

void* queue_rear(queue* this) {
	if(queue_isempty(this)) {
		this->err_msg = "Error: cannot access rear, queue is empty";
		return NULL;
	}
	else
		return(this->rear->info);
}

#endif
