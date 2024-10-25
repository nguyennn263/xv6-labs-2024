#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(void) {
    int p1[2]; // Tạo pipe từ parent -> child
    int p2[2]; // Tạo pipe từ child -> parent
    char buf[1];
    
    if (pipe(p1) < 0 || pipe(p2) < 0) {
        fprintf(2, "pipe failed\n");
        exit(1);
    }

    int pid = fork();
    if (pid < 0) {
        fprintf(2, "fork failed\n");
        exit(1);
    }

    if (pid == 0) { // child process
        close(p1[1]); // đóng write end của p1
        close(p2[0]); // đóng read end của p2
        
        // Đọc byte
        if (read(p1[0], buf, 1) != 1) {
            fprintf(2, "child read failed\n");
            exit(1);
        }
        printf("%d: received ping\n", getpid());

        // Gửi byte
        if (write(p2[1], buf, 1) != 1) {
            fprintf(2, "child write failed\n");
            exit(1);
        }

        close(p1[0]);
        close(p2[1]);
        exit(0);
    } else { // parent process
        close(p1[0]); 
        close(p2[1]);

        // Gửi byte
        if (write(p1[1], "a", 1) != 1) {
            fprintf(2, "parent write failed\n");
            exit(1);
        }

        // Đọc byte
        if (read(p2[0], buf, 1) != 1) {
            fprintf(2, "parent read failed\n");
            exit(1);
        }
        printf("%d: received pong\n", getpid());

        close(p1[1]);
        close(p2[0]);
        wait(0); // đợi child process kết thúc
        exit(0);
    }
}
