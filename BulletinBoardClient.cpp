#include<iostream>
#include<string>
#include<WS2tcpip.h>
#include <sstream> 
#pragma comment(lib, "ws2_32.lib")


using namespace std;


int main() {
	string ipNum;
	string portNums;
	int portNum = 0;
	cout << "Enter the IP address: ";
	cout << endl;
	getline(cin, ipNum);
	cout << "Enter the Port Number: ";
	cout << endl;
	getline(cin, portNums);
	stringstream pn(portNums);
	pn >> portNum;
	WSAData value;
	WORD ide = MAKEWORD(2, 2);
	int flag = WSAStartup(ide, &value);
	if (flag != 0) {
		cerr << "ERROR: Winsock cannot be opened!" << flag << endl;
		return 0;
	}


	SOCKET location = socket(AF_INET, SOCK_STREAM, 0);
	if (location == INVALID_SOCKET) {
		cerr << "ERROR: Socket cannot be created!" << WSAGetLastError() << endl;
		return 0;
	}


	sockaddr_in local;
	local.sin_family = AF_INET;
	local.sin_port = htons(portNum);
	inet_pton(AF_INET, ipNum.c_str(), &local.sin_addr);


	int signal = connect(location, (sockaddr*)&local, sizeof(local));
	if (signal == SOCKET_ERROR) {
		cout << "IP Address: " << ipNum << "\t" << "Port Number: " << portNum;
		cout << endl;
		cout << "Connect status: failed" << endl;
		closesocket(location);
		WSACleanup();
		return 0;
	}
	// The failed connection needs to wait longer than the demo version or need to click buttons to trigger it.
	else
	{
		cout << "IP Address: " << ipNum << "\t" << "Port Number: " << portNum;
		cout << endl;
		cout << "Connect status: success" << endl;
	}



	char bufPut[4096];
	string inpVal;
	string userType;
	int a = 5;
	//while loop to send and receive message to the server
	do
	{
		getline(cin, userType);
		if (userType == "READ") {		//When the user input READ
			inpVal = userType + '\n';
			cout << "client:" << " " << userType << endl;
			int tstore;
			send(location, inpVal.c_str(), inpVal.size() + 1, 0);
			ZeroMemory(bufPut, 4096);
			tstore = recv(location, bufPut, 1024, 0);
			if (tstore > 0) {
				cout << "Server:" << " " << string(bufPut, 0, tstore) << endl;;
			}

			while (string(bufPut, 0, tstore) != ".") {
				tstore = recv(location, bufPut, 1024, 0);
				if (tstore > 0) {
					cout << "Server:" << " " << string(bufPut, 0, tstore) << endl;;
				}
			}
		}
		else if (userType == "POST") {		//When the user input POST
			inpVal = userType + '\n';
			while (userType != ".") {			//When the input of the user is not ".", loop to get the input
				cout << "client:" << " " << userType << endl;
				getline(cin, userType);
				inpVal = inpVal + userType + '\n';
			}
			cout << "client:" << " " << userType << endl;
			send(location, inpVal.c_str(), inpVal.size() + 1, 0);
			int tstore;
			tstore = recv(location, bufPut, 1024, 0);
			if (tstore > 0) {
				cout << "Server:" << " " << string(bufPut, 0, tstore) << endl;;
			}

		}
		else if (userType == "QUIT") {			//When the user input QUIT
			cout << "client:" << " " << userType << endl;
			string quit = userType + '\n';
			send(location, quit.c_str(), quit.size() + 1, 0);
			ZeroMemory(bufPut, 4096);
			int tstore;
			tstore = recv(location, bufPut, 1024, 0);
			if (tstore >= 1) {
				cout << "Server:" << " " << string(bufPut, 0, tstore) << endl;;
			}

		}
		else {
			inpVal = userType + '\n';
			cout << "client:" << " " << userType << endl;
			send(location, inpVal.c_str(), inpVal.size() + 1, 0);
			ZeroMemory(bufPut, 4096);
			int tstore;
			tstore = recv(location, bufPut, 1024, 0);
			if (tstore >= 1) {
				cout << "Server:" << " " << string(bufPut, 0, tstore) << endl;;
			}
		}
	} while (userType != "QUIT");



	closesocket(location);
	WSACleanup();
	return 0;
}