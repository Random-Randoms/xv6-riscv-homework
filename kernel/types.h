typedef unsigned int   uint;
typedef unsigned short ushort;
typedef unsigned char  uchar;

typedef unsigned char uint8;
typedef unsigned short uint16;
typedef unsigned int  uint32;
typedef unsigned long uint64;

typedef uint64 pde_t;

enum pstate { _SLEEPING, _RUNNABLE, _RUNNING, _ZOMBIE };

struct procinfo {
  enum pstate state;
  int pid;
  int ppid;
  char name[16];
};
