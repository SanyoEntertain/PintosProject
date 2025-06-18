#include "userprog/syscall.h"
#include <stdio.h>
#include <syscall-nr.h>
#include "threads/interrupt.h"
#include "threads/thread.h"

#include "threads/vaddr.h"
#include "devices/shutdown.h"

static void syscall_handler (struct intr_frame *);

void
syscall_init (void) 
{
  intr_register_int (0x30, 3, INTR_ON, syscall_handler, "syscall");
}

static void
syscall_handler (struct intr_frame *f UNUSED) 
{
  printf ("system call!\n");
  // esp가 유효한지 검사한다.
  if(!is_user_vaddr(f->esp)){
    thread_exit();
    return;
  }
    
  // number를 받아온다.
  int number = *(int*)f->esp;
  printf("syscall number: %d\n", number);
  // halt만 인자 없음.
  if(number == SYS_HALT){
    halt();
    return;
  }
  // +4 의 추가 인자 유효 검사
  void* up_esp = f->esp+4;
  if(!is_user_vaddr(up_esp)){
    thread_exit();
    return;
  }
  switch(number){
    case SYS_EXIT:
      exit(*(int*)up_esp);
      break;
    case SYS_EXEC:
      exec(*(char**)up_esp);
      break;
    case SYS_WAIT:
      wait(*(pid_t*)up_esp);
      break;
  }
}

// system call functions
void halt(void){
  shutdown_power_off();
}

void exit(int status){
  struct thread *cur = thread_current();
  cur->status = status;
  printf("%s: exit(%d)\n", cur->name, status);
  thread_exit();
}

pid_t exec(const char *cmp_line){

}

int wait(pid_t pid){

}

