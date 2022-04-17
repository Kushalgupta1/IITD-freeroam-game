#include <iostream>
#include <fstream>
#include <cstdio>
#include <unistd.h>
#include <cstdlib>
#include <cstring>
#include <time.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#define  PORT 9987

using namespace std;

struct Info{
	int stateFirst;
	int stateSecond;
	int myXcoord;
	int myYcoord;
	int myState;
	int health;
	int happiness;
	int money;
};

void toNetwork(char* buffer, struct Info* mydata){
	//32 byte buffer required
	//test bit
	buffer[0] = '*';
	//storing X of player
	std::sprintf(buffer +1, "%02d", mydata -> stateFirst);
	std::sprintf(buffer +4, "%02d", mydata -> stateSecond);
	std::sprintf(buffer +7, "%04d", mydata -> myXcoord);
	std::sprintf(buffer +12, "%04d", mydata -> myYcoord);
	std::sprintf(buffer +17, "%01d", mydata -> myState);
	std::sprintf(buffer +19, "%03d", mydata -> health);
	std::sprintf(buffer +23, "%03d", mydata -> happiness);
	std::sprintf(buffer +27, "%03d", mydata -> money);
	buffer[31] = '#';
}

bool fromNetwork(char* buffer, struct Info* indata){
	if(buffer[0] != '*' || buffer[31] != '#') return false;
	indata -> stateFirst = atoi(buffer +1);
	indata -> stateSecond = atoi(buffer +4);
	indata -> myXcoord = atoi(buffer +7);
	indata -> myYcoord = atoi(buffer +12);
	indata -> myState = atoi(buffer +17);
	indata -> health = atoi(buffer +19);
	indata -> happiness = atoi(buffer +23);
	indata -> money = atoi(buffer +27);
	return true;
}

void print_data(struct Info* indata){
	
	cout << indata -> stateFirst << "\n";
	cout << indata -> stateSecond << "\n";
	cout << indata -> myXcoord << "\n";
	cout << indata -> myYcoord << "\n";
	cout << indata -> myState << "\n";
	cout << indata -> health << "\n";
	cout << indata -> happiness << "\n";
	cout << indata -> money << "\n";
}

int main(int argc, char *argv[])
{
	int cli_fd, bytes_sent, bytes_recvd;
	int port_no = PORT;
	char in_buffer[32], out_buffer[32], sname[16], cname[16];

	struct Info indata;
	struct Info mydata = {42, 5, 6, 7, 8, 9, 1, 4};
	bool validate_data;

	//IP address of server
	char serv_ip[INET_ADDRSTRLEN]= "192.168.43.53";	
	//char serv_ip[INET_ADDRSTRLEN]= "127.0.0.1";	

	struct sockaddr_in serv_addr;
	
	//if (argc != 2)
	//{
	//	perror("Incomplete arguments!");
	//	return 1;
	//}

	if((cli_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{	
		perror("Sorry. Socket could not be created!");
		return 1;
	}
	int opt = 1;

	if (setsockopt(cli_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
		perror("setsockopt");
		exit(EXIT_FAILURE);
	}

    memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(port_no);

	// Convert IPv4 addresses from text to binary form
	if (inet_pton(AF_INET, serv_ip, &serv_addr.sin_addr) <= 0) {
		printf(
			"\nInvalid address/ Address not supported \n");
		return -1;
	}
	cout << "Socket created and trying to connect\n";

	if( connect(cli_fd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) <0)
	{	
		perror("Sorry. Could not connect to server.");
		return 1;
	}

	cout << "Connected (hopefully)\n";

	cout<<"Enter your name : ";
	cin>>cname;

	do
	{
		static int flag = 0;
		bytes_sent = send(cli_fd, &cname, sizeof(cname), 0);
		if (bytes_sent == -1 && flag == 0)
		{
			cout<<"PLAYER DATA NOT SENT!"<<endl<<"Trying Again...";  
			continue;
		}
		else
		{		
			flag = 1;
			memset(&sname, 0, sizeof(sname));
			bytes_recvd = recv(cli_fd, &sname, sizeof(sname), 0);
			if (bytes_recvd == -1)
				cout<<"COULD NOT ACQUIRE PLAYER INFORMATION!"<<endl<<"Trying Again..."<<endl; 
			else
				cout<<"You have joined "<<sname<<" for a game of Tic-Tac-Toe."<<endl;
		}
	}while(bytes_sent == -1 || bytes_recvd == -1);

	for(int i = 0; i < 100; i++){
		//each frame
		mydata = {i, i, i, i, i, i, i, i};
		//each frame

		//sending
		toNetwork(out_buffer, &mydata);
		do
		{
			bytes_sent = send(cli_fd, &out_buffer, sizeof(out_buffer), 0);
			if (bytes_sent == -1)
				cout<<"Frame data not sent!"<<endl<<"Trying Again...";  

			else if (bytes_sent != 32)
				cout << "complete data not sent, what is going on???????\n";
		}while(bytes_sent != 32);

		//receiving
		usleep(10000);		
		do
		{
			memset(&in_buffer, 0, sizeof(in_buffer));

			bytes_recvd = recv(cli_fd, &in_buffer, sizeof(in_buffer), 0);
			if (bytes_recvd == -1)
				cout<<"Frame data not received!"<<endl<<"Trying Again..."<<endl; 

			else if (bytes_recvd != 32)
				cout << "complete data not received, what is going on!!!\n";

			else 
				validate_data = fromNetwork(in_buffer, &indata);
				if(!validate_data) {
					cout << "Wrong data received";
					bytes_recvd = -1;
				}
			
		} while (bytes_recvd != 32);

		print_data(&indata);
	}
	close(cli_fd);
}

