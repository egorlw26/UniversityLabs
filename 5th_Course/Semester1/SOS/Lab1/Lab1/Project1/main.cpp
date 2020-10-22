#pragma comment(lib,"Ws2_32.lib")

#include <iostream>
#include <fstream>
#include <sys/types.h>
#include <winsock2.h>
#include <string>
#include <stdio.h>
#include <WS2tcpip.h>

using namespace std;

int main()
{
	// Setting up WSADATA
	WSADATA wsadata = { 0 };
	int startupRes = WSAStartup(MAKEWORD(2, 2), &wsadata);

	// Creating a socket
	auto serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (serverSocket ==  INVALID_SOCKET)
	{
		cerr << "Can't create socket correctly, error: "<< WSAGetLastError() << endl;
		return -1;
	}

	// Binding to IP/Port
	sockaddr_in hint;
	hint.sin_family = AF_INET;
	hint.sin_port = htons(54000);
	inet_pton(AF_INET, "0.0.0.0", &hint.sin_addr);

	if (bind(serverSocket, (SOCKADDR*)&hint, sizeof(hint)) == -1)
	{
		cerr << "Can't bind to port " << hint.sin_port << endl;
		return -1;
	}

	if (listen(serverSocket, SOMAXCONN) == -1)
	{
		cerr << "Can't listen" << endl;
		return -1;
	}

	// Creating client and accepting it
	sockaddr_in client;
	int clientSize = sizeof(client);
	char host[NI_MAXHOST];
	char service[NI_MAXSERV];

	int clientSocket = accept(serverSocket, (SOCKADDR*)&client, &clientSize);
	if (clientSocket == INVALID_SOCKET)
	{
		cerr << "Can't accept client" << endl;
		return -1;
	}

	closesocket(serverSocket);

	memset(host, 0, NI_MAXHOST);
	memset(service, 0, NI_MAXSERV);

	int nameInfo = getnameinfo((SOCKADDR*)&client,
		clientSize,
		host, NI_MAXHOST,
		service, NI_MAXSERV,
		0);

	if (nameInfo)
	{
		cout << host << " connected on " << service << endl;
	}
	else
	{
		inet_ntop(AF_INET, &client.sin_addr, host, NI_MAXHOST);
		cout << host << " connected on " << ntohs(client.sin_port) << endl;
	}

	// Recv messages and show them
	char msgBuf[4096];
	while (true)
	{
		memset(msgBuf, 0, 4096);
		int bytesRecv = recv(clientSocket, msgBuf, 4096, 0);
		if (bytesRecv == -1)
		{
			cerr << "Can't receive bytes from client" << endl;
			break;
		}

		if (bytesRecv == 0)
		{
			cerr << "Client disconnected" << endl;
			break;
		}

		cout << "Message: " << string(msgBuf, 0, bytesRecv) << endl;

		send(clientSocket, msgBuf, bytesRecv + 1, 0);
	}

	closesocket(clientSocket);

	return 0;
}