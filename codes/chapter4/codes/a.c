/**
 * a.c
 */
extern int shared; 	// weak referrence

int main() {

	int a = 10;
	swap(&a, &shared);

}