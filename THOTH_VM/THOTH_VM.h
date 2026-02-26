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

#define ErrMem 0x01
#define NoArgs {0x00, 0x00}

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

/**
    16 bit virtual machine
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
typedef int8 Args;

typedef int8 Memory[((int16)(-1))];
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

void error(VM* vm, ErrorCode e);
void evecute(VM*);
    Program *exempleProgram(VM* vm); 
int8 map(Opcode);
VM *virtualMachine();

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