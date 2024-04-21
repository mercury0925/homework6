/**
 * circularQ.c
 *
 * School of Computer Science, 
 * Chungbuk National University
 */

#include <stdio.h>
#include <stdlib.h>

#define MAX_QUEUE_SIZE 4 // MAX_QUEUE_SIZE를 4로 정의

typedef char element; // 데이터 타입을 char로 정의
typedef struct { // QueueType 구조체 정의
	element queue[MAX_QUEUE_SIZE]; // MAX_QUEUE_SIZE인 queue 선언
	int front, rear; // front와 rear 선언
}QueueType;


QueueType *createQueue();
int freeQueue(QueueType *cQ);
int isEmpty(QueueType *cQ);
int isFull(QueueType *cQ);
void enQueue(QueueType *cQ, element item);
void deQueue(QueueType *cQ, element* item);
void printQ(QueueType *cQ);
void debugQ(QueueType *cQ);
element getElement();

int main(void)
{
	QueueType *cQ = createQueue(); // createQueue 함수를 호출하여 새로운 큐를 생성하고 그 큐를 가리키는 QueueType 구조체 포인터 cQ 선언
	element data; // 큐에 추가할 데이터를 저장할 변수 선언
	char command; // 명령을 저장할 변수 선언

	printf("-----[HanKangmin] [2023078040]-----\n");

	do{  // do-while문
		printf("\n-----------------------------------------------------\n");
		printf("                     Circular Q                   \n");
		printf("------------------------------------------------------\n");
		printf(" Insert=i,  Delete=d,   PrintQ=p,   Debug=b,   Quit=q \n");
		printf("------------------------------------------------------\n");

		printf("Command = "); 
		scanf(" %c", &command); // 명령을 입력받음

		switch(command) { 
		case 'i': case 'I':
			data = getElement(); // 데이터 입력받음
			enQueue(cQ, data); // 큐에 데이터 추가
			break;
		case 'd': case 'D':
			deQueue(cQ, &data); // 큐에서 데이터 삭제
			break;
		case 'p': case 'P':
			printQ(cQ); // 큐 출력
			break;
		case 'b': case 'B':
			debugQ(cQ); // 큐 디버깅
			break;
		case 'q': case 'Q':
   	        freeQueue(cQ); // 큐 해제
			break;
		default:
			printf("\n       >>>>>   Concentration!!   <<<<<     \n"); // 잘못된 명령을 입력했을 때 Concentration!! 출력
			break;
		}

	}while(command != 'q' && command != 'Q'); // q나 Q를 입력받으면 반복문 종료


	return 1; 
}

// 큐를 생성하는 함수 createQueue()
QueueType *createQueue()
{
	QueueType *cQ; // QueueType 구조체 포인터 cQ 선언
	cQ = (QueueType *)malloc(sizeof(QueueType)); // cQ에 QueueType 구조체 크기만큼 동적으로 메모리 할당함
	cQ->front = 0; // cQ가 가리키는 멤버 변수 front를 0으로 초기화
	cQ->rear = 0; // cQ가 가리키는 멤버 변수 rear를 0으로 초기화
	return cQ; // cQ 반환
}

// 큐를 해제하는 함수 freeQueue()
int freeQueue(QueueType *cQ)
{
    if(cQ == NULL) return 1; // 큐가 NULL이면(큐가 할당되지 않은 경우) 1 반환
    free(cQ); // cQ가 가리키는 메모리 해제
    return 1; // 큐가 해제되었으므로 1 반환
}

// 데이터를 입력받는 함수 getElement()
element getElement()
{
	element item; // 데이터를 저장할 변수 item 선언
	printf("Input element = ");
	scanf(" %c", &item); // item 변수에 데이터 입력받음
	return item; // item 반환
}

/* complete the function */
// 큐가 비어있는지 확인하는 함수 isEmpty()
int isEmpty(QueueType *cQ)
{
    if(cQ->front == cQ->rear) // front와 rear가 같으면 큐가 비어있는 것이므로 1 반환
		return 1; 
	else // 큐가 비어있지 않으면
		return 0; // 0 반환
}


/* complete the function */
// 큐가 가득 차있는지 확인하는 함수 isFull()
int isFull(QueueType *cQ)
{
	if((cQ->rear+1)%MAX_QUEUE_SIZE == cQ->front) // rear의 다음 위치가 front와 같으면 큐가 가득 차있는 것이므로 1 반환
    	return 1; 
	else // 큐가 가득 차있지 않으면
		return 0; // 0 반환
}


/* complete the function */
// 큐에 데이터를 추가하는 함수 enQueue()
void enQueue(QueueType *cQ, element item) 
{
	if(isFull(cQ)) // 큐가 가득 차있으면
		printf("Queue is full\n"); // Queue is full 출력
	else { // 큐가 가득 차있지 않으면
		cQ->rear = (cQ->rear+1)%MAX_QUEUE_SIZE; // rear의 다음 위치로 이동
		cQ->queue[cQ->rear] = item; // item을 큐의 rear 위치에 저장
	}
}

/* complete the function */
// 큐에서 데이터를 삭제하는 함수 deQueue()
void deQueue(QueueType *cQ, element *item)
{
	if(isEmpty(cQ)) // 큐가 비어있으면 
		printf("Queue is empty\n"); // Queue is empty 출력
	else { // 큐가 비어있지 않으면
		cQ->front = (cQ->front+1)%MAX_QUEUE_SIZE; // front의 다음 위치로 이동
		*item = cQ->queue[cQ->front]; // 큐의 front 위치에 있는 데이터를 item이 가리키는 변수에 저장 (삭제된 요소를 반환하기 위해 item에 저장)
	}
}


// 큐를 출력하는 함수 printQ()
void printQ(QueueType *cQ)
{
	int i, first, last; // i, first, last 정수형 변수 선언

	first = (cQ->front + 1)%MAX_QUEUE_SIZE; // first에 front+1을 MAX_QUEUE_SIZE로 나눈 나머지 대입
	// first에 front의 다음 위치 저장
	last = (cQ->rear + 1)%MAX_QUEUE_SIZE; // last에 rear+1을 MAX_QUEUE_SIZE로 나눈 나머지 대입
	// last에 rear의 다음 위치 저장
	// 원형 큐이므로 MAX_QUEUE_SIZE로 나눈 나머지를 사용하여 인덱스가 배열의 크기를 초과하지 않도록 함
	
	printf("Circular Queue : [");

	i = first; // i에 first 값 대입
	while(i != last){ // i가 last와 같지 않은 동안 반복
		printf("%3c", cQ->queue[i]); // 큐의 i번째 원소 출력
		i = (i+1)%MAX_QUEUE_SIZE; // i에 i+1을 MAX_QUEUE_SIZE로 나눈 나머지 대입
	}
	printf(" ]\n");
}

// 큐를 디버깅하는 함수 debugQ()
void debugQ(QueueType *cQ)
{

	printf("\n---DEBUG\n");
	for(int i = 0; i < MAX_QUEUE_SIZE; i++) // i가 0부터 MAX_QUEUE_SIZE-1까지 반복
	{
		if(i == cQ->front) { // i가 front와 같으면(현재 위치가 큐의 시작 위치이면)
			printf("  [%d] = front\n", i); // front 출력
			continue;
		}
		printf("  [%d] = %c\n", i, cQ->queue[i]); // front의 위치가 아니면 현재 인덱스 i와 큐의 i번째 요소 출력

	}
	printf("front = %d, rear = %d\n", cQ->front, cQ->rear); // front와 rear 출력
}


