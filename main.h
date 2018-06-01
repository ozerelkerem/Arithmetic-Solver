//stacks start
#define MAX_STACK_LENGTH 100

//Stack for the storage characters
typedef struct stack_c
{
	int top;	
	char S[MAX_STACK_LENGTH];
}STACK_c;

//Stack for the storage integers
typedef struct stack_i
{
	int top;	
	int S[MAX_STACK_LENGTH];
}STACK_i;


/*Functions signatures start*/
/*stack functions start*/
char popFromStack_c(STACK_c *s);
void pushToStack_c(STACK_c *s, char val);
int popFromStack_i(STACK_i *s);
void pushToStack_i(STACK_i *s, int val);
int isSTACK_cEmpty(STACK_c *s);
int isSTACK_iEmpty(STACK_i *s);
int isSTACK_cFull(STACK_c *s);
int isSTACK_iFull(STACK_i *s);

void printStack_i(STACK_i *s);
void printStack_c(STACK_c *s);

/*stack functions end*/
//stack end

//arithmetic solver start

#define MAX_INPUT_LENGTH 200
#define MAX_SYMBOL 10

//Struct for the storage characters value
typedef struct values
{
	char symbol;
	float value;
}Values;





void insertChar(char *s,char ss);
void insertString(char *s,char *ss);



char *processInputLine(char *line); // infix to postfix
int solvePostfix(Values *l,char *postfix); // postfix to result

int isArithmeticSign(char *s);// return 1 if the string is +,-,*,/,(,)
int isNumber(char *s);
int getPriority(char c);

float getValues(Values *l,char sym); // get a value from list 
void setValues(Values *l,char sym,float val); // add a value to list ( a = 5 + 2 ;) so storage  a = 7;

/*Functions signatures end*/
//arithmetic solver end


//other functions

FILE *readFile(char *filePath);
