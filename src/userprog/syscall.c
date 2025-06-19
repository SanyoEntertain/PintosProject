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
      f->eax = exec(*(char**)up_esp);
      break;
    case SYS_WAIT:
      f->eax = wait(*(pid_t*)up_esp);
      break;
  }
}

// system call functions
void halt(void){
  shutdown_power_off();
}

void exit(int status){
  struct thread *cur = thread_current();
  cur->exit_status = status;
  sema_up(&cur->sem_wait);
  printf("%s: exit(%d)\n", cur->name, cur->exit_status);
  thread_exit();
}

// cmd_line 읽고, page 만들고, copy, 
pid_t exec(const char *cmd_line){
  pid_t tid = process_execute(cmd_line);
  if(tid == TID_ERROR)
    // child를 없앤다.
    return -1;
  // current가 sema_down을 해야 한다.
  struct thread* cur = thread_current();
  struct thread* child;

  struct list_elem *e;
  bool isFound = false;
  for(e = list_begin(&cur->child_list); e != list_end(&cur->child_list);
      e = list_next(e))
      {
        struct thread *t = list_entry(e, struct thread, elem);
        if (t == NULL) continue;
        if(t->tid == tid){
          child = t;
          isFound = true;
          break;
        }
      }
  // 못 찾으면 fail.
  if (!isFound){
    return -1;
  }
  // 이 스레드를 현재 스레드의 자식으로 만들기.
  child->parent = cur;
  list_push_back(&cur->child_list, &child->elem);

  sema_down(&child->sem_exec);
  if (child->load_status == -1){
    return -1;
  }
  return tid;
}

int wait(pid_t pid){
  return process_wait(pid);
}

