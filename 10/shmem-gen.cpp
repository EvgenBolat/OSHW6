#include <iostream>
#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <sys/shm.h>
#include <stdlib.h>
#include <time.h>

int *share;

int *share2;

void SendExitMessage(int sigino)
{
  *share2 = -1;
  sleep(1);
  printf("\nПрограмма завершена");
  *share2 = 0;
  exit(0);
}

int main()
{
  (void)signal(SIGINT, SendExitMessage);

  int shm_id;

  int shm_id2;
  int num;

  srand(time(NULL));
  shm_id = shmget(0x2FF, getpagesize(), 0666 | IPC_CREAT);
  printf("shm_id = %d\n", shm_id);
  if (shm_id < 0)
  {
    perror("shmget()");
    exit(1);
  }

  shm_id2 = shmget(0x300, getpagesize(), 0666 | IPC_CREAT);
  printf("shm_id2 = %d\n", shm_id2);
  if (shm_id2 < 0)
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


  /* подключение сегмента к адресному пространству процесса */
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
      printf("\nПрограмма завершена");
      *share2 = 0;
      break;
    }
    
    num = random() % 1000;
    *share = num;
    printf("write a random number %d\n", num);
    sleep(1);
  }
  shmdt(share);
  shmdt(share2);
  return 0;
}
