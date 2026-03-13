/* THOTH_VM.c */
#include "THOTH_VM.h"

/*
    start from the IM size, then iterate on each instruction to correspond it
    to the correct op code and return it instruction set
*/

int8 map(Opcode o){
    int8 n, ret;
    IM *p;

    ret = 0;
    for(n=IMs, p=instrmap; n; n--, p++)
    
        if(p->o == o){
            ret = p->size;
            break;
        }
    

    return ret;
}

void MOV(VM *vm, Opcode o, Args a0, Args a1){
    vm $ax = (Reg)a1;
    return;
}


void execi(VM* vm, Program *pp)
{
     Args a1, a2;
     a1=a2=0;
     printf("i.o = %d, i.a[0] = %d, i.a[1] = %d, i.a[2] = %d\n", *pp, *(pp+1), *(pp+2), *(pp+3));
     int16 size = map(*pp);
     switch (size)
     {
        case 1:
            break;
        case 2:
            a1 = *(pp+1);
            break;
        case 3:
            a1 = *(pp+2);
            a2 = *(pp+3);
            break;
        default:
            segfault(vm);
            break;
     }

     switch (*pp)
     {
        case mov:
            MOV(vm, *pp, a1, a2);
            break;
        case nop:
            break;
        case hlt:
            error(vm, SysHlt);
        break;
    }

    return;
}


void error(VM* vm, ErrorCode e){
    int exitcode;

    switch(e)
    {
        case ErrSegv:
            fprintf(stderr, "%s\n", "VM Error: Segmentation Fault");
            exitcode=-1;
            break;
        case SysHlt:
            printf("ax = %.08hx \n", $i vm $ax);
            fprintf(stderr, "%s\n", "System halted");
            exitcode = 0;
            break;
        default:
            fprintf(stderr, "%s\n", "VM Error");
            exitcode = -1;
            break;
    }

    if(vm)
        free(vm);

    exit(exitcode);
}

VM *virtualMachine()
{
    VM * vm;
    int16 size;

    size = $2 sizeof(struct s_vm);
    vm = (VM *)calloc(1, $i size);
     
    if(!vm)
    {
        errno = ErrMem;
        return (VM *)0;
    }

    return vm;
}

int destroyVirtualMachine(VM *vm){
    free(vm);
    return 0;
}


Program* exampleProgram(VM* vm)
{
    Program *p = NULL;
    Instruction *i1 = NULL, *i2 = NULL, *i3 = NULL;
    Args a1=0;
    int16 s1, s2, s3, sa1;

    s1 = map(mov);
    s2 = map(nop);
    s3 = map(hlt);

    i1 = (Instruction* )calloc(1, $i s1);
    i2 = (Instruction* )calloc(1, $i s2);
    i3 = (Instruction* )calloc(1, $i s3);
    assert(i1 && i2 && i3);
    p = vm->m;

    i1->o = mov; 
    sa1 = s1 - 1;
    a1 = 0x0005;
        
    // 0000 0001 mov ax
    // 0000 0000
    // 0000 0005 mov ax, 0X05

        // fprintf(stdout, "vm -> m = %p *vm -> m = %d \n", vm -> m, *vm -> m);
        // fprintf(stdout, "p = %p *p = %d \n", p, *p);

    copy($1 p, $1 i1, 1); // mov ax
    p++;
        // fprintf(stdout, "p = %p *p = %d \n", p, *p);

    if(a1){
        copy($1 p, $1 &a1, sa1); // mov ax,5
        p+=sa1;
    }

    i2->o = nop;
    copy($1 p, $1 i2, 1);
    // fprintf(stdout, "p = %p *p = %d \n", p, *p);

    p++;
    i3->o = hlt;
    copy($1 p, $1 i3, 1);

    vm->b = s1+s2+s3+sa1;
    vm $ip = (Reg)vm->m;
    vm $sp = (Reg)-1;

    free(i1);
    free(i2);
    free(i3);
    fprintf(stderr, "p is %p, vm-> is %p\n", p, (vm->m));


    return vm->m;
}


void execute(VM* vm){
    int16 brkaddr;
    Program *pp=NULL;
    int16 size=0;
    //Instruction *ip=NULL; //local instruction pointer
    Instruction ip;
    assert(vm && *vm->m);
    brkaddr = ((int16)vm->m + vm->b);
    pp = (Program *)&vm -> m;
    fprintf(stdout, "vm -> m = %p *vm -> m = %d \n", vm -> m, *vm -> m);

    /**
     * instruction1 arg1 instruction2 arg2
     * mov ax,0x05; nop; hlt;
     * 0x01  0x0 0x05; <- pp points here
     * 0X02
     * 0X03
     */

    do{
        vm $ip+=size;
        pp += size;
      
        // static int i = 0;
        // printf("I = %d\n", i++);
        // fprintf(stdout, "pp = %p *pp = %d \n", pp, *pp);


        if((int16)pp > (int16)brkaddr) 
        {
            segfault(vm);  
        }

        //ip = pp;
        size = map(*pp);
        execi(vm, pp);

        // size = map(ip->o);
        // execi(vm, ip, size);
  
        // fprintf(stderr, "pp is %p\n", pp);
        // fprintf(stderr, "pp+1 is %p\n", pp+1);
        // fprintf(stderr, "pp+2 is %p\n", pp+2);
        // fprintf(stderr, "pp+3 is %p\n", pp+3);

        // fprintf(stderr, "Ip is %d\n", sizeof(ip->o));

        //fprintf(stderr, "Ip is %d\n", sizeof(ip.o));
       
     }while((*pp != (Opcode)hlt));    

}





int main(int argc, char *argv[]) {
    VM *vm; 
    Program *Prog;

    vm = virtualMachine();

    exampleProgram(vm);      
    
    // printf("VM = %p \n", vm);
    // printf("P = %p \n", vm->m);
    execute(vm);

    printhex($1 vm->m, map(mov)+map(nop), ' ');

    return 0;
}




// TODO
// %x ?
// %h ?
// %hx ?
// %hhx ?