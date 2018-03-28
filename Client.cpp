#include <Turboc.h>
#include <process.h>

#pragma comment(lib,"ws2_32.lib")

#define BUF_SIZE 100
#define NAME_SIZE 20

#define LEFT 75
#define RIGHT 77
#define UP 72
#define DOWN 80
#define ESC 27

enum Status { HIDDEN, FLIP, TEMPFLIP };
struct tag_Cell
{
	int Num;
	Status St;
};
tag_Cell arCell[4][4];
int nx, ny;
int count;


void InitGame();
void DrawScreen(BOOL bHint);
void GetTempFlip(int *tx, int *ty);
int GetRemain();
void gamming(void* arg);
char gamefinishcode[10] = "endgame";
void DrawWaitScreen();//대기화면생성함수
int flagggg = 0;
int gameflag = 0;
int ch;

int stacktop = 0; // 배열용 변수
char stackarray[20][100]; //채팅방 배열
char fornicksend[30]; //첫접속시 서버에닉네임전송

char playerNickName[20][20];//닉네임보관배열
int nickcount = 0;
char *three, *four;
int j;
int k = 0;

unsigned WINAPI SendMsg(void* arg);//쓰레드 전송함수
unsigned WINAPI RecvMsg(void* arg);//쓰레드 수신함수
void ErrorHandling(char* msg);

char name[NAME_SIZE] = "[DEFAULT]";
char msg[BUF_SIZE];

