#include<stdio.h>

#define MAX_STACK_LENGTH 100
#define MAX_INPUT_LENGTH 200
#define MAX_SYMBOL 10

//Struct for the storage characters value
typedef struct values
{
	char symbol;
	float value;
}Values;


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
/*stack functions end*/


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

int main()
{
	Values vals[MAX_SYMBOL]={0};
	
	puts("Please give inputs :\n");
	char Input[MAX_INPUT_LENGTH];
	
	char *postfix;
	
	int finish = 0;
	do
	{
		puts("\nInfix Form:");
		gets(Input);
	
		if(Input[0])
		{
			postfix = processInputLine(Input);
			printf("Postfix Form :\n%s \n", postfix);
			int c = solvePostfix(vals,postfix);
			free(postfix);
			printf("Result : %d\n", c);
		}
		else
			finish = 1;
	}while(!finish);
	
	
	
	
	return 0;
}

int solvePostfix(Values *l,char *postfix)
{
	//char *postfix = (char *)malloc(MAX_INPUT_LENGTH);
	int popped;
	int x,y;
	//create a stack
	STACK_i stack;
	stack.top=0;
	
	char *token = strtok(postfix, " ");
	char left = token[0];
	
	token =  strtok(NULL, " "); // pass left value
	token =  strtok(NULL, " "); // pass =
	while (token[0] != ';')
	{
		if(isNumber(token))
			pushToStack_i(&stack,atoi(token));
		else if(isArithmeticSign(token))
		{
			x=popFromStack_i(&stack);
			y=popFromStack_i(&stack);
			switch(token[0])
			{
				case '+':
					pushToStack_i(&stack,x+y);
					break;
				case '/':
					pushToStack_i(&stack,y/x);
					break;
				case '-':
					pushToStack_i(&stack,y-x);
					break;
				case '*':
					pushToStack_i(&stack,x*y);
					break;
			}
		}
		else
		{
			pushToStack_i(&stack,getValues(l,token[0]));
		}
		token =  strtok(NULL, " ");
	}
	setValues(l,left,(int)popFromStack_i(&stack)); // we find the result , result is the last integer at the stack
	return getValues(l,left);
}

char *processInputLine(char *line) // return postfixform
{
	char *postfix = (char *)malloc(MAX_INPUT_LENGTH);
	postfix[0] = 0;
	
	char popped;
	
	
	STACK_c stack; //create a stack
	stack.top=0; // set size = 0
	
	char *token = strtok(line, " ");
	
	insertString(postfix,token); // first character  before '='
	token =  strtok(NULL, " "); // pass left value
	insertString(postfix,token); // insert '='
	token =  strtok(NULL, " "); // pass =
	
	while (token[0] != ';') // read line until see the ';' character
    {
       	if(!isArithmeticSign(token))
       		insertString(postfix,token); //insert to postfix if char is a number
       	else
       	{
			switch(token[0])
			{
				case '(':{
						pushToStack_c(&stack,'(');
					}break;
					
				case ')':{ // if read value ')' pop until see the '('
						do
						{
							popped = popFromStack_c(&stack);
							if(popped!='(')
								insertChar(postfix,popped);
						}while(popped!='(');
					}break;
				default:{
					if(isSTACK_cEmpty(&stack))
						pushToStack_c(&stack,token[0]);
					else
					{
						popped = popFromStack_c(&stack);
						pushToStack_c(&stack,popped);
						if(getPriority(token[0]) > getPriority(popped))
							pushToStack_c(&stack,token[0]);
						else
						{	
							do
							{	
								popped = popFromStack_c(&stack);
								if(getPriority(token[0]) <= getPriority(popped))
									insertChar(postfix,popped);
								else
									pushToStack_c(&stack,popped);
							}while((getPriority(token[0]) <= getPriority(popped)) && !isSTACK_cEmpty(&stack));
							pushToStack_c(&stack,token[0]);
						}
					}
					
					}break;
				}			
			}	
			token =  strtok(NULL, " ");	// next value
		}
		
    /*insert after all line read*/
    while(!isSTACK_cEmpty(&stack))
    {
    	popped = popFromStack_c(&stack);
    	insertChar(postfix,popped);
	}
	insertString(postfix,";");
    return postfix;
    
}

int isNumber(char *s)
{
	if(strlen(s) > 1)
		return 1;
	else
	{
		if(s[0] >='0' && s[0] <= '9')
			return 1;
		else
			return 0;
	}
}

int isArithmeticSign(char *s)
{
	if(isNumber(s))
		return 0;
	else
		if(s[0] == '+' || s[0] == '-' || s[0] == '/' || s[0] == '*' || s[0] == '(' || s[0] == ')')
			return 1;
		else	
			return 0;
}

float getValues(Values *l,char sym)
{
	int i;
	for(i=0;i<MAX_SYMBOL;i++)
	{
		if(l[i].symbol == sym)
			return l[i].value;
	}
		return 0;
}

void setValues(Values *l,char sym,float val)
{
		int i;
	for(i=0;i<MAX_SYMBOL;i++)
	{
		if(l[i].symbol == 0 || l[i].symbol == sym) // change value or insert new
		{
			l[i].symbol = sym;
			l[i].value = val;
			return;
		}	
	}
}

int getPriority(char c)
{
	switch(c)
	{
		case '+':
			return 0;
			break;
		case '-':
			return 0;
			break;
		case '/':
			return 1;
			break;
		case '*':
			return 1;
			break;
		case '(':
			return -1;
			break;
	}
}


void insertString(char *s,char *ss)
{
	sprintf(s,"%s %s",s,ss);
}
void insertChar(char *s,char ss)
{
	sprintf(s,"%s %c",s,ss);
}


char popFromStack_c(STACK_c *s)
{
	if(isSTACK_cEmpty(s))
	{
		puts("Stack is empty, you can not pop");
		exit(1);
	}
	else
	{
		s->top--;
		return s->S[s->top];
	}
}

void pushToStack_c(STACK_c *s, char val)
{
	if(isSTACK_cFull(s))
	{
		puts("Stack is full, you can not push");
		exit(1);
	}
	else
	{
		s->S[s->top] = val;
		s->top++;
	}
}

int popFromStack_i(STACK_i *s)
{
	if(isSTACK_iEmpty(s))
	{
		puts("Stack is empty, you can not pop");
		exit(1);
	}
	else
	{
		s->top--;
		return s->S[s->top];
	}
}

void pushToStack_i(STACK_i *s, int val)
{
	if(isSTACK_iFull(s))
	{
		puts("Stack is full, you can not push");
		exit(1);
	}
	else
	{
		s->S[s->top] = val;
		s->top++;
	}
}

int isSTACK_cEmpty(STACK_c *s)
{
	return (!s->top) ? 1 : 0;
}
int isSTACK_iEmpty(STACK_i *s)
{
	return (!s->top) ? 1 : 0;
}
int isSTACK_cFull(STACK_c *s)
{
	return (s->top == MAX_STACK_LENGTH) ? 1 : 0;
}
int isSTACK_iFull(STACK_i *s)
{
	return (s->top == MAX_STACK_LENGTH) ? 1 : 0;
}



