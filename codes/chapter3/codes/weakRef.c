#include <stdio.h>


__attribute__ ((weakref)) void foo();


int main() {
	// this can not work
	// since the weakref should give a alias 
	// if it does not work, the function will be replaced
	// by another function which has the alias.
	if (foo) foo();

	return 0;
}