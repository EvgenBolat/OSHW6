#include <iostream>
#include <stdio.h>
#include <unistd.h>
#include <sys/shm.h>
#include <stdlib.h>
#include <signal.h>

int *share;

int *share2;

int shm_id;

int shm_id2;

void ExitProgram(int sigino)
{
  shmdt(share);
  if (shmctl(shm_id, IPC_RMID, (struct shmid_ds *)0) < 0)
  {
    printf("server: shared memory remove error");
  }
  *share2 = -1;
  sleep(1);
  shmdt(share2);
  if (shmctl(shm_id2, IPC_RMID, (struct shmid_ds *)0) < 0)
  {
    printf("server: shared memory remove error");
  }
  printf("\nВыход корректно выполнен");
  exit(0);
}

int main()
{
  (void)signal(SIGINT, ExitProgram);

  shm_id = shmget(0x2FF, getpagesize(), 0666 | IPC_CREAT);
  if (shm_id == -1)
  {
    printf("shmget()");
    exit(1);
  }

  shm_id2 = shmget(0x300, getpagesize(), 0666 | IPC_CREAT);
  printf("shm_id2 = %d\n", shm_id2);
  if (shm_id2 < 0)
  {
    perror("shmget()");
    exit(1);
  }

  share = (int *)shmat(shm_id, 0, 0);
  if (share == NULL)
  {
    printf("shmat()");
    exit(2);
  }

  share2 = (int *)shmat(shm_id2, 0, 0);
  if (share2 == NULL)
  {
    perror("shmat()");
    exit(2);
  }
  printf("share2 = %p\n", share2);

  while (1)
  {
    if (*share2 == -1)
    {
      *share2 = 0;
      printf("\nПрограмма завершена успешно");
      break;
    }
    
    sleep(1);
    printf("%d\n", *share);
  }

  return 0;
}
