DEF_CMD (PUSH, 4, 1, YES, 
{
    MY_ASSERT (1, "the command was not detected earlier");
})
DEF_CMD (POP,  3, 2,  YES,
{
    MY_ASSERT (1, "the command was not detected earlier");
})
DEF_CMD (ADD,  3, 3,  NO,
{
    int n1 = stack_pop (&stk, logfile);
    int n2 = stack_pop (&stk, logfile);
    stack_push (&stk, n1 + n2, logfile);
})
DEF_CMD (SUB,  3, 4,  NO,
{
    int n1 = stack_pop (&stk, logfile);
    int n2 = stack_pop (&stk, logfile);
    stack_push (&stk, n2 - n1, logfile);
})
DEF_CMD (MUL,  3, 5,  NO,
{
    int n1 = stack_pop (&stk, logfile);
    int n2 = stack_pop (&stk, logfile);
    stack_push (&stk, (int)(((double)n1/1000 * (double)n2/1000)*1000), logfile);
})
DEF_CMD (DIV,  3, 6,  NO,
{
    int n1 = stack_pop (&stk, logfile);
    int n2 = stack_pop (&stk, logfile);
    stack_push (&stk, (int)(((double)n2 / (double)n1)*1000), logfile);
})
DEF_CMD (OUT,  3, 7,  NO,
{
    fprintf (stdout, "OUT: %.3f\n", ((double)stack_pop (&stk, logfile))/1000);
})
DEF_CMD (IN,   2, 8,  NO,
{
    int tmp = getNum ();
    stack_push (&stk, tmp, logfile);
    stack_dump (stk, logfile);
})
DEF_CMD (JMP,  3, 9,  YES,
{
    i++;
    int ptrToJmp = code[i];
    i = ((size_t) ptrToJmp)-1;
})
DEF_CMD (CALL, 4, 10, YES,
{
    i++;
    int ptrToJmp = code[i];
    stack_push (&callStack, ((int)i) + 1, logCallStack);
    i = (size_t) ptrToJmp-1;
})
DEF_CMD (RET,  3, 11, NO,
{
    i = (size_t) stack_pop (&callStack, logCallStack) - 1;
})
DEF_CMD (JA,   2, 12, YES,
{
    JUMP_FORM (>)
})
DEF_CMD (JB,   2, 13, YES,
{
    JUMP_FORM (<)
})
DEF_CMD (JBE,  3, 14, YES,
{
    JUMP_FORM (<=)
})
DEF_CMD (JGE,  3, 15, YES,
{
    JUMP_FORM (>=)
})
DEF_CMD (JE,   2, 16, YES,
{
    JUMP_FORM (==)
})
DEF_CMD (JNE,  3, 17, YES,
{
    JUMP_FORM (!=)
})
DEF_CMD (HLT,  3, 18, NO,
{
    break;
})
DEF_CMD (SQRT, 4, 19, NO,
{
    int tmp = stack_pop (&stk, logfile);
    int result = (int) (sqrt ((double)tmp/1000) * 1000);
    stack_push (&stk, result, logfile);
})
DEF_CMD (MEOW, 4, 20, NO,
{
    fprintf (stdout, "meow\n");
})
DEF_CMD (BA, 2, 21, NO,
{
    BOOL_EXPR (>)
})
DEF_CMD (BB, 2, 22, NO,
{
    BOOL_EXPR (<)
})
DEF_CMD (BBE, 3, 23, NO,
{
    BOOL_EXPR (<=)
})
DEF_CMD (BGE, 3, 24, NO,
{
    BOOL_EXPR (>=)
})
DEF_CMD (BE, 3, 25, NO,
{
    BOOL_EXPR (==)
})
DEF_CMD (BNE, 3, 26, NO,
{
    BOOL_EXPR (!=)
})
