/* THOTH_VM.h */
#define _GNU_SOURCE
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>
#include <errno.h>
#include <birchutils.h>

#define VM_MEM ((int16)(-1))

#define NoErr 0x00 // 00 00
#define SysHlt 0x01 // 00 01
#define ErrMem 0x02 // 00 10
#define ErrSegv 0x03 // 00 11

typedef unsigned char int8;
typedef unsigned short int int16;
typedef unsigned int int32;
typedef unsigned long long int int64;

#define $1 (int8 *)
#define $2 (int16)
#define $4 (int32)
#define $8 (int64)
#define $c (char *)
#define $i (int)


#define $ax ->c.r.ax
#define $bx ->c.r.bx
#define $cx ->c.r.cx
#define $dx ->c.r.dx
#define $sp ->c.r.sp
#define $ip ->c.r.ip


#define segfault(vm) error(vm, ErrSegv)

/**
    16 bit virtual machineb
        (65536 mem adresses)
    
    The Registers:
        AX
        BX
        CX
        DX

        SP
        IP

    Memory 65KB

    Serial COM Port
    Floppy Drive

 */

typedef unsigned short int Reg;
typedef int16 Args;

typedef int8 Memory[VM_MEM];
//typedef int8 Memory[((unsigned int)(-1))];

typedef unsigned char ErrorCode;

struct s_registers
{
    Reg ax;
    Reg bx;
    Reg cx;
    Reg dx;

    Reg sp;
    Reg ip;
};
typedef struct s_registers Register;


struct s_cpu{
    Register r;
};
typedef struct s_cpu CPU;

/*

mov ax, 0ax05 // (0x01 OR 0x02)
              // 0000 0011 = mov
              // 0000 0000
              // 0000 0101 = 0x05

*/

enum e_opcode{
    mov = 0X01,
    nop = 0x02,    
    hlt = 0x03,
};

typedef enum e_opcode Opcode;

struct s_instrmap{
    Opcode o;
    int8 size;
};
typedef struct s_instrmap IM;

struct s_instruction{
    Opcode o;
    Args a[]; // 0-2 bytes
};
typedef struct s_instruction Instruction;


typedef int8 Program;


struct s_vm{
    CPU c;
    Memory m; // 65550 addrs
    int16 b; /* break line*/
};

typedef struct  s_vm VM;

typedef Memory *Stack;

static IM instrmap[] = {
    {mov, 0x03},
    {nop, 0x01},
    {hlt, 0x01},

};

#define IMs ( sizeof(instrmap) / (sizeof(struct s_instrmap)) )
void MOV(VM *vm, Opcode o, Args a0, Args a1);



void execi(VM* vm, Program *pp);
void error(VM* vm, ErrorCode e);
void execute(VM* vm);
    Program *exampleProgram(VM* vm); 
int8 map(Opcode o);
VM *virtualMachine();
int destroyVirtualMachine(VM *vm);

int main(int,char**);


/*
Section .text(code)
    
    #########
    #######



    ****
    ******
    *********
    
Section .data
*/