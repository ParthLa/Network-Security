#include<iostream>
#include<fstream>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<regex>
#include<unistd.h>
#include<dirent.h>
#include<list>
#include<string>
#include<string.h>
#include<stdlib.h>
using namespace std;

const int chunk_size = 1024;

/////////////////////////
//FUNCTION DECLARATIONS//
/////////////////////////

int send_message(int sockfd, string message);
//If not sent, prints error message and returns -10
//Else returns number of bytes sent
string rec_message(int sockfd);
//Receives upto chunk_size bytes and returns string
//If error receiving, prints appropriate error and returns "error"

int send_login(string username, string password, int sockfd);
//Sends login message
//Returns -10 and prints "Login failed" if failed

///////////////
//MAIN STARTS//
///////////////

int main(int argc, char*argv[]){
//Validate and extract arguments
//	string IP
//	int port
//	string username
//	string password
	if (argc != 6){
		cerr<<"Invalid number of arguments\nUsage: SimpleEmailClientPhase3 <IP:PortNum> <Username> <Password> <list-of-messages> <local-folder>\n";
		exit(1);
	}
	vector<long int> msg_list;
	long int n;
	string arg4(argv[4]);
	char* ends;
	int b=10;
	while(true){
	 n= strtol(arg4,&ends,b);
	 if (ends[0] != ','){
	  cerr<<"not parsable to a list of numbers";
	  exit(3);
	 }
     else{
      msg_list.push_back(n);
      //int s3= send_message(sockfd, "RETRV msg_list[0]")
	  if (! *ends){
	   break;
	  }
	  else{
	   arg4= ends.erase(0,1);
	  }
	 }
	}

	ifstream mfolder;
	mfolder.open(argv[5]);
	if (!(mfolder.good()){
	exit(4);
	}
	//mfolder.write();//to decide arguements of write
	string arg1(argv[1]),username(argv[2]),password(argv[3]);
	int col = arg1.find(":");
	string IP = arg1.substr(0,col);
	string port_s = arg1.substr(col+1);
	int port;
	try{
		port = stoi(port_s);
	}
	catch(...){
		port = 0;
	}

//Initialize sockaddr
	sockaddr_in server_address;
    memset(&server_address, '0', sizeof(server_address));
	server_address.sin_family = AF_INET;
	server_address.sin_addr.s_addr = inet_addr(IP.c_str());
	server_address.sin_port = htons(port);
	int size_addr = sizeof(struct sockaddr);

//Set up socket
	int sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if(sockfd == 0){
		cerr<<"Couldn't form socket\n";
		exit(2);
	}

//Connect to server
	if(connect(sockfd,  (struct sockaddr *)&server_address, sizeof(struct sockaddr)) < 0){
		cerr<<"Connection to server failed\n";
		exit(2);
	}
	else{
		cout<<"ConnectDone: "<<argv[1]<<endl;
	}

//Login
	if(send_login(username,password,sockfd)<0){
		return 0;
	}
	int s1=send_message(sockfd, "LIST");
	string r1 = rec_message(sockfd);
    cout<<r1;

//Download the message using message list
    while(msg_list.size() != 0){
     string acc = to_string(msg_list[0]);
     int s2= send_message(sockfd, "RETRV "+acc);
     string r2 = recieve_msg;
     if (s2>0){
      string prepath(argv[5]);
      //prepath.append("/");
      const char* path = prepath.c_str();
	  DIR* pathdir = NULL;
	  struct dirent* pt = NULL;
	  pathdir = opendir(path);
      if (pathdir == NULL){
       int check = mkdir(argv[5]);
      }
      cout<<"Downloaded Message "<<acc;
     }
     msg_list.erase(msg_list.begin()+0,msg_list.begin()+1);
    }
   /* for(int i= 0; i<num; i++)
	{
        if(send_message(sockfd, "RETRV "+ to_string(mess[i]))<0){
            return 0;
        }
        int buffersize = 0, recv_size = 0,size = 0, read_size, write_size, packet_index =1,stat;

        char filearray[10241];
        FILE *file;

        //Find the size of the image
        do{
        stat = read(sockfd, &size, sizeof(int));
        }while(stat<0);

        printf("Packet received.\n");
        printf("Packet size: %i\n",stat);
        printf("file size: %i\n",size);
        printf(" \n");

        char buffer[] = "Got it";

//       string filename = rec_message(sockfd);
        string filename = to_string(mess[i]);
        filename = string(mess_database) + filename;
        int n = filename.size();
        char u[n+1];
        strcpy(u, filename.c_str());

        file = fopen((const char*)u, "w");
        if(file==NULL)
        {
            cout<<"Error";
            exit(-1);
        }

        while(recv_size<size)
        {
            do{
               read_size = read(sockfd,filearray, 10241);
               cout<<"a"<<endl;
            }while(read_size <0);

            printf("Packet number received: %i\n",packet_index);
        printf("Packet size: %i\n",read_size);


        //Write the currently read data into our image file
         write_size = fwrite(filearray,1,read_size, file);
         printf("Written file size: %i\n",write_size);

             if(read_size !=write_size) {
                 printf("error in read write\n");    }


             //Increment the total number of bytes read
             recv_size += read_size;
             packet_index++;
             printf("Total received file size: %i\n",recv_size);
             printf(" \n");
             printf(" \n");
    }
    fclose(file);
    cout<<"Successfully received"<<endl;

        } */

//Quit
	int s3=send_message(sockfd,"quit");
	close(sockfd);

}

////////////////////////
//FUNCTION DEFINITIONS//
////////////////////////

int send_message(int sockfd, string message){
//Prints error message if not sent and returns -10
//Else returns number of bytes sent
	//Convert to char array
	char mess[message.length()+1];
	for (int i=0; i< message.length();i++){
		mess[i] = message[i];
	}
	mess[message.length()] = '\0';
	//Send char array
	int x = send(sockfd,&mess,strlen(mess),0);
	if(x<0){
		cerr<<"Message not sent: "<<message<<endl;
		return -10;
	}
	return x;
}

string rec_message(int sockfd){
//Receives upto chunk_size bytes and returns string
//If error receiving, prints appropriate error and returns string "error"
	char buff[chunk_size] = {0};
	int read_bytes = recv(sockfd,(void*)&buff, chunk_size, 0);
	if(read_bytes == 0){
		cerr<<"Connection closed before reading\n";
		exit(3);
	}
	else if(read_bytes == -1){
		cerr<<"Error on receive\n";
		return "error";
	}
	string buf(buff);
	return buf;
}

int send_login(string username, string password, int sockfd){
//Sends login message
//Returns -10 and prints "Login failed" if failed
	if(send_message(sockfd,"User: "+username+" Pass: "+password) < 0){
		cout<<"Login failed"<<endl;
		return -10;
	}
	return 0;
}