int main() {
	WSADATA wsaData;
	SOCKET sock;
	SOCKADDR_IN serverAddr;
	HANDLE sendThread, recvThread;

	char myIp[100];
	char port[100];
	char inputName[100];
	/*
	if(argc!=4){
	printf("Usage : %s <IP> <port> <name>\n",argv[0]);
	exit(1);
	}
	*/
	printf("Input server IP : ");
	gets_s(myIp);
	//strcpy(myIp, "192.168.0.219");

	printf("Input server port : ");
	gets_s(port);
	//strcpy(port, "2323");

	printf("Input your name : ");
	gets_s(inputName);

	system("cls");

	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)// 윈도우 소켓을 사용한다고 운영체제에 알림
		ErrorHandling("WSAStartup() error!");

	sprintf(name, "[%s]", inputName);
	sock = socket(PF_INET, SOCK_STREAM, 0);//소켓을 하나 생성한다.

	memset(&serverAddr, 0, sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_addr.s_addr = inet_addr(myIp);
	serverAddr.sin_port = htons(atoi(port));

	if (connect(sock, (SOCKADDR*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR)//서버에 접속한다.
		ErrorHandling("connect() error");

	sprintf(fornicksend, "adbra5@%s", inputName);
	send(sock, fornicksend, strlen(fornicksend), 0);

	//접속에 성공하면 이 줄 아래가 실행된다.

	//gotoxy(40, 2); printf("player1 = 비어있음");
	//gotoxy(40, 4); printf("player2 = 비어있음");
	//gotoxy(40, 6); printf("player3 = 비어있음");
	//gotoxy(40, 8); printf("player4 = 비어있음");
	//gotoxy(0, 21); printf("----------------------------");
	gotoxy(1, 22); printf("Plese Wait...");

	sendThread = (HANDLE)_beginthreadex(NULL, 0, SendMsg, (void*)&sock, 0, NULL);//메시지 전송용 쓰레드가 실행된다.
	recvThread = (HANDLE)_beginthreadex(NULL, 0, RecvMsg, (void*)&sock, 0, NULL);//메시지 수신용 쓰레드가 실행된다.

	WaitForSingleObject(sendThread, INFINITE);//전송용 쓰레드가 중지될때까지 기다린다./
	WaitForSingleObject(recvThread, INFINITE);//수신용 쓰레드가 중지될때까지 기다린다.

											  //클라이언트가 종료를 시도한다면 이줄 아래가 실행된다.
	closesocket(sock);//소켓을 종료한다.
	WSACleanup();//윈도우 소켓 사용중지를 운영체제에 알린다.
	return 0;
}

unsigned WINAPI SendMsg(void* arg) {//전송용 쓰레드함수
	SOCKET sock = *((SOCKET*)arg);//서버용 소켓을 전달한다.
	char nameMsg[NAME_SIZE + BUF_SIZE];

	int strLen;

	while (1)
	{
		while (gameflag != 1) {//게임중일땐 입력안받는다.
			gotoxy(1, 22);
			fgets(msg, BUF_SIZE, stdin);//입력을 받는다.
			if (!strcmp(msg, "quit\n")) {//q를 입력하면 종료한다.
				send(sock, "quit", 4, 0);//nameMsg를 서버에게 전송한다.
			}
			if (!strcmp(msg, "go\n")) {//q를 입력하면 종료한다.
				send(sock, "go", 2, 0);//nameMsg를 서버에게 전송한다.
			}
			
			if (!strcmp(msg, "y")) {//q를 입력하면 종료한다.
				send(sock, "y", 1, 0);//nameMsg를 서버에게 전송한다.
			}
			if (!strcmp(msg, "y\n\n")) {//q를 입력하면 종료한다.
				send(sock, "y", 1, 0);//nameMsg를 서버에게 전송한다.
			}

			if (!strcmp(msg, "\n\n")) {//q를 입력하면 종료한다.
				send(sock, "\n", 1, 0);//nameMsg를 서버에게 전송한다.
			}
			
			if (!strcmp(msg, "st\n")) {//q를 입력하면 종료한다.
				send(sock, "st", 2, 0);//nameMsg를 서버에게 전송한다.
			}
			if (!strcmp(msg, "y\n")) {//q를 입력하면 종료한다.
				send(sock, "y", 1, 0);//nameMsg를 서버에게 전송한다.
			}
			else
			{
				sprintf(nameMsg, "%s %s", name, msg);//nameMsg에 메시지를 전달한다.
				send(sock, nameMsg, strlen(nameMsg), 0);//nameMsg를 서버에게 전송한다.
			}
		}
		//게임플래그가 1일 땐(게임중) 남은갯수를 보내줌
		while (gameflag == 1)
		{
			strLen = recv(sock, nameMsg, NAME_SIZE + BUF_SIZE - 1, 0);//서버로부터 메시지를 수신한다.
			if (strLen == -1)
				return -1;

			nameMsg[strLen] = 0;//문자열의 끝을 알리기 위해 설정
								//sprintf(nameMsg, "%s %s", name, msg);//nameMsg에 메시지를 전달한다.
								//send(sock, nameMsg, strlen(nameMsg), 0);//nameMsg를 서버에게 전송한다.
								//printf("돈다");
								//fputs(nameMsg, stdout);//자신의 콘솔에 받은 메시지를 출력한다.
			if (!strcmp(nameMsg, "st")) {
				system("cls");
				gameflag = 0;
				flagggg = 1;
				ch = ESC;

				char dex[10];
				itoa(GetRemain(), dex, 10);
				sprintf(nameMsg, "opkl#%s#%s", name, dex);//nameMsg에 메시지를 전달한다.
				send(sock, nameMsg, strlen(nameMsg), 0);//nameMsg를 서버에게 전송한다.
														//순위띄워줌

				system("cls");
				printf("엔터를 두 번 눌러주세요");
				getch();
				send(sock, "okay", 4, 0);
				//system("cls");



			}

		}
	}
	return 0;
}

unsigned WINAPI RecvMsg(void* arg) {

	SOCKET sock = *((SOCKET*)arg);//서버용 소켓을 전달한다.
	char nameMsg[NAME_SIZE + BUF_SIZE];

	char com[120] = { "go" };
	int strLen;

	char copynameMsg[NAME_SIZE + BUF_SIZE];
	char *ouch[20];
	char nickcode[10] = "nickname";
	char *token = NULL;
	char *out = NULL;
	char *outtwo = NULL;
	char w2[] = "@";



	while (1) {//반복
		strLen = recv(sock, nameMsg, NAME_SIZE + BUF_SIZE - 1, 0);//서버로부터 메시지를 수신한다.

		if (strLen == -1)
			return -1;

		nameMsg[strLen] = 0;//문자열의 끝을 알리기 위해 설정

		if (!strcmp(nameMsg, "quit")) {
			printf("left the chat\n");
			closesocket(sock);
			exit(0);
		}

		else if (!strcmp(nameMsg, "go")) {
			system("cls");
			for (int i = 0; i < 20; i++)
			{
				strcpy(stackarray[i],"\0"); //채팅방 배열
			}
			stacktop = 0;
			gameflag = 1;
			gamming(arg);

		}

		//else if (!strcmp(nameMsg, "st")) {
		//	gameflag = 0;
		//	system("cls");
		//
		//}

		else if (strstr(nameMsg, nickcode) != NULL) {
			strcpy(copynameMsg, nameMsg);
			//   fflush(stdout);

			//      Sleep(1000);
			token = strtok(copynameMsg, w2);
			nickcount = atoi(token);
			//      gotoxy(60, 17); printf("닉카운트값 : %d", nickcount);

			//      k = 0;
			//   gotoxy(50, 12); printf("닉네임넣는곳걸림 = %s", nameMsg);
			//   Sleep(1000);
			//      ouch[k] = nameMsg;
			//      k++;


		}

		else if (stacktop < 20) {
			strcpy(stackarray[stacktop], nameMsg);
			fflush(stdout);
			stacktop++;
			//   gotoxy(50, 12); printf("player6 = %s", nameMsg);

		}
		else {

			for (int i = 0; i < stacktop - 1; i++) {
				strcpy(stackarray[i], stackarray[i + 1]);
			}
			strcpy(stackarray[stacktop - 1], nameMsg);
			fflush(stdout);
			//   gotoxy(50, 12); printf("player7 = %s", nameMsg);

		}
		//   if(strtok)

		clrscr();
		j = 0;
		gotoxy(3, 0); printf("채팅창");

		while (j < 20)
		{
			gotoxy(1, j + 1);
			fputs(stackarray[j], stdout);//자신의 콘솔에 받은 메시지를 출력한다.
			j++;
		}
		char waiter[10] = " ";
		gotoxy(43, 0); printf(" 접속상태 ");
		for (int o = 0; o < nickcount; o++) {
			if (o> 3)strcpy(waiter, "[대기자]");
			gotoxy(40, o + 2); printf("player%d접속%s", o + 1, waiter);

		}
		gotoxy(0, 21); printf("----------------------------");
		gotoxy(1, 22); printf("입력 : ");
		//   gotoxy(1, 24); fflush(stdin); printf("%s", no1);
		fflush(stdout);

		//fputs(nameMsg, stdout);//자신의 콘솔에 받은 메시지를 출력한다.
	}
	return 0;
}

void gamming(void* arg)
{
	SOCKET sock = *((SOCKET*)arg);//서버용 소켓을 전달한다.
	char nameMsg[NAME_SIZE + BUF_SIZE];
	printf("y버튼과 엔터를 눌러주세요");
	fflush(stdin);
	system("cls");


	//delay(5000);

	int tx, ty;

	randomize();
	InitGame();

	while (gameflag == 1) {
		//if (!strcmp(nameMsg, "st")) {
		//	gameflag = 0;
		//	system("cls");
		//	break;
		//}
		gotoxy(nx * 5 + 2, ny * 3 + 2);

		ch = getch();
		if (ch == 0xE0) {
			ch = getch();
			switch (ch) {
			case LEFT:
				if (nx > 0) nx--;
				break;
			case RIGHT:
				if (nx < 3) nx++;
				break;
			case UP:
				if (ny > 0) ny--;
				break;
			case DOWN:
				if (ny < 3) ny++;
				break;
			}
		}
		else {
			switch (ch) {
			case ESC:
				//exit(0);
				break;
			case ' ':
				if (arCell[nx][ny].St == HIDDEN) {
					GetTempFlip(&tx, &ty);
					if (tx == -1) {
						arCell[nx][ny].St = TEMPFLIP;
					}
					else {
						count++;
						if (arCell[tx][ty].Num == arCell[nx][ny].Num) {
							putch('\a');
							arCell[tx][ty].St = FLIP;
							arCell[nx][ny].St = FLIP;
							if (GetRemain() == 0) {
								//DrawScreen(FALSE);
								//gotoxy(26, 22); puts("축하합니다. 다시 시작합니다.");
								//delay(2000);
								//InitGame();
								DrawScreen(FALSE);
								gotoxy(26, 22); puts("축하합니다.");
								//delay(2000);
								//gameflag = 0; // 메세지전송활성화를위한 플래그값변경
								//sprintf(nameMsg, "%s %s", name, gamefinishcode);//nameMsg에 메시지를 전달한다.
								//send(sock, nameMsg, strlen(nameMsg), 0);//nameMsg를 서버에게 전송한다.

								//sprintf(msg, "1등입니다");
								//sprintf(nameMsg, "%s %s", name, msg);//nameMsg에 메시지를 전달한다.
								//send(sock, nameMsg, strlen(nameMsg), 0);//nameMsg를 서버에게 전송한다.
								send(sock, "st", 2, 0);
								//	gotoxy(24, 23);
								//printf("%s  %s",name,msg);

								//delay(2000);

								//종료시키기
								//sprintf(msg, "st@%s",name);
								//send(sock, msg, 2, 0);//nameMsg를 서버에게 전송한다.
								//system("cls");
								//send(sock, nameMsg, strlen(nameMsg), 0);

								//break;
							}
						}
						else {
							arCell[nx][ny].St = TEMPFLIP;
							DrawScreen(FALSE);
							delay(1000);
							arCell[tx][ty].St = HIDDEN;
							arCell[nx][ny].St = HIDDEN;
						}
					}
					DrawScreen(FALSE);

				}
				break;
			}
		}
	}
}

void ErrorHandling(char* msg) {
	fputs(msg, stderr);
	fputc('\n', stderr);
	exit(1);
}

void InitGame()
{
	int i, j;
	int x, y;

	nx = ny = 0;
	count = 0;

	memset(arCell, 0, sizeof(arCell));

	for (i = 1; i <= 8; i++) {
		for (j = 0; j < 2; j++) {
			do {
				x = random(4);
				y = random(4);
			} while (arCell[x][y].Num != 0);
			arCell[x][y].Num = i;
		}
	}

	DrawScreen(TRUE);
	delay(2000);
	clrscr();
	DrawScreen(FALSE);
}

void DrawWaitScreen() {
	//printf("여기는 대기방입니다");
	gotoxy(26, 30); puts("대기방.");
}

void DrawScreen(BOOL bHint)
{
	int x, y;

	for (y = 0; y < 4; y++) {
		for (x = 0; x < 4; x++) {
			gotoxy(x * 5 + 2, y * 3 + 2);
			if (bHint == TRUE || arCell[x][y].St == FLIP) {
				gotoxy(wherex() - 1, wherey());
				printf("[%d]", arCell[x][y].Num);
			}
			else if (arCell[x][y].St == TEMPFLIP) {
				printf("%d", arCell[x][y].Num);
			}
			else {
				printf("?");
			}
		}
	}

	gotoxy(30, 2); puts("커플 게임 멀티 Ver 1.0");
	gotoxy(30, 4); puts("커서키:이동. 공백:뒤집기");
	gotoxy(30, 6); printf("총 시도 회수 : %d", count);
	gotoxy(30, 8); printf("아직 못 찾은 것 : %d ", GetRemain());
}

void GetTempFlip(int *tx, int *ty)
{
	int i, j;
	for (i = 0; i < 4; i++) {
		for (j = 0; j < 4; j++) {
			if (arCell[i][j].St == TEMPFLIP) {
				*tx = i;
				*ty = j;
				return;
			}
		}
	}
	*tx = -1;
}

int GetRemain()
{
	int i, j;
	int remain = 16;

	for (i = 0; i < 4; i++) {
		for (j = 0; j < 4; j++) {
			if (arCell[i][j].St == FLIP) {
				remain--;
			}
		}
	}
	return remain;
}