/** 
 * postfix.c
 *
 * School of Computer Science, 
 * Chungbuk National University
 */

#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define MAX_STACK_SIZE 10 // MAX_STACK_SIZE를 10으로 정의
#define MAX_EXPRESSION_SIZE 20 // MAX_EXPRESSION_SIZE를 20으로 정의

/* stack 내에서 우선순위, lparen = 0 가장 낮음 */
typedef enum{
	lparen = 0,  /* ( 왼쪽 괄호 */
	rparen = 9,  /* ) 오른쪽 괄호*/
	times = 7,   /* * 곱셈 */
	divide = 6,  /* / 나눗셈 */
	plus = 5,    /* + 덧셈 */
	minus = 4,   /* - 뺄셈 */
	operand = 1 /* 피연산자 */
} precedence;

char infixExp[MAX_EXPRESSION_SIZE];   	/* infix expression을 저장하는 배열 */
char postfixExp[MAX_EXPRESSION_SIZE];	/* postfix로 변경된 문자열을 저장하는 배열 */
char postfixStack[MAX_STACK_SIZE];	/* postfix로 변환을 위해 필요한 스택 */
int evalStack[MAX_STACK_SIZE];		/* 계산을 위해 필요한 스택 */

int postfixStackTop = -1;  /* postfixStack용 top */
int evalStackTop = -1;	   /* evalStack용 top */

int evalResult = 0;	   /* 계산 결과를 저장 */

void postfixpush(char x);
char postfixPop();
void evalPush(int x);
int evalPop();
void getInfix();
precedence getToken(char symbol);
precedence getPriority(char x);
void charCat(char* c);
void toPostfix();
void debug();
void reset();
int evaluation();

int main()
{
	char command;

	printf("-----[HanKangmin] [2023078040]-----\n");

	do{
		printf("----------------------------------------------------------------\n");
		printf("               Infix to Postfix, then Evaluation               \n");
		printf("----------------------------------------------------------------\n");
		printf(" Infix=i,   Postfix=p,  Eval=e,   Debug=d,   Reset=r,   Quit=q \n");
		printf("----------------------------------------------------------------\n");

		printf("Command = ");
		scanf(" %c", &command); // 명령을 입력받음

		switch(command) {
		case 'i': case 'I':
			getInfix(); 
			break;
		case 'p': case 'P':
			toPostfix();
			break;
		case 'e': case 'E':
			evaluation();
			break;
		case 'd': case 'D':
			debug();
			break;
		case 'r': case 'R':
			reset();
			break;
		case 'q': case 'Q':
			break;
		default:
			printf("\n       >>>>>   Concentration!!   <<<<<     \n"); // 잘못된 명령을 입력한 경우 Concentration!! 출력
			break;
		}

	}while(command != 'q' && command != 'Q'); // q나 Q를 입력받으면 반복문 종료

	return 1;
}

// 스택에 새로운 요소를 추가하는 함수 postfixPush()
void postfixPush(char x)
{
    postfixStack[++postfixStackTop] = x; // postfixStackTop을 1 증가시키고 x를 postfixStack에 저장
}

// 스택에서 요소를 제거하는 함수 postfixPop()
char postfixPop()
{
    char x; 
    if(postfixStackTop == -1) // postfixStackTop이 -1이면 (스택이 비어있으면)
        return '\0'; // \0 반환
    else { // 스택이 비어있지 않으면
    	x = postfixStack[postfixStackTop--]; // postfixStackTop을 1 감소시키고 postfixStackTop에 있는 값을 x에 저장
    }
    return x; // x 반환
}

// 스택에 새로운 요소를 추가하는 함수 evalPush()
void evalPush(int x)
{
    evalStack[++evalStackTop] = x; // evalStackTop을 1 증가시키고 x를 evalStack에 저장
}

// 스택에서 요소를 제거하는 함수 evalPop()
int evalPop()
{
    if(evalStackTop == -1) // evalStackTop이 -1이면 (스택이 비어있으면)
        return -1; // -1 반환
    else // 스택이 비어있지 않으면
        return evalStack[evalStackTop--]; // evalStackTop에 있는 값을 반환하고 evalStackTop을 1 감소시킴
}

/**
 * infix expression을 입력받는다.
 * infixExp에는 입력된 값을 저장한다.
 */
// infix expression을 입력받는 함수 getInfix()
void getInfix()
{
    printf("Type the expression >>> ");
    scanf("%s",infixExp); // infixExp에 문자열을 입력받음
}

// 문자를 받아 우선순위를 반환하는 함수 getToken()
precedence getToken(char symbol)
{
	switch(symbol) {
	case '(' : return lparen;
	case ')' : return rparen;
	case '+' : return plus;
	case '-' : return minus;
	case '/' : return divide;
	case '*' : return times;
	default : return operand;
	}
}

// 우선순위를 반환하는 함수 getPriority()
precedence getPriority(char x)
{
	return getToken(x); // getToken() 함수를 호출하여 x의 우선순위를 반환
}

/**
 * 문자하나를 전달받아, postfixExp에 추가
 */
void charCat(char* c)
{
	if (postfixExp == '\0') // postfixExp가 \0(NULL)이면 (postfixExp가 비어있으면)
		strncpy(postfixExp, c, 1); // c의 첫 번째 문자를 postfixExp에 복사
	else // postfixExp가 비어있지 않으면
		strncat(postfixExp, c, 1); // c의 첫 번째 문자를 postfixExp에 추가
}

/**
 * infixExp의 문자를 하나씩 읽어가면서 stack을 이용하여 postfix로 변경한다.
 * 변경된 postfix는 postFixExp에 저장된다.
 */

// infixExp를 postfix로 변경하는 함수 toPostfix()
void toPostfix()
{
	/* infixExp의 문자 하나씩을 읽기위한 포인터 */
	char *exp = infixExp; 
	char x; /* 문자하나를 임시로 저장하기 위한 변수 */

	/* exp를 증가시켜가면서, 문자를 읽고 postfix로 변경 */
	while(*exp != '\0') // exp가 \0(NULL)이 아니면 반복
	{
		if(getToken(*exp) == operand) { // getToken() 함수를 호출하여 *exp가 피연산자이면
			charCat(exp); // exp를 postfixExp에 추가
		}
		else if(*exp == '(') { // *exp가 '('이면
			postfixPush(*exp); // *exp를 postfixStack에 추가
		}
		else if(*exp == ')') { // *exp가 ')'이면
			while((x=postfixPop()) != '(') { // postfixStack에서 pop한 값을 x에 저장하고 x가 '('가 아닐 때까지)
				charCat(&x); // x를 postfixExp에 추가			
			}
		}
		else {
			// postfixStack이 비어있지 않고 postfixStackTop에 있는 연산자의 우선순위가 *exp의 우선순위보다 크거나 같을 동안
			while(postfixStackTop != -1 && getPriority(postfixStack[postfixStackTop]) >= getPriority(*exp)){ 
				x = postfixPop(); // postfixStack에서 pop한 값을 x에 저장
				charCat(&x); // x를 postfixExp에 추가
			}
			postfixPush(*exp); // *exp를 postfixStack에 추가
		}
		/* 필요한 로직 완성 */
		exp++; // exp를 증가시키고 다음 문자로 넘어감
	}
	while(postfixStackTop != -1) { // postfixStack이 비어있지 않으면 
		x = postfixPop(); // postfixStack에서 pop(스택에 남아있는 연산자를)
		charCat(&x); // x를 postfixExp에 추가
	}
	/* 필요한 로직 완성 */
}

// 디버깅을 위한 함수 debug()
void debug()
{
	printf("\n---DEBUG\n");
	printf("infixExp =  %s\n", infixExp); // infixExp 출력
	printf("postExp =  %s\n", postfixExp); // postfixExp 출력
	printf("eval result = %d\n", evalResult); // evalResult 출력

	printf("postfixStack : ");
	for(int i = 0; i < MAX_STACK_SIZE; i++) // i가 0부터 MAX_STACK_SIZE-1까지 반복
		printf("%c  ", postfixStack[i]); // postfixStack의 i번째 요소 출력

	printf("\n");

}

// 초기화를 위한 함수 reset()
void reset()
{
	infixExp[0] = '\0'; // infixExp 초기화
	postfixExp[0] = '\0'; // postfixExp 초기화

	for(int i = 0; i < MAX_STACK_SIZE; i++) // i가 0부터 MAX_STACK)SIZE-1까지 반복
		postfixStack[i] = '\0'; // postfixStaxk의 i번째 요소를 NULL로 초기화
         
	postfixStackTop = -1; // postfixStackTop을 -1로 초기화
	evalStackTop = -1; // evalStackTop을 -1로 초기화
	evalResult = 0; // evalResult를 0으로 초기화
}

// 후위 표기식을 계산하는 함수 evaluation()
int evaluation()
{
	char *exp = postfixExp; // postfixExp의 문자 하나씩을 읽기위한 포인터
	int op1, op2; // 숫자를 저장하기 위한 변수
	while(*exp != '\0') { // exp가 \0(NULL)이 아니면 반복
		if(getToken(*exp) == operand){ // getToken() 함수를 호출하여 *exp가 피연산자이면
			evalPush(*exp - '0'); // *exp에서 '0'을 빼고 evalStack에 추가(숫자로 변환)
		}
		else { // *exp가 연산자이면
			op2 = evalPop(); // evalStack에서 pop한 값을 op2에 저장
			op1 = evalPop(); // evalStack에서 pop한 값을 op1에 저장
			// 연산자에 따라 op1과 op2를 계산하여 evalStack에 추가
			switch(*exp) {
				case '+':
					evalPush(op1+op2);
					break;
				case '-':
					evalPush(op1-op2);
					break;
				case '*':
					evalPush(op1*op2);
					break;
				case '/':
					evalPush(op1/op2);
					break;				
			}
		}
		exp++; // exp를 증가시키고 다음 문자로 넘어감
	}
	evalResult = evalPop(); // evalStack에서 pop한 값을 evalResult에 저장
	return evalResult; // evalResult 반환
	/* postfixExp, evalStack을 이용한 계산 */
}