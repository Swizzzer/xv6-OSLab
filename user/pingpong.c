#include "kernel/types.h"
#include "user/user.h"

int main(int argc, char *argv[]) {
  int parent_fd[2], child_fd[2];
  pipe(parent_fd);
  pipe(child_fd);
  char buf[64];

  if (fork()) {
    // Parent
    close(parent_fd[0]);
    write(parent_fd[1], "ping", strlen("ping"));
    close(parent_fd[1]);
    wait(0);
    read(child_fd[0], buf, 4);
    close(child_fd[0]);
    printf("%d: received %s\n", getpid(), buf);
  } else {
    // Child
    close(parent_fd[1]);
    read(parent_fd[0], buf, 4);
    close(parent_fd[0]);
    printf("%d: received %s\n", getpid(), buf);
    close(child_fd[0]);
    write(child_fd[1], "pong", strlen("pong"));
    close(child_fd[1]);
  }

  exit(0);
}