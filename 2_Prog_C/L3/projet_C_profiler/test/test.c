#include <stdio.h>
#include <stdlib.h>

#define PROFILER_ENABLED
#define MAIN_FILE
#include <macro_profiler.h>

int factorielle(int n) {
	PROFILE

	int res;
	if(n <= 1) {
		return 1;
	}
	else {
		res = n * factorielle(n - 1);
		return res;
	}
}


int itest() {
	PROFILE
	if(NULL == NULL) {
		while(1 < 0) {
			continue ;
		}
	}
	return 0;
}


int main(void) {
	PROFILE
	factorielle(10);
	factorielle(10);
	itest();

	return 1;
}
