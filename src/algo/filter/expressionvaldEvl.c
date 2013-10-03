/**
This is responsible for the Infix to postfix conversion and postfix evaluation of the expression.
*/
#include <stdio.h>
#define MAX_STACK_SIZE 1000

typedef char  stack_element ;


struct stack{
	size_t top;
	stack_element data[MAX_STACK_SIZE];
};

void stack_init (struct stack*);
void stack_push (struct stack*,stack_element);
stack_element stack_pop (struct stack*);
stack_element stack_top (struct stack*);
boolean stack_empty (struct stack*);

void stack_init (struct stack *stack){
	stack->top=-1;
}

void stack_push (struct stack *stack, stack_element elem){
	stack->top++;
	stack->data[stack->top]=elem;
}

stack_element stack_pop (struct stack *stack){
	stack_element result=stack->data[stack->top];
	--stack->top;
	return  result;
}

stack_element stack_top (struct stack* stack){
	if(stack->top==-1){
		return NULL;
	}
	return stack->data[stack->top];
}

boolean stack_empty (struct stack *stack){
	return stack->top==-1;
}
#define isOperand(__ch)    (__ch=='1' || __ch=='0' || __ch=='A' || __ch=='C' || __ch=='B' || __ch=='D' || __ch=='E' || __ch=='F' || __ch=='$')

typedef enum operator_t{
	mul,
	//div,
	plus,
	minus,
	and,
	or,
	not,
	dolor,
	open_bracket,
	close_bracket,
}operator_t;

operator_t get_operatot(char symb){
	operator_t result;

	switch(symb){
	case '&':result=and; break;
	case '|':result=or; break;
	case '!': result=not; break;
	case '(': result=open_bracket; break;
	case ')': result=close_bracket; break;
	case '*': result=mul; break;
//	case '/': result=div; break;
	case '+': result=plus; break;
	case '-': result=minus; break;
	case '$' : result=dolor ; break;
	}
	return result;
}


boolean precedence(char symb1,char symb2){

	static int op_pre_table[]={
			[mul]=1,
//			[div]=2,
			[plus]=3,
			[minus]=4,
			[and]=5,
			[or]=6,
			[not]=7,
			[dolor]=8,
			[open_bracket]=9,
			[close_bracket]=10
	};
	operator_t op1=get_operatot(symb1);
	operator_t op2=get_operatot(symb2);
	if(op1==open_bracket || op2==open_bracket)
		return false;
	if(op2==close_bracket)
		return true;

	return op_pre_table[op1] >op_pre_table[op2];
}


boolean is_valid_paraenthis(char *infix){
	boolean res=false;
	struct stack stack;

	stack_init(&stack);
	char symb;
	while((symb=*infix++)!='\0'){
		if(stack_empty(&stack) && symb==')'){
			return false;
		}
		if(symb=='('){
			stack_push(&stack,symb);
		}
		if(symb==')' ){
			if(stack_top(&stack)!='(')
		     return false;
			else{
				stack_pop(&stack);
			}
		}
	}
   return stack_empty(&stack);
}


void infix_to_postfix(char *infix,char *postfix){


	struct stack stack;
    char *start=postfix;
	stack_init(&stack);
	char symb='\0',topsymb;

	while((symb=*infix++)!='\0'){
		if(isOperand(symb)){
			*postfix++=symb;
		}

		else{
			if(!stack_empty(&stack)){
			 topsymb=stack_top(&stack);
			}
			while(!stack_empty(&stack) && precedence(topsymb,symb)){
				*postfix++=topsymb;
				stack_pop(&stack);
				topsymb=stack_top(&stack);
			}

			if(stack_empty(&stack) || symb!=')'){
				stack_push(&stack,symb);
			}
			else{
				topsymb=stack_pop(&stack);
			}
		}
	}

	while(!stack_empty(&stack)){
		*postfix++=stack_pop(&stack);
	}
	*postfix='\0';

}

boolean eval_postfix(char *postfix){

	struct stack stack;
	stack_init(&stack);
	char op1,op2,value;

	char symb='\0';
	while((symb=*postfix++)!='\0'){

		if(isOperand(symb)){
			stack_push(&stack,symb);
		}
		else
		{

			char value=(op1==op2)? '1':'0';
			switch(symb){
			case '|':
				op1=stack_pop(&stack);
				op2=stack_pop(&stack);
				value=(op1=='1' || op2=='1')? '1':'0';
				break;
			case '&':
				op1=stack_pop(&stack);
				op2=stack_pop(&stack);
				value=(op1=='0' || op2=='0')? '0':'1';
				break;
			case '!':
				op1=stack_pop(&stack);
				value=(op1=='1')? '0':'1';
				break;
			}
			stack_push(&stack,value);
		}
	}
	return (stack_pop(&stack)=='1');
}

