#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <time.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#define  PORT 9987;

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
	int serv_fd, newserv_fd, bytes_sent, bytes_recvd;
	int port_no =PORT;
	char in_buffer[32], out_buffer[32], sname[64] = "sKushal", cname[64]; 
	struct Info indata;
	struct Info mydata = {42, 5, 6, 7, 8, 9, 1, 4};
	bool validate_data;

	char cli_ip[INET_ADDRSTRLEN];	
	//IP address of server
	char serv_ip[INET_ADDRSTRLEN]= "192.168.43.53";	

	struct sockaddr_in serv_addr, cli_addr;	

	//creating sever side socket	
	if ( (serv_fd = socket(AF_INET, SOCK_STREAM, 0)) < -1)
	{	
		perror("Server side listening Socket could not be created!");
		return 1;
	}

	int opt = 1;

	if (setsockopt(serv_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
		perror("setsockopt");
		exit(EXIT_FAILURE);
	}

	memset(&serv_addr, 0, sizeof(serv_addr));	

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(port_no);

	//serv_addr.sin_addr.s_addr = INADDR_ANY;

	//Convert IPv4 addresses from text to binary form
	if (inet_pton(AF_INET, serv_ip, &serv_addr.sin_addr)
		<= 0) {
		printf(
			"\nInvalid address/ Address not supported \n");
		return -1;
	}
	
	//binding socket
	if (bind(serv_fd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
	{	
		perror("Failed to bind!");
		return 1;
	}

	if (listen(serv_fd, 5)== -1)
	{	
		perror("Failed to listen!");
		return 1;
	}

	memset(&cli_addr, 0, sizeof(cli_addr));	

	socklen_t cli_size = sizeof(cli_addr);
		
	if ( (newserv_fd = accept(serv_fd, (struct sockaddr *)&cli_addr, &cli_size)) == -1)
	{	
		perror("Failed to accept from client!");
		return 1;
	}
	inet_ntop(AF_INET, &cli_addr.sin_addr, cli_ip, INET_ADDRSTRLEN);
	cout<<"Server received connections from "<<cli_ip<<endl;

	do
	{	
		static int flag = 0;
		memset(&cname, 0, sizeof(cname));
		bytes_recvd = recv(newserv_fd, &cname, sizeof(cname), 0);
		if (bytes_recvd == -1 && flag == 0)
		{
			memset(&cname, 0, sizeof(cname));
			cout<<"Could not ACQUIRE Player Information!"<<endl<<"Trying again..."<<endl; 
			continue;
		}
		else
		{	
			flag = 1;
			bytes_sent = send(newserv_fd, &sname, sizeof(sname), 0);
			if (bytes_sent == -1)
				cout<<"Could not SEND Player Data!"<<"Trying Again..."<<endl; 
			else
				cout<<cname<<" has joined the game."<<endl;
		}
	}while(bytes_recvd == -1 || bytes_sent == -1);

	for(int i = 0; i < 100; i++){
		//each frame
		mydata = {i, i, i, i, i, i, i, i};

		//receiving
		usleep(10000);		
		do
		{
			memset(&in_buffer, 0, sizeof(in_buffer));

			bytes_recvd = recv(newserv_fd, &in_buffer, sizeof(in_buffer), 0);
			if (bytes_recvd == -1)
				cout<<"Frame data not received!"<<endl<<"Trying Again..."<<endl; 

			else if (bytes_recvd != 32)
				cout << "complete data not received, what is going on!!!\n";

			else 
				validate_data = fromNetwork(in_buffer, &indata);
				if(!validate_data) {
					cout << "Wrong data received\n";
					bytes_recvd = -1;
				}
			
		} while (bytes_recvd != 32);

		print_data(&indata);

		//sending
		toNetwork(out_buffer, &mydata);
		do
		{
			bytes_sent = send(newserv_fd, &out_buffer, sizeof(out_buffer), 0);
			if (bytes_sent == -1)
				cout<<"Frame data not sent!"<<endl<<"Trying Again...";  

			else if (bytes_sent != 32)
				cout << "complete data not sent, what is going on???????\n";
		}while(bytes_sent != 32);

	}
	close(serv_fd);
	close(newserv_fd);
}