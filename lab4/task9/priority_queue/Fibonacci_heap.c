#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "Fibonacci_heap.h"
#include "../errors.h"

//for rebyild
typedef int loh;

void Fib_node_free(Fib_node* node) {
	if (node == NULL) {
		return;
	}

	if (node->left != NULL) {
		node->left->right = NULL;
	}
	
	Fib_node_free(node->child);
	Fib_node_free(node->right);

	free_application(node->req);
    free(node);
	//free_all(2, node->req, node);
}

int Fib_node_copy(Fib_node** node_dest, Fib_node* node_src, Fib_node* barrier) {
	if (node_dest == NULL) {
		return INVALID_FUNCTION_ARGUMENT;
	}
	
	if (node_src == NULL || node_src == barrier) {
		*node_dest = NULL;
		return ok;
	}
	
	*node_dest = (Fib_node*) malloc(sizeof(Fib_node));
	if (*node_dest == NULL) {
		return MEMORY_ISSUES;
	}
	
	(*node_dest)->rank = node_src->rank;
	// (*node_dest)->req = (Application*) malloc(sizeof(Application));
	// if ((*node_dest)->req == NULL) {
	// 	free(*node_dest);
	// 	return MEMORY_ISSUES;
	// }
	
	int code = copy_application(&((*node_dest)->req), node_src->req);
	if (code) {
		free((*node_dest)->req);
		free(*node_dest);
		return code;
	}
	
	barrier = barrier == NULL ? node_src : barrier;
	
	code = code ? code : Fib_node_copy(&(*node_dest)->child, node_src->child, NULL);
	code = code ? code : Fib_node_copy(&(*node_dest)->right, node_src->right, barrier);
	
	if (code) {
		Fib_node_free(*node_dest);
		return code;
	}
	
	if ((*node_dest)->right != NULL) {
		(*node_dest)->left = (*node_dest)->right->left;
		(*node_dest)->left->right = *node_dest;
		(*node_dest)->right->left = *node_dest;
	}
	else {
		(*node_dest)->left = (*node_dest);
	}
	
	return ok;
}


int Fib_heap_init(Fib_heap* fh, int (*compare)(const Application*, const Application*)) {
	if (fh == NULL) {
		return INVALID_FUNCTION_ARGUMENT;
	}

	fh->head = NULL;
	fh->compare = compare;
	fh->size = 0;

	return ok;
}

int Fib_heap_set_null(Fib_heap* fh) {
	if (fh == NULL) {
		return INVALID_FUNCTION_ARGUMENT;
	}

	fh->head = NULL;
	fh->compare = NULL;
	fh->size = 0;

	return ok;
}

int Fib_heap_free(Fib_heap* fh) {
	if (fh == NULL) {
		return ok;
	}

	Fib_node_free(fh->head);
	Fib_heap_set_null(fh);

	return ok;
}

int Fib_heap_copy(Fib_heap* fh_dest, Fib_heap* fh_src) {
	if (fh_dest == NULL || fh_src == NULL) {
		return INVALID_FUNCTION_ARGUMENT;
	}

	int status = Fib_heap_init(fh_dest, fh_src->compare);
	status = status ? status : Fib_node_copy(&(fh_dest->head), fh_src->head, NULL);
	if (status != ok) {
		Fib_heap_free(fh_dest);
		return status;
	}
	
	fh_dest->size = fh_src->size;

	return ok;
}

int Fib_heap_meld(Fib_heap* fh_res, Fib_heap* fh_l, Fib_heap* fh_r) {
	if (fh_res == NULL || fh_l == NULL || fh_r == NULL) {
		return INVALID_FUNCTION_ARGUMENT;
	}
	
	if (fh_l == fh_r || fh_l->compare != fh_r->compare) {
		return INVALID_INPUT;
	}
	
	if (fh_l->head == NULL || fh_r->head == NULL) {
		fh_res->head = fh_l->head == NULL ? fh_r->head : fh_l->head;
		Fib_heap_set_null(fh_l);
		Fib_heap_set_null(fh_r);
		return ok;
	}
	
	Fib_heap fh_tmp;
	
	fh_tmp.size = fh_l->size + fh_r->size;
	fh_tmp.compare = fh_l->compare;
	
	if (fh_l->head == NULL || fh_r->head == NULL) {
		fh_tmp.head = fh_l->head == NULL ? fh_r->head : fh_l->head;
	}
	else {
		fh_l->head->left->right = fh_r->head;
		fh_l->head->left = fh_r->head->left;
		fh_r->head->left = fh_l->head->left;
		fh_r->head->left->right = fh_l->head;
		
		fh_tmp.head = fh_l->compare(fh_l->head->req, fh_r->head->req) <= 0 ? fh_l->head : fh_r->head;
	}
	
	fh_l->size = fh_r->size = 0;
	fh_l->head = fh_r->head = NULL;
	*fh_res = fh_tmp;
	
	return ok;
}

int Fib_heap_copy_meld(Fib_heap* fh_res, Fib_heap* fh_l, Fib_heap* fh_r) {
	if (fh_res == NULL || fh_l == NULL || fh_r == NULL) {
		return INVALID_FUNCTION_ARGUMENT;
	}
	
	if (fh_res == fh_l || fh_res == fh_r) {
		return INVALID_INPUT;
	}
	
	int code = ok;
    Fib_heap fh_lc, fh_rc;
        
    Fib_heap_set_null(&fh_lc);
    Fib_heap_set_null(&fh_rc);
    Fib_heap_set_null(fh_res);
    
    code = code ? code : Fib_heap_copy(&fh_lc, fh_l);
    code = code ? code : Fib_heap_copy(&fh_rc, fh_r);
    code = code ? code : Fib_heap_meld(fh_res, &fh_lc, &fh_rc);
    
    Fib_heap_free(&fh_lc);
    Fib_heap_free(&fh_rc);
    
    if (code)
    {
        Fib_heap_free(fh_res);
        return code;
    }
    
    return ok;
}

