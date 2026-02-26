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


/*
    Method to create a virtual machine by 
    takes in parameter a program which is  just a set of 
    instructions and the size of the program
*/
VM *virtualMachine()
{
    VM * vm;
    //Program *pp = NULL;
    // assert((pr) && (progSZ));

    int16 size;

    size = $2 sizeof(struct s_vm);
    vm = (VM *)calloc(1, $i size);
    
    if(!vm)
    {
        errno = ErrMem;
        return (VM *)0;
    }

    /* zero($1 vm, size);
    pp = (Program*)malloc($i progSZ);
    if(!pp){
        free(vm);
        errno = ErrMem;
        return (VM *)0;
    }

    copy($1 pp, $1 pr, progSZ);*/

    return vm;
}

Program* exempleProgram(VM* vm)
{
    Program *p = NULL;
    Instruction *i1, *i2;
    Args *a1 = NULL;
    int16 s1, s2, sa1;

    s1 = map(mov);
    s2 = map(nop);

    i1 = (Instruction* )calloc(1, $i s1);
    i2 = (Instruction* )calloc(1, $i s2);
    assert(i1 && i2);

    i1->o = mov;
    sa1 = s1 - 1;
    if(s1){
        a1 = (Args*)calloc(1, $i sa1);
        assert(a1);

        *a1 = 0x00;
        *(a1+1) = 0x05;
        
        // 0000 0001 mov ax
        // 0000 0000
        // 0000 0005 mov ax, 0X05
    }

    
    p = vm->m;
    copy($1 p, $1 i1, 1);
    p++;

    if(a1){
        copy($1 p, $1 a1, sa1);
        p+=sa1;
    }

    i2->o = nop;
    copy($1 p, $1 i2, 1);
    
    free(i1);
    free(i2);

    vm->b = s1+s2+sa1;

    return vm->m;
}

void error(VM* vm, ErrorCode e){

    if(vm)
        free(vm);
    switch(e)
    {
        case e->segv:
            fprintf(stderr, "%s\n", "VM Error: Segmentation Fault");
            break;
        default:
            fprintf(stderr, "%s\n", "VM Error");
            break;
    }

    exit(-1);
}

void evecute(VM* vm){
    Program *pp;

    assert(vm && vm->m);
    pp = vm -> m;

    while((*pp != (Opcode)hlt) && (pp <= vm -> b))
    {

    }

    if(pp > vm -> b)
        error(vm);
}



int main(int argc, char *argv[]) {
    VM *vm; 
    vm = virtualMachine();

    exempleProgram(vm);      
    
    printf("VM = %p \n", vm);
    printf("P = %p \n", vm->m);

    printhex($1 vm->m, map(mov)+map(nop), ' ');

    return 0;
}

