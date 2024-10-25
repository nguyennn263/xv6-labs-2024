#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/param.h"

int is_blank(char ch) {
    return (ch == ' ' || ch == '\t');
} 

int main(int argc, char *argv[]) {
	char buffer[2222];
    char *p_buffer = buffer;
    char ch;
	char *arg[MAXARG];
	int num_arg = argc-1;
	int blank = 0;
	int len = 0;

	if(argc < 2){
		fprintf(2, "Thieu tham so: xargs <command> arg...");
		exit(1);
	}

	for (int i = 1; i <= num_arg; i++) {
		arg[i-1] = argv[i];
	}

    // Doc lan luot ki tu
	while (read(0, &ch, 1) > 0) {
        if(is_blank(ch)){
            blank = 1;
            continue;
        }

        // Them 1 argument moi neu truoc do co blank
        if(blank){
			buffer[len++] = '\0';

			arg[num_arg++] = p_buffer;
			p_buffer = buffer + len;
			blank = 0;
        }

        // Neu xuong dong thi phai fork va thuc thi lenh
		if (ch != '\n') {
			buffer[len++] = ch;
		} 
        else {
			arg[num_arg++] = p_buffer;
			p_buffer = buffer + len;

            int pid = fork();
			if (pid == 0) {
                exec(arg[0], arg);
				exit(0);
			}
			wait(0);
			num_arg = argc - 1;
		}
	}

	exit(0);
}