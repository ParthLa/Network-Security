#include<iostream>
#include <winsock.h>

using namespace std;

struct sockaddr {
unsigned short sa_family; // address family,
 AF_xxx char sa_data[14]; // 14 bytes of protocol address
  };

struct sockaddr_in{
 short int sin_family; // Address family
 unsigned short int sin_port; // Port number
 struct in_addr sin_addr; // Internet address
 unsigned char sin_zero[8]; // Same size as struct sockaddr
};

struct in_addr {
unsigned long s_addr;
};


int main(){
//Write your code here
WSAData wsaData;
if (WSAStartup(MAKEWORD(1, 1), &wsaData) != 0)
 {
  fprintf(stderr, "WSAStartup failed.\n"); exit(1);
 }
}
