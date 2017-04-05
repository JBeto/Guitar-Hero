#ifndef CIRCLE_ARRAY_C
#define CIRCLE_ARRAY_C

// -------------------- Rectangle ----------------------
typedef struct rectangle {
	double freq;
	unsigned char row;
	unsigned char col;
	unsigned char length;
	unsigned char traveled;
} rectangle;
// -----------------------------------------------------

// -------------------Circular Array --------------------
typedef struct CircleArray {
	unsigned char cap;
	rectangle* LED_notes;
	rectangle* limit;
	rectangle* head;
	rectangle* tail;
	unsigned char sz;
} CircleArray;

unsigned char isEmpty(CircleArray* circle) {
	if (circle->sz == 0) {
		return 0;
	}
	else {
		return 1;
	}
}

rectangle* next(CircleArray* circle, rectangle* curr) {
	++curr;
	if (curr == circle->limit) { //if reached end of array, circle back
		return circle->LED_notes;
	}
	else {
		return curr;
	}
}

void pop_front(CircleArray* circle) {
	if (circle->sz > 0) {
		--(circle->sz);
		circle->head = next(circle, circle->head);
	}
}

void push_back(CircleArray* circle, rectangle* toAdd) {
	if (circle->sz < circle->cap) {
		++(circle->sz);
		*(circle->tail) = *toAdd;
		circle->tail = next(circle, circle->tail);
	}
}
// -------------------------------------------------------
#endif
