#include<stdio.h>
#include "main.h"

#define TEST 1
#define file "input.txt"


int main()
{
	FILE *f = readFile(file);
	Values vals[MAX_SYMBOL]={0};
	
	
	char Input[MAX_INPUT_LENGTH];
	char *postfix;
	while(fgets(Input,255,f)!=NULL) // get lines
	{
		printf("Infix : %s",Input);
		#ifdef TEST
			printf("--Infix to Postfix Started--\n");
		#endif
		postfix = processInputLine(Input);
		#ifdef TEST
			printf("--Infix to Postfix Finished--\n");
		#endif		
		printf("Postfix Form : %s \n", postfix);
		#ifdef TEST
			printf("--Solving Postfix Started--\n");
		#endif
		int c = solvePostfix(vals,postfix);
		free(postfix);
		#ifdef TEST
			printf("--Solving Postfix Finished--\n");
		#endif
		printf("Result : %d\n\n", c);	
	}
	
	int i;
	printf("All Results :\n");
	for(i=0;i<MAX_SYMBOL;i++)
	{
		if(vals[i].symbol!=0)
			printf("%c:%0.f, ",vals[i].symbol,vals[i].value);
	}
    
	fclose(f);
	
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
		
		
		#ifdef TEST
			printStack_i(&stack);
			printf("\n");
		#endif
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
			
			#ifdef TEST
				printf("postfix : %s \t",postfix);
				printStack_c(&stack);
				printf("\n");
			#endif
			
		}
		
    /*insert after all line read*/
    while(!isSTACK_cEmpty(&stack))
    {
    	popped = popFromStack_c(&stack);
    	insertChar(postfix,popped);
    	
    	#ifdef TEST
			printf("postfix : %s \t",postfix);
			printStack_c(&stack);
			printf("\n");
		#endif
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


FILE *readFile(char *filePath)
{
	FILE *f = fopen(filePath,"r");
	
	if(!f)
	{
		printf("File could not read!");
		exit(1);
	}
	
	return f;
}





void printStack_c(STACK_c *s)
{
	printf("Stack: ");
	int i;
	for(i=0;i<s->top;i++)
		printf("%c",s->S[i]);
}
void printStack_i(STACK_i *s)
{
	printf("Stack: ");
	int i;
	for(i=0;i<s->top;i++)
		printf("%d ",s->S[i]);
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