int Fib_heap_size(Fib_heap* fh, unsigned int* size) {
	if (fh == NULL) {
		return INVALID_FUNCTION_ARGUMENT;
	}

	*size = fh->size;

	return ok;
}

int Fib_node_merge(Fib_heap* fh, Fib_node* node_1, Fib_node* node_2, Fib_node** res) {
	if (fh == NULL || node_1 == NULL || node_2 == NULL || res == NULL) {
		return INVALID_FUNCTION_ARGUMENT;
	}
	
	if (node_1 == node_2 || node_1->rank != node_2->rank) {
		return INVALID_INPUT;
	}
	
	if (fh->compare(node_1->req, node_2->req) > 0) {
		Fib_node* tmp = node_1;
		node_1 = node_2;
		node_2 = tmp;
	}
	
	// hang node_2 on node_1
	if (node_1->child != NULL) {
		node_2->right = node_1->child;
		node_2->left = node_1->child->left;
		node_2->left->right = node_2;
		node_2->right->left = node_2;
	}
	else {
		node_2->left = node_2;
		node_2->right = node_2;
	}
	
	node_1->child = node_2;
	node_1->rank++;
	
	*res = node_1;

	return ok;
}

int Fib_heap_consolidate(Fib_heap* fh) {
	if (fh == NULL) {
		return INVALID_FUNCTION_ARGUMENT;
	}
	
	if (fh->head == NULL || fh->head->right == fh->head) {
		return ok;
	}

	int code = ok;
	Fib_node* cur = fh->head;
	Fib_node* nxt = fh->head->right;
	unsigned int size = log2(1.0 * fh->size) + 3;

	Fib_node** arr = (Fib_node**) calloc(size, sizeof(Fib_node*));
	if (arr == NULL) {
		return MEMORY_ISSUES;
	}
	
	fh->head->left->right = NULL;
	
	while (!code && cur != NULL) {
		nxt = cur->right;
		
		if (cur->right == cur)
		{
			cur->right = NULL;
		}
		
		while (!code && arr[cur->rank] != NULL)
		{
			Fib_node* node_tmp = arr[cur->rank];
			arr[cur->rank] = NULL;
			code = Fib_node_merge(fh, cur, node_tmp, &cur);
			
			if (code)
			{
				arr[size - 1] = cur;
			}
		}
		
		arr[cur->rank] = cur;
		cur = nxt;
	}
	
	Fib_node* top = NULL;
	Fib_node* start = NULL;
	Fib_node* prev = NULL;
	
	for (unsigned int i = 0; i < size; ++i) {
		if (arr[i] == NULL)
		{
			continue;
		}
		
		start = start != NULL ? start : arr[i];
		top = top != NULL ? top : arr[i];
		
		if (fh->compare(arr[i]->req, top->req) < 0)
		{
			top = arr[i];
		}
		
		if (prev != NULL)
		{
			prev->right = arr[i];
		}
		
		arr[i]->left = prev;
		prev = arr[i];
	}
	
	prev->right = start;
	start->left = prev;
	fh->head = top;
	free(arr);
	
	return code;
}

int Fib_heap_get_max(Fib_heap* fh, Application** req) {
	if (fh == NULL || req == NULL) {
		return INVALID_FUNCTION_ARGUMENT;
	}
	
	if (fh->head == NULL) {
		req = NULL;
		return ok;
	}

	*req = fh->head->req;

	return ok;
}

int Fib_heap_del_max(Fib_heap* fh, Application** req) {
	if (fh == NULL || req == NULL) {
		return INVALID_FUNCTION_ARGUMENT;
	}
	
	if (fh->head == NULL) {
		*req = NULL;
		return ok;
	}
	
	Fib_node* top = fh->head;
	*req = fh->head->req;
	
	if (fh->head->right == fh->head && fh->head->child == NULL) {
		fh->head = NULL;
	}
	else if (fh->head->right == fh->head) {
		fh->head = fh->head->child;
	}
	else if (fh->head->child == NULL) {
		fh->head->left->right = fh->head->right;
		fh->head->right->left = fh->head->left;
		fh->head = fh->head->left;
	}
	else {
		fh->head->left->right = fh->head->child;
		fh->head->right->left = fh->head->child->left;
		fh->head->left->right->left = fh->head->left;
		fh->head->right->left->right = fh->head->right;
		fh->head = fh->head->left;
	}
	
	free(top);
	fh->size--;
	
	return Fib_heap_consolidate(fh);
}

int Fib_heap_insert(Fib_heap* fh, Application* req) {
	if (fh == NULL || req == NULL) {
		return INVALID_FUNCTION_ARGUMENT;
	}
	
	Fib_node* node = (Fib_node*)malloc(sizeof(Fib_node));
	if (node == NULL) {
		return MEMORY_ISSUES;
	}
	
	node->req = req;
	node->child = NULL;
	node->rank = 0;
	
	if (fh->head == NULL) {
		node->left = node->right = node;
		fh->head = node;
	}
	else {	
		node->left = fh->head->left;
		node->right = fh->head;
		node->left->right = node;
		node->right->left = node;

		if (fh->compare(node->req, fh->head->req) < 0)
		{
			fh->head = node;
		}
	}
	
	fh->size++;
	
	return ok;
}