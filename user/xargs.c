#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/param.h"  // MAXARG

int is_blank(char ch) {
    return (ch == ' ' || ch == '\t');
} 

int main(int argc, char *argv[]) {
	char buffer[2048];
    char *p_buffer = buffer;
    char ch;
	char *arg[MAXARG];
	int num_arg = argc-1;
	int blank = 0;
	int len = 0;


	if(argc <= 1){
		fprintf(2, "Thieu tham so");
		exit(1);
	}

	for (int i = 1; i <= num_arg; i++) {
		arg[i-1] = argv[i];
	}

	while (read(0, &ch, 1) > 0) {
        if(is_blank(ch)){
            blank = 1;
            continue;
        } 

        if(blank){
			buffer[len++] = '\0';

			arg[num_arg++] = p_buffer;
			p_buffer = buffer + len;
			blank = 0;
        }

		if (ch != '\n') {
			buffer[len++] = ch;
		} 
        else {
			arg[num_arg++] = p_buffer;
			p_buffer = buffer + len;

            int pid = fork();
			if (pid == 0) {
				exit(exec(arg[0], arg));
			}
			wait(0);
			
			num_arg = argc - 1;

		}
	}

	exit(0);
}