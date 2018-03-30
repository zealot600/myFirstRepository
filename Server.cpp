#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <Windows.h>
#include <process.h>
#pragma comment(lib,"ws2_32.lib")
#define BUF_SIZE 100
#define MAX_CLNT 256

unsigned WINAPI HandleClient(void* arg);//쓰레드 함수
void SendMsg(char* msg, int len);//메시지 보내는 함수
void ErrorHandling(char* msg);

char searc = 'b';
char *out;
char *result;
int score[4];
int gameflag = 0;//게임중인지 아닌지 여부 flag
int p = 0;
char printnickcode[10] = "nickname";
char nickName[4][10];
int clientCount = 0;
char playerNickName[20][20];//닉네임보관배열
int countt = 0;

SOCKET clientSocks[MAX_CLNT];//클라이언트 소켓 보관용 배열
HANDLE hMutex;//뮤텍스

int main() {
	WSADATA wsaData;
	SOCKET serverSock, clientSock;
	SOCKADDR_IN serverAddr, clientAddr;
	int clientAddrSize;
	HANDLE hThread;

	char port[100];
	int startcount = 0;
	char *strtk2, *strtk3;
	char forsprintf[100];//sprintf 용 문자열

	printf("Input port number : ");
	scanf("%s", port);
	/*
	if(argc!=2){
	printf("Usage : %s <port>\n",argv[0]);
	exit(1);
	}
	*/
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) //윈도우 소켓을 사용하겠다는 사실을 운영체제에 전달
		ErrorHandling("WSAStartup() error!");

	hMutex = CreateMutex(NULL, FALSE, NULL);//하나의 뮤텍스를 생성한다.
	serverSock = socket(PF_INET, SOCK_STREAM, 0); //하나의 소켓을 생성한다.

	memset(&serverAddr, 0, sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	serverAddr.sin_port = htons(atoi(port));

	if (bind(serverSock, (SOCKADDR*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) //생성한 소켓을 배치한다.
		ErrorHandling("bind() error");
	if (listen(serverSock, 5) == SOCKET_ERROR)//소켓을 준비상태에 둔다.
		ErrorHandling("listen() error");

	printf("listening...\n");

	while (1) {
		while (1) {
			if ((clientCount == 4) && (gameflag == 0))
			{
				if (startcount == 10) {
					char startcode[9] = "go";  // 이부분 수정함 char7값이전달되도록
					send(clientSocks[0], startcode, 9, 0);//SendMsg에 받은 메시지를 전달한다.
					send(clientSocks[1], startcode, 9, 0);//SendMsg에 받은 메시지를 전달한다.
					send(clientSocks[2], startcode, 9, 0);//SendMsg에 받은 메시지를 전달한다.
					send(clientSocks[3], startcode, 9, 0);//SendMsg에 받은 메시지를 전달한다.
					break;
				}
				sprintf(forsprintf, "%d 초후 게임이 시작됩니다\n", 10 - startcount);
				SendMsg(forsprintf, strlen(forsprintf));//5초를세고 게임시작
				Sleep(1000);
				startcount++;
			}
			else {
				startcount = 0;
				break;
			}
		}

		clientAddrSize = sizeof(clientAddr);
		clientSock = accept(serverSock, (SOCKADDR*)&clientAddr, &clientAddrSize);//서버에게 전달된 클라이언트 소켓을 clientSock에 전달
		WaitForSingleObject(hMutex, INFINITE);//뮤텍스 실행
		clientSocks[clientCount++] = clientSock;//클라이언트 소켓배열에 방금 가져온 소켓 주소를 전달
		ReleaseMutex(hMutex);//뮤텍스 중지
		hThread = (HANDLE)_beginthreadex(NULL, 0, HandleClient, (void*)&clientSock, 0, NULL);//HandleClient 쓰레드 실행, clientSock을 매개변수로 전달
		printf("Connected Client IP : %s\n", inet_ntoa(clientAddr.sin_addr));
	}
	closesocket(serverSock);//생성한 소켓을 끈다.
	WSACleanup();//윈도우 소켓을 종료하겠다는 사실을 운영체제에 전달
	return 0;
}

unsigned WINAPI HandleClient(void* arg) {
	SOCKET clientSock = *((SOCKET*)arg); //매개변수로받은 클라이언트 소켓을 전달
	int strLen = 0, i;
	char msg[BUF_SIZE];

	char searc = 'b';
	char *out[10];//스트링토크나이징용
	char *result;
	char sendmsging[200];
	char printnickcode[10] = "nickname";
	char forprint[50];


	while ((strLen = recv(clientSock, msg, sizeof(msg), 0)) != 0) { //클라이언트로부터 메시지를 받을때까지 기다린다.

		msg[strLen] = 0;
		printf("%s\n", msg);

		if (!strcmp(out[0] = strtok(msg, "@"), "adbra5"))//닉네임입력받기위해서 토크나이징
		{
			//printf("닉네임보내기!!!!!!!!!!!\n");
			
			//   out = strtok(msg, ":");
			out[1] = strtok(NULL, "儆");
			//printf("%s", out[1]);
			//printf("%d", clientCount);
			strcpy(playerNickName[clientCount - 1], out[1]);//닉네임배열에추가
															//   sprintf(sendmsging, "%d",clientCount);
															//strcat(sendmsging, "@");
															//   for (int j = 0; j < clientCount; j++) {
			sprintf(sendmsging, "%d@%s", clientCount, printnickcode);
			Sleep(500);
			SendMsg(sendmsging, strlen(sendmsging));
			//   }

			sprintf(forprint, "%s 님이 입장하였습니다.", out[1]);



			SendMsg(forprint, strlen(forprint));
			printf("메세지보내기!!  %s\n", sendmsging);


		}

		else if (!strcmp(msg, "okay"))
		{
			countt++;
			if (countt == 4) {
				Sleep(1000);
				for (int i = 0; i < 4; i++)
				{
					Sleep(150);
					printf("for문실행!\n");
					sprintf(sendmsging, "%s %d남음", nickName[i], score[i]);
					SendMsg(sendmsging, strlen(sendmsging));

				}
				sprintf(sendmsging, "%d@%s", clientCount, printnickcode);
				SendMsg(sendmsging, strlen(sendmsging));
				printf("p제로전");
				p = 0;
				countt = 0;
				printf("p제로후");
			}
		}


		else    if (!strcmp(msg, "quit")) {
			printf("큐들어옴\n");
			send(clientSock, "quit", 4, 0);
			break;
		}

		else if (!strcmp(msg, "go")) {
			printf("go들어옴\n");
			//send(clientSock, "go", 2, 0);
			send(clientSocks[0], "go", 2, 0);//SendMsg에 받은 메시지를 전달한다.
			send(clientSocks[1], "go", 2, 0);//SendMsg에 받은 메시지를 전달한다.
			send(clientSocks[2], "go", 2, 0);//SendMsg에 받은 메시지를 전달한다.
			send(clientSocks[3], "go", 2, 0);//SendMsg에 받은 메시지를 전달한다.
			//break;
		}
		else if (!strcmp(msg, "y")) {
			printf("y걸림\n");
		}

		else if (!strcmp(msg, "\n")) {
			printf("엔터 걸림\n");
		}

		else   if (!strcmp(msg, "st")) {
			printf("stop들어옴\n");
			//sprintf(msg, "1등입니다");
			//sprintf(msg, "%s 1등입니다.", clientSocks[i]);
			for (int i = 0; i < 4; i++)
			{

				send(clientSocks[i], "st", 2, 0);
				Sleep(300);
			}
			//SendMsg(msg, strLen);//SendMsg에 받은 메시지를 전달한다.
			//break;
		}
		else   if (!strcmp(strtok(msg, "#"), "opkl"))//닉네임입력받기위해서 토크나이징
		{
			//msg[strLen] = 0;
			printf("걸림1!!!!!!!!!!!\n");
			//opkl@name@getRe
			//   out = strtok(msg, ":");
			strcpy(nickName[p], strtok(NULL, "#"));
			printf("%s\n", nickName[p]);
			score[p] = atoi(strtok(NULL, "#"));
			printf("%d\n", score[p]);
			p++;


			//printf("%s", out[1]);
			//printf("%d", clientCount);
			//strcpy(playerNickName[clientCount - 1], out[1]);//닉네임배열에추가
			//                                    //   sprintf(sendmsging, "%d",clientCount);
			//                                    //strcat(sendmsging, "@");
			//                                    //   for (int j = 0; j < clientCount; j++) {
			//sprintf(sendmsging, "%d@%s", clientCount, printnickcode);
			//Sleep(500);
			//SendMsg(sendmsging, strlen(sendmsging));
			////   }
			//
			//sprintf(forprint, "%s 님이 입장하였습니다.", out[1]);
			//
			//
			//
			//SendMsg(forprint, strlen(forprint));
			//printf("메세지보내기!!  %s\n", sendmsging);
			//printf("%s %s", out[1], out[2]);


		}

		//else if (!strcmp(result = strchr(out = strtok(msg, "儆 \n"), searc), "b"))
		//{
		//   printf("여기걸림2\n");
		//   char startcode[9] = "a921223b";  // 이부분 수정함 char7값이전달되도록
		//   send(clientSock, startcode, strlen(startcode), 0);//SendMsg에 받은 메시지를 전달한다.
		//}

		//if (!strcmp(msg, "endgame"))//만약 게임종료코드가들어오면
		//{
		//   printf("엔드게임들어옴\n");
		//   send(clientSock, "endgame", 1, 0);
		//   //clientCount--;
		//   SendMsg(msg, strLen);
		//   WaitForSingleObject(hMutex, INFINITE);//뮤텍스 실행
		//   for (i = 0; i<clientCount; i++) {//배열의 갯수만큼
		//      if (clientSock == clientSocks[i]) {//만약 현재 clientSock값이 배열의 값과 같다면
		//         while (i++<clientCount - 1)//클라이언트 개수 만큼
		//            clientSocks[i] = clientSocks[i + 1];//앞으로 땡긴다.
		//         break;
		//      }
		//   }
		//   clientCount--;//클라이언트 개수 하나 감소
		//   ReleaseMutex(hMutex);//뮤텍스 중지
		//   closesocket(clientSock);//소켓을 종료한다.         ***********나중에없애야함************
		//}


		else
			SendMsg(msg, strLen);//SendMsg에 받은 메시지를 전달한다.



	}

	printf("client left the chat\n");
	sprintf(sendmsging, "%d@%s", clientCount-1, printnickcode);
	SendMsg(sendmsging, strlen(sendmsging));
	//이 줄을 실행한다는 것은 해당 클라이언트가 나갔다는 사실임 따라서 해당 클라이언트를 배열에서 제거해줘야함
	WaitForSingleObject(hMutex, INFINITE);//뮤텍스 실행
	for (i = 0; i<clientCount; i++) {//배열의 갯수만큼
		if (clientSock == clientSocks[i]) {//만약 현재 clientSock값이 배열의 값과 같다면
			sprintf(forprint, "%s 님이 퇴장하였습니다.", playerNickName[i]);
			SendMsg(forprint, strlen(forprint));
			while (i++ < clientCount - 1)//클라이언트 개수 만큼
			{
				clientSocks[i] = clientSocks[i + 1];//앞으로 땡긴다.
				strcpy(playerNickName[i], playerNickName[i + 1]);//플레이어도 앞으로땡긴다
			}
			break;
		}
	}
	clientCount--;//클라이언트 개수 하나 감소
	ReleaseMutex(hMutex);//뮤텍스 중지
	closesocket(clientSock);//소켓을 종료한다.
	return 0;
}

void SendMsg(char* msg, int len) { //메시지를 모든 클라이언트에게 보낸다.
	int i;
	WaitForSingleObject(hMutex, INFINITE);//뮤텍스 실행
	for (i = 0; i<clientCount; i++)//클라이언트 개수만큼
		send(clientSocks[i], msg, len, 0);//클라이언트들에게 메시지를 전달한다.
	ReleaseMutex(hMutex);//뮤텍스 중지
}
void ErrorHandling(char* msg) {
	fputs(msg, stderr);
	fputc('\n', stderr);
	exit(1);
}