/**
 * a_fix.c
 */
extern int shared; 	// weak referrence


// we need to specify the exit function to get out of the segent
void exit() {
	asm("movq $60, %rax \n\t"
		"syscall \n\t");
}


int hale() {

	int a = 10;
	swap(&a, &shared);

	exit();
}