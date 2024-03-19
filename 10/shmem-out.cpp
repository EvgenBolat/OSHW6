#include <iostream>
#include <stdio.h>
#include <unistd.h>
#include <sys/shm.h>
#include <stdlib.h>
#include <signal.h>

int *share;
int shm_id;

pid_t pid = -1;

void ExitProgram(int sigino)
{
  shmdt(share);
  if (shmctl(shm_id, IPC_RMID, (struct shmid_ds *)0) < 0)
  {
    printf("server: shared memory remove error");
  }
  printf("\nВыход корректно выполнен");
  kill(pid, SIGINT);
  exit(0);
}

int main()
{
  (void)signal(SIGINT, ExitProgram);

  pid_t own_pid = getpid();

  std::cout << "Мой pid: " << own_pid << "\n";
  
  printf("Введите pid программы собеседника (клиента): ");

  std::cin >> pid;

  shm_id = shmget(0x2FF, getpagesize(), 0666 | IPC_CREAT);
  if (shm_id == -1)
  {
    printf("shmget()");
    exit(1);
  }

  share = (int *)shmat(shm_id, 0, 0);
  if (share == NULL)
  {
    printf("shmat()");
    exit(2);
  }

  while (1)
  {
    sleep(1);
    printf("%d\n", *share);
  }

  return 0;
}
