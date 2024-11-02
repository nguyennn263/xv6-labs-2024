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
		// Lan luot sang so nguyen to
		while (read(in_fd, &num, sizeof(int))) {
			if (num % prime != 0) {
				// Tranh truong hop pipe bi day.
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
// Boi vi pipe chi chua duoc 512 byte tuong ung voi 128 so nguyen -> khong the viet het 279 so vao pipe
// Do do can phai vua ghi vua doc tren pipe. 
// Co the thay duoc write chi co 1 thao tac, nhung read thi se co nhieu thao tac hon, do do co the write nhanh hon read
// -> pipe co the se bi day

    int pid = fork();
	if (pid == 0) {
		for (int i = FIRST_PRIME; i <= LAST_PRIME; i++) {
			// Tranh truong hop pipe bi day.
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
		// Doc pipe de in ra so nguyen to va goi fork.
		while (read(in_pipe, &prime, sizeof(int))) {
			printf("prime %d\n", prime); 
			in_pipe = process(in_pipe, prime);
		}

		wait(0);
		exit(0);
	}
}
