#include <iostream>
#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <sys/shm.h>
#include <stdlib.h>
#include <time.h>

int *share;

pid_t pid;

void ExitProgram(int sigino)
{
  shmdt(share);
  printf("\nВыход корректно выполнен");
  kill(pid, SIGINT);
  exit(0);
}

int main()
{
  (void)signal(SIGINT, ExitProgram);
  
  int shm_id;
  int num;

  pid_t own_pid = getpid();

  std::cout << "Мой pid: " << own_pid << "\n";
  printf("Введите pid программы собеседника (сервера): ");

  std::cin >> pid;

  srand(time(NULL));
  shm_id = shmget(0x2FF, getpagesize(), 0666 | IPC_CREAT);
  printf("shm_id = %d\n", shm_id);
  if (shm_id < 0)
  {
    perror("shmget()");
    exit(1);
  }

  /* подключение сегмента к адресному пространству процесса */
  share = (int *)shmat(shm_id, 0, 0);
  if (share == NULL)
  {
    perror("shmat()");
    exit(2);
  }
  printf("share = %p\n", share);

  while (1)
  {
    num = random() % 1000;
    *share = num;
    printf("write a random number %d\n", num);
    sleep(1);
  }
  return 0;
}
