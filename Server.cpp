#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <Windows.h>
#include <process.h>
#pragma comment(lib,"ws2_32.lib")
#define BUF_SIZE 100
#define MAX_CLNT 256

unsigned WINAPI HandleClient(void* arg);//������ �Լ�
void SendMsg(char* msg, int len);//�޽��� ������ �Լ�
void ErrorHandling(char* msg);

char searc = 'b';
char *out;
char *result;
int score[4];
int gameflag = 0;//���������� �ƴ��� ���� flag
int p = 0;
char printnickcode[10] = "nickname";
char nickName[4][10];
int clientCount = 0;
char playerNickName[20][20];//�г��Ӻ����迭
int countt = 0;

SOCKET clientSocks[MAX_CLNT];//Ŭ���̾�Ʈ ���� ������ �迭
HANDLE hMutex;//���ؽ�

int main() {
	WSADATA wsaData;
	SOCKET serverSock, clientSock;
	SOCKADDR_IN serverAddr, clientAddr;
	int clientAddrSize;
	HANDLE hThread;

	char port[100];
	int startcount = 0;
	char *strtk2, *strtk3;
	char forsprintf[100];//sprintf �� ���ڿ�

	printf("Input port number : ");
	scanf("%s", port);
	/*
	if(argc!=2){
	printf("Usage : %s <port>\n",argv[0]);
	exit(1);
	}
	*/
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) //������ ������ ����ϰڴٴ� ����� �ü���� ����
		ErrorHandling("WSAStartup() error!");

	hMutex = CreateMutex(NULL, FALSE, NULL);//�ϳ��� ���ؽ��� �����Ѵ�.
	serverSock = socket(PF_INET, SOCK_STREAM, 0); //�ϳ��� ������ �����Ѵ�.

	memset(&serverAddr, 0, sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	serverAddr.sin_port = htons(atoi(port));

	if (bind(serverSock, (SOCKADDR*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) //������ ������ ��ġ�Ѵ�.
		ErrorHandling("bind() error");
	if (listen(serverSock, 5) == SOCKET_ERROR)//������ �غ���¿� �д�.
		ErrorHandling("listen() error");

	printf("listening...\n");

	while (1) {
		while (1) {
			if ((clientCount == 4) && (gameflag == 0))
			{
				if (startcount == 10) {
					char startcode[9] = "go";  // �̺κ� ������ char7�������޵ǵ���
					send(clientSocks[0], startcode, 9, 0);//SendMsg�� ���� �޽����� �����Ѵ�.
					send(clientSocks[1], startcode, 9, 0);//SendMsg�� ���� �޽����� �����Ѵ�.
					send(clientSocks[2], startcode, 9, 0);//SendMsg�� ���� �޽����� �����Ѵ�.
					send(clientSocks[3], startcode, 9, 0);//SendMsg�� ���� �޽����� �����Ѵ�.
					break;
				}
				sprintf(forsprintf, "%d ���� ������ ���۵˴ϴ�\n", 10 - startcount);
				SendMsg(forsprintf, strlen(forsprintf));//5�ʸ����� ���ӽ���
				Sleep(1000);
				startcount++;
			}
			else {
				startcount = 0;
				break;
			}
		}

		clientAddrSize = sizeof(clientAddr);
		clientSock = accept(serverSock, (SOCKADDR*)&clientAddr, &clientAddrSize);//�������� ���޵� Ŭ���̾�Ʈ ������ clientSock�� ����
		WaitForSingleObject(hMutex, INFINITE);//���ؽ� ����
		clientSocks[clientCount++] = clientSock;//Ŭ���̾�Ʈ ���Ϲ迭�� ��� ������ ���� �ּҸ� ����
		ReleaseMutex(hMutex);//���ؽ� ����
		hThread = (HANDLE)_beginthreadex(NULL, 0, HandleClient, (void*)&clientSock, 0, NULL);//HandleClient ������ ����, clientSock�� �Ű������� ����
		printf("Connected Client IP : %s\n", inet_ntoa(clientAddr.sin_addr));
	}
	closesocket(serverSock);//������ ������ ����.
	WSACleanup();//������ ������ �����ϰڴٴ� ����� �ü���� ����
	return 0;
}

unsigned WINAPI HandleClient(void* arg) {
	SOCKET clientSock = *((SOCKET*)arg); //�Ű������ι��� Ŭ���̾�Ʈ ������ ����
	int strLen = 0, i;
	char msg[BUF_SIZE];

	char searc = 'b';
	char *out[10];//��Ʈ����ũ����¡��
	char *result;
	char sendmsging[200];
	char printnickcode[10] = "nickname";
	char forprint[50];


	while ((strLen = recv(clientSock, msg, sizeof(msg), 0)) != 0) { //Ŭ���̾�Ʈ�κ��� �޽����� ���������� ��ٸ���.

		msg[strLen] = 0;
		printf("%s\n", msg);

		if (!strcmp(out[0] = strtok(msg, "@"), "adbra5"))//�г����Է¹ޱ����ؼ� ��ũ����¡
		{
			//printf("�г��Ӻ�����!!!!!!!!!!!\n");
			
			//   out = strtok(msg, ":");
			out[1] = strtok(NULL, "��");
			//printf("%s", out[1]);
			//printf("%d", clientCount);
			strcpy(playerNickName[clientCount - 1], out[1]);//�г��ӹ迭���߰�
															//   sprintf(sendmsging, "%d",clientCount);
															//strcat(sendmsging, "@");
															//   for (int j = 0; j < clientCount; j++) {
			sprintf(sendmsging, "%d@%s", clientCount, printnickcode);
			Sleep(500);
			SendMsg(sendmsging, strlen(sendmsging));
			//   }

			sprintf(forprint, "%s ���� �����Ͽ����ϴ�.", out[1]);



			SendMsg(forprint, strlen(forprint));
			printf("�޼���������!!  %s\n", sendmsging);


		}

		else if (!strcmp(msg, "okay"))
		{
			countt++;
			if (countt == 4) {
				Sleep(1000);
				for (int i = 0; i < 4; i++)
				{
					Sleep(150);
					printf("for������!\n");
					sprintf(sendmsging, "%s %d����", nickName[i], score[i]);
					SendMsg(sendmsging, strlen(sendmsging));

				}
				sprintf(sendmsging, "%d@%s", clientCount, printnickcode);
				SendMsg(sendmsging, strlen(sendmsging));
				printf("p������");
				p = 0;
				countt = 0;
				printf("p������");
			}
		}


		else    if (!strcmp(msg, "quit")) {
			printf("ť����\n");
			send(clientSock, "quit", 4, 0);
			break;
		}

		else if (!strcmp(msg, "go")) {
			printf("go����\n");
			//send(clientSock, "go", 2, 0);
			send(clientSocks[0], "go", 2, 0);//SendMsg�� ���� �޽����� �����Ѵ�.
			send(clientSocks[1], "go", 2, 0);//SendMsg�� ���� �޽����� �����Ѵ�.
			send(clientSocks[2], "go", 2, 0);//SendMsg�� ���� �޽����� �����Ѵ�.
			send(clientSocks[3], "go", 2, 0);//SendMsg�� ���� �޽����� �����Ѵ�.
			//break;
		}
		else if (!strcmp(msg, "y")) {
			printf("y�ɸ�\n");
		}

		else if (!strcmp(msg, "\n")) {
			printf("���� �ɸ�\n");
		}

		else   if (!strcmp(msg, "st")) {
			printf("stop����\n");
			//sprintf(msg, "1���Դϴ�");
			//sprintf(msg, "%s 1���Դϴ�.", clientSocks[i]);
			for (int i = 0; i < 4; i++)
			{

				send(clientSocks[i], "st", 2, 0);
				Sleep(300);
			}
			//SendMsg(msg, strLen);//SendMsg�� ���� �޽����� �����Ѵ�.
			//break;
		}
		else   if (!strcmp(strtok(msg, "#"), "opkl"))//�г����Է¹ޱ����ؼ� ��ũ����¡
		{
			//msg[strLen] = 0;
			printf("�ɸ�1!!!!!!!!!!!\n");
			//opkl@name@getRe
			//   out = strtok(msg, ":");
			strcpy(nickName[p], strtok(NULL, "#"));
			printf("%s\n", nickName[p]);
			score[p] = atoi(strtok(NULL, "#"));
			printf("%d\n", score[p]);
			p++;


			//printf("%s", out[1]);
			//printf("%d", clientCount);
			//strcpy(playerNickName[clientCount - 1], out[1]);//�г��ӹ迭���߰�
			//                                    //   sprintf(sendmsging, "%d",clientCount);
			//                                    //strcat(sendmsging, "@");
			//                                    //   for (int j = 0; j < clientCount; j++) {
			//sprintf(sendmsging, "%d@%s", clientCount, printnickcode);
			//Sleep(500);
			//SendMsg(sendmsging, strlen(sendmsging));
			////   }
			//
			//sprintf(forprint, "%s ���� �����Ͽ����ϴ�.", out[1]);
			//
			//
			//
			//SendMsg(forprint, strlen(forprint));
			//printf("�޼���������!!  %s\n", sendmsging);
			//printf("%s %s", out[1], out[2]);


		}

		//else if (!strcmp(result = strchr(out = strtok(msg, "�� \n"), searc), "b"))
		//{
		//   printf("����ɸ�2\n");
		//   char startcode[9] = "a921223b";  // �̺κ� ������ char7�������޵ǵ���
		//   send(clientSock, startcode, strlen(startcode), 0);//SendMsg�� ���� �޽����� �����Ѵ�.
		//}

		//if (!strcmp(msg, "endgame"))//���� ���������ڵ尡������
		//{
		//   printf("������ӵ���\n");
		//   send(clientSock, "endgame", 1, 0);
		//   //clientCount--;
		//   SendMsg(msg, strLen);
		//   WaitForSingleObject(hMutex, INFINITE);//���ؽ� ����
		//   for (i = 0; i<clientCount; i++) {//�迭�� ������ŭ
		//      if (clientSock == clientSocks[i]) {//���� ���� clientSock���� �迭�� ���� ���ٸ�
		//         while (i++<clientCount - 1)//Ŭ���̾�Ʈ ���� ��ŭ
		//            clientSocks[i] = clientSocks[i + 1];//������ �����.
		//         break;
		//      }
		//   }
		//   clientCount--;//Ŭ���̾�Ʈ ���� �ϳ� ����
		//   ReleaseMutex(hMutex);//���ؽ� ����
		//   closesocket(clientSock);//������ �����Ѵ�.         ***********���߿����־���************
		//}


		else
			SendMsg(msg, strLen);//SendMsg�� ���� �޽����� �����Ѵ�.



	}

	printf("client left the chat\n");
	sprintf(sendmsging, "%d@%s", clientCount-1, printnickcode);
	SendMsg(sendmsging, strlen(sendmsging));
	//�� ���� �����Ѵٴ� ���� �ش� Ŭ���̾�Ʈ�� �����ٴ� ����� ���� �ش� Ŭ���̾�Ʈ�� �迭���� �����������
	WaitForSingleObject(hMutex, INFINITE);//���ؽ� ����
	for (i = 0; i<clientCount; i++) {//�迭�� ������ŭ
		if (clientSock == clientSocks[i]) {//���� ���� clientSock���� �迭�� ���� ���ٸ�
			sprintf(forprint, "%s ���� �����Ͽ����ϴ�.", playerNickName[i]);
			SendMsg(forprint, strlen(forprint));
			while (i++ < clientCount - 1)//Ŭ���̾�Ʈ ���� ��ŭ
			{
				clientSocks[i] = clientSocks[i + 1];//������ �����.
				strcpy(playerNickName[i], playerNickName[i + 1]);//�÷��̾ �����ζ����
			}
			break;
		}
	}
	clientCount--;//Ŭ���̾�Ʈ ���� �ϳ� ����
	ReleaseMutex(hMutex);//���ؽ� ����
	closesocket(clientSock);//������ �����Ѵ�.
	return 0;
}

void SendMsg(char* msg, int len) { //�޽����� ��� Ŭ���̾�Ʈ���� ������.
	int i;
	WaitForSingleObject(hMutex, INFINITE);//���ؽ� ����
	for (i = 0; i<clientCount; i++)//Ŭ���̾�Ʈ ������ŭ
		send(clientSocks[i], msg, len, 0);//Ŭ���̾�Ʈ�鿡�� �޽����� �����Ѵ�.
	ReleaseMutex(hMutex);//���ؽ� ����
}
void ErrorHandling(char* msg) {
	fputs(msg, stderr);
	fputc('\n', stderr);
	exit(1);
}