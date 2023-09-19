#include <stdio.h>
#include <stdlib.h>

#define MAXSIZE 50
#define LENGTH 40
#define PLOTLENGTH 14


int fib(int n) {
	if (n == 0) {
		return 0;
	}
	if (n == 1) {
		return 1;
	}
	return fib(n - 1) + fib(n - 2);
}

void recursive_func(int *arr, int n) {
	arr[0] = 0;
	arr[1] = 1;
	for (int i = 2; i <= n; i++) {
		arr[i] = fib(i);
	}
}

void loop_func(int *arr, int n) {
	arr[0] = 0;
	arr[1] = 1;
	for (int i = 2; i <= n; i++) {
		arr[i] = arr[i - 1] + arr[i - 2];
	}
}

void plot_figure(int *arr, int n) {
	for (int i = 1; i <= n; i++) {
		printf("%d:", i);
		int max = (i * i > arr[i]) ? i * i : arr[i];
		for (int j = 0; j <= max; j++) {
			if (j == arr[i]) printf("*");
			else if (j == i * i) printf("x");
			else printf(" ");
		}
		printf("\n");
	}
	printf("            n * n: x     fib(n) = *\n");
}

int main() {
	int *arr1 = (int*)malloc(sizeof(int) * MAXSIZE);
	int *arr2 = (int*)malloc(sizeof(int) * MAXSIZE);

	recursive_func(arr1, LENGTH);
	loop_func(arr2, LENGTH);

	printf("Generate the fibonacci numbers by using recuisive way:\n");
	for (int i = 0; i < 4; i++) {
		for (int j = 1; j <= 10; j++){
			printf("%d ", arr1[i * 10 + j]);
		}
		printf("\n");
	}

	printf("Generate the fibonacci numbers by using loop way:\n");
	for (int i = 0; i < 4; i++) {
		for (int j = 1; j <= 10; j++){
			printf("%d ", arr2[i * 10 + j]);
		}
		printf("\n");
	}

	plot_figure(arr1, PLOTLENGTH);

	free(arr1);
	free(arr2);
	return 0;
}