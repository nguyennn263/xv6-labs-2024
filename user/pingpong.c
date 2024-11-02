#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main() {
    int p[2]; // Tạo pipe
    char buf[1];
    
    if (pipe(p)<0) {
        fprintf(2, "pipe failed\n");
        exit(1);
    }

    int pid = fork();
    if (pid < 0) {
        fprintf(2, "fork failed\n");
        exit(1);
    }

    if (pid == 0) { // child process
        close(p[1]); // đóng write end của pipe
        
        // Đọc byte
        if (read(p[0], buf, 1) != 1) {
            fprintf(2, "child read failed\n");
            exit(1);
        }
        printf("%d: received ping\n", getpid());

        close(p[0]);
        exit(0);
    } else { // parent process
        close(p[0]); 

        // Gửi byte
        if (write(p[1], "a", 1) != 1) {
            fprintf(2, "parent write failed\n");
            exit(1);
        }

        wait(0); // đợi child process kết thúc

        printf("%d: received pong\n", getpid());

        close(p[1]);
        exit(0);
    }
}