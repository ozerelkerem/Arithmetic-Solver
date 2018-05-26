#include<stdio.h>

#define MAX_STACK_LENGTH 100
#define MAX_INPUT_LENGTH 200
#define MAX_SYMBOL 10

typedef struct values
{
	char symbol;
	float value;
}Values;

typedef struct stack_c
{
	int top;	
	char S[MAX_STACK_LENGTH];
}STACK_c;
typedef struct stack_i
{
	int top;	
	int S[MAX_STACK_LENGTH];
}STACK_i;

char popFromStack_c(STACK_c *s);
void pushToStack_c(STACK_c *s, char val);
int popFromStack_i(STACK_i *s);
void pushToStack_i(STACK_i *s, int val);
int isSTACK_cEmpty(STACK_c *s);
int isSTACK_iEmpty(STACK_i *s);
int isSTACK_cFull(STACK_c *s);
int isSTACK_iFull(STACK_i *s);

int solvePostfix(Values *l,char *postfix);

void insertChar(char *s,char ss);
void insertString(char *s,char *ss);



char *processInputLine(char *line,char *l,char **);
int isNumber(char *s);
int getPriority(char c);

float getValues(Values *l,char sym);
void setValues(Values *l,char sym,float val);

int main()
{
	Values vals[MAX_SYMBOL];
	memset(vals,NULL,sizeof(Values)*MAX_SYMBOL);
	puts("Please give inputs =");
	char Input[MAX_INPUT_LENGTH];
	char *postfix;
	char left;
	int finish = 0;
	do
	{
		puts("\nGive Line :");
		gets(Input);
	
		if(Input[0])
		{
			processInputLine(Input,&left,&postfix);
			printf("Prefix Form : %s \n", postfix);
			int c = solvePostfix(vals,postfix);
			printf("Result : %d", c);
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
					pushToStack_i(&stack,x-y);
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
	int xxx = (int)popFromStack_i(&stack);
	setValues(l,left,xxx);
	return  getValues(l,left);
}

char *processInputLine(char *line,char *l,char **p) // return postfixform
{
	char *postfix = (char *)malloc(MAX_INPUT_LENGTH);
	char popped;
	postfix[0] = 0;
	//create a stack
	STACK_c stack;
	stack.top=0;
	
	char *token = strtok(line, " ");
	char left = token[0];
	*l = left;
	insertString(postfix,token);
	token =  strtok(NULL, " "); // pass left value
	insertString(postfix,token);
	token =  strtok(NULL, " "); // pass =
	while (token[0] != ';')
    {
       	if(!isArithmeticSign(token))
       		insertString(postfix,token);
       	else
       	{
			switch(token[0])
			{
				case '(':{
						pushToStack_c(&stack,'(');
					}break;
					
				case ')':{
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
			token =  strtok(NULL, " ");	
		}
        
    while(!isSTACK_cEmpty(&stack))
    {
    	popped = popFromStack_c(&stack);
    	insertChar(postfix,popped);
	}
	insertString(postfix,";");
    *p = postfix;
    
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
		if(l[i].symbol == 0)
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



