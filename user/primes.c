#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

const int LAST_PRIME = 280;
const int FIRST_PRIME = 2;

int process(int in_fd, int prime) {
	int p[2];
	pipe(p);
	int out_pipe = p[1];

	int num;
	int pid = fork();
	if (pid == 0) {
		while (read(in_fd, &num, sizeof(int))) {
			if (num % prime != 0) {
				while(write(out_pipe, &num, sizeof(int)) == -1){
					
				}
			}
		}
		close(in_fd);
		close(out_pipe);
		exit(0);
	}
	else {
		close(in_fd);
		close(out_pipe);
		wait(0);
		return p[0];
	}
}

int main(int argc, char *argv[]) {
	int p[2];
	pipe(p);
    int pid = fork();
	if (pid == 0) {
		for (int i = FIRST_PRIME; i <= LAST_PRIME; i++) {
			// printf("test %d\n", i);
			
			while(write(p[1], &i, sizeof(int)) == -1){

			}
		}
		close(p[1]);
		exit(0);
	}
	else {
		close(p[1]);

		int in_pipe = p[0];
		int prime; 
		while (read(in_pipe, &prime, sizeof(int))) {
			printf("prime %d\n", prime); 
			in_pipe = process(in_pipe, prime);
		}

		wait(0);
		exit(0);
	}
}

