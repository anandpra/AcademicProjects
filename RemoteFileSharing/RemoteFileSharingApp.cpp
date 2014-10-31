#include "iostream"
#include "cstdlib"	
#include "cstdio"	
#include <sys/types.h>         
#include <sys/socket.h>
#include <sys/time.h>
#include "netinet/in.h"
#include "netinet/tcp.h"
#include "string.h"
#include "strings.h"
#include "arpa/inet.h"
#include "sstream"
#include <netdb.h>
#include <unistd.h>
#include <algorithm>

#define CLIENTTYPE 1
#define SERVERTYPE 0
using namespace std;
class Client;

static int clientUniqueId=0;
const int controlByteSize=30;
const int packetSize=512;

template<class T>
class LinkedList{
private:
	class Node{
	public :
		T * data;
		Node * next;
		Node(T* obj){
			data=obj;
		}
		bool equals(T* obj){
			if(obj->equals(data)){
				return true;
			}else{
				return false;
			}

		}
	};
	Node * head;
	Node * tail;
	Node * currentPointer;

public:
	LinkedList<T>(){
		head=NULL;
		tail=NULL;
		currentPointer=NULL;
	}
	bool add(T* newNode){
		if(newNode!=NULL){
			if(head==NULL){
				head=new Node(newNode);
				head->next=NULL;
				tail=head;
			}else{
				Node * tempNode=new Node(newNode);
				tempNode->next=NULL;
				tail->next=tempNode;
				tail=tempNode;
			}
			return true;
		}
		else{
			return false;
		}
	}
	bool remove(T* oldNode){
		Node* temp=NULL,*prev=NULL;
		temp=head;
		while(temp!=NULL){
			if(temp->equals(oldNode)){
				if(head==temp){
					//head node
					head=head->next;
				}
				if(tail==temp){
					tail=prev;
				}
				if(prev!=NULL)
					prev->next=temp->next;
				temp->next=NULL;
				//delete temp;
				return true;
			}
			prev=temp;
			temp=temp->next;
		}
		return false;

	}
	void seekFirst(){
		currentPointer=NULL;
	}
	bool hasNext(){
		if(currentPointer==NULL){
			if(head==NULL)
				return false;
			else
				return true;
		}else{

			if(currentPointer->next==NULL)
				return false;
			else
				return true;
		}

	}

	T * next(){
		if(hasNext()){
			if(currentPointer==NULL)
				currentPointer=head;
			else
				currentPointer=currentPointer->next;
			return currentPointer->data;
		}
		return NULL;
	}
	void printList(){
		Node*temp=head;
		cout<<"Printing List elements:\n";
		while(temp!=NULL){
			//		cout<<*temp->data<<" ";
			temp=temp->next;
			cout<<"1\n";

		}
	}
};

static int connectionIdCounter=0;
class Connection{
private:
	int connectionId;
	int socketDescriptor;
	bool activeFlag;
public:
	Connection(int descriptor){
		connectionId=++connectionIdCounter;
		activeFlag=true;
		socketDescriptor=descriptor;
	}
	Connection(){
		activeFlag=false;
	}
	int getConnectionID(){
		return connectionId;
	}
	void setConnectionID(int id){
		connectionId=id;

	}
	int getSocketDescriptor(){
		return socketDescriptor;
	}
	void setSocketDescriptor(int socket){
		socketDescriptor=socket;
	}

	bool isActive(){
		return activeFlag;
	}
	void setActive(bool status){
		activeFlag=status;
	}
};


class Program{
protected :
	string portNum;
	string ipAddress;
	string hostName;

	LinkedList<Client>* registerClientList;

public :
	Program(){
		registerClientList= new LinkedList<Client>();
	}
	virtual void display(){};
	virtual void help(){};
	void terminate(){

	}
	string getPortNum(){
		return portNum;
	}
	void setPortNum(string portNumParam){
		//	strcpy(portNum,portNumParam);
		portNum=portNumParam;
	}
	string getIPAddress(){
		return ipAddress;
	}
	void setIPAddress(string ipAddressParam){
		ipAddress=ipAddressParam;
	}
	void exitIfError(int descriptor,const char msg[]){
		if(descriptor==-1){
			cout<<"Error with "<<msg<<"..Exiting\n";
			exit(-1);
		}
	}
	string* split(string inputCommand){
		string* strArray =new string[20];
		istringstream input(inputCommand);
		int i=0;
		do{
			input>>strArray[i++];
		}while(input);
		//i=0;
		if(i!=0){
			i=i-1;
			//improve this logic
			while(i<strArray->size()){
				strArray[i++]="#";
			}
			fflush(stdout);
		}

		return strArray;
	}
	string* split(char * inputString,const char * delim,string * tokenStrings){

		int i=0;
		//string *tokenStrings=new string[10];
		char * token=strtok(inputString,delim);
		while(token!=NULL){
			tokenStrings[i++]=token;
			//	cout<<"Token:"<<tokenStrings[i-1]<<endl;
			token=strtok(NULL,delim);
		}
		return tokenStrings;
	}
	void sendMessage(int socketDescriptor,string controlMsg, int msgSize){
		//		cout<<"Inside sendMessage:"<<controlMsg<<endl;
		const char * controlMsgArr;
		int bytesTransferred=0;
		controlMsgArr=controlMsg.c_str();
		do{
			bytesTransferred+=send(socketDescriptor, controlMsgArr+bytesTransferred,msgSize-bytesTransferred,0);
			//cout<<"bytesTransferred:"<<bytesTransferred<<endl;
		}while(bytesTransferred!=msgSize);
	}
	void sendMessage(int socketDescriptor,char * fileChunk, int chunkSize){
		//	cout<<"Inside sendMessage"<<endl;
		int bytesTransferred=0;
		do{
			bytesTransferred+=send(socketDescriptor, fileChunk+bytesTransferred,chunkSize-bytesTransferred,0);
			//cout<<"bytesTransferred:"<<bytesTransferred<<endl;
		}while(bytesTransferred!=chunkSize);
		//	cout<<"Sending chunk of size:"<<chunkSize<<" bytes"<<endl;
	}
	int readControlMsg(int clientDescriptor, string *tokenStrings){
		int bytesRead=0;
		int returnBytevalue=0;
		char controlMsgArr[controlByteSize];
		do{
			returnBytevalue=recv(clientDescriptor, controlMsgArr+bytesRead,controlByteSize-bytesRead,0);
			if(returnBytevalue==0){
				//	cout<<"Connection closed by Peer/Client"<<endl;
				terminateConnection(clientDescriptor);
				return 0;
			}else if(returnBytevalue==-1){
				cout<<"Error while reading data from connection"<<endl;
				return -1;
			}else{
				bytesRead+=returnBytevalue;
				//			cout<<"bytesRead:"<<bytesRead<<endl;
			}
		}while(bytesRead!=controlByteSize);
		split(controlMsgArr,"*",tokenStrings);
		return 1;
	}
	int readMessage(int clientDescriptor, char * readBuffer,int readSize){
		//		cout<<"***************************************************************************************"<<endl;
		int bytesRead=0;
		int returnBytevalue=0;
		do{
			returnBytevalue=recv(clientDescriptor, readBuffer+bytesRead,readSize-bytesRead,0);
			if(returnBytevalue==0){
				///connection closed
				//need to remove this socket from connection object in client list
				terminateConnection(clientDescriptor);
				return 0;
			}else if(returnBytevalue==-1){
				cout<<"Error while reading data from connection"<<endl;
				return -1;
			}else{
				bytesRead+=returnBytevalue;
			}
			//or EOF has reached
		}while(bytesRead!=readSize);
		//	cout<<"Data File chunk of packetSize read:"<<bytesRead<<endl;

		return 1;
	}
	void printCreator(){
		cout<<"Created By:"<<endl;
		cout<<"\tName:\t\tAnand Prakash Tiwary"<<endl;
		cout<<"\tUBIT Name:\tanandpra"<<endl;
		cout<<"\tUB Email:\tanandpra@buffalo.edu"<<endl;
	}
	virtual int terminateConnection(int socketDescriptor){

	}
	int setFDS(fd_set*,int );

	string getHostNameFromIP(string ip){
		//referenced from beej
		struct hostent *he;
		struct in_addr addr;
		inet_pton(AF_INET,ip.c_str(),&addr);
		he=gethostbyaddr(&addr,sizeof(addr),AF_INET);

		return he->h_name;


	}

	string getIPFromHostName(string ipOrHost,string portNum){
		//Referenced from beej
		struct addrinfo hints;
		int serverSocketfd;
		struct addrinfo *res,*p;
		int con;
		memset(&hints,0,sizeof(hints))		;
		hints.ai_family=AF_UNSPEC;
		hints.ai_socktype=SOCK_STREAM;
		int resultvalue=getaddrinfo(ipOrHost.c_str(),portNum.c_str(),&hints,&res);
		if(resultvalue!=0){
			cout<<"Error occured.\n"<<gai_strerror(resultvalue)<<endl;
			return "";
		}
		char ipstr[INET6_ADDRSTRLEN];
		for(p=res;p!=NULL;p=p->ai_next){
			void *addr;
			char *ipver;
			if(p->ai_family==AF_INET){
				struct sockaddr_in *ipv4=(struct sockaddr_in*)p->ai_addr;
				addr=&(ipv4->sin_addr);
				inet_ntop(p->ai_family,addr,ipstr,sizeof(ipstr));
				return ipstr;
			}
		}
	}
};


class Client: public Program{
private:
	string clientId;
	Connection* connectionObj;
	int activeConnections;
	string serverIpAddr;
	string serverPortNum;
public :
	Client(){
		activeConnections=0;
		connectionObj=new Connection();
	}

	void registerAndAcceptCommands(int socketDescriptor){

		struct sockaddr_in client;
		unsigned int len=sizeof(struct sockaddr_in);
		int peerDescriptor;
		fd_set readfds;
		Client * registerClientObj=NULL;
		int otherClientDescriptor;
		int fd_max=socketDescriptor;
		string inputCommand="";
		cout<<"Client Started...."<<endl;
		while(1){
			fd_max=setClientFDS(&readfds,socketDescriptor);
			select(fd_max+1,&readfds,NULL,NULL,NULL);
			//check for server
			if(connectionObj->isActive() && FD_ISSET(connectionObj->getSocketDescriptor(),&readfds)){
				//Incoming Message from Server
				//		cout<<"Incoming Message from Server"<<endl;
				parseControlMsgAndCallFunction(connectionObj->getSocketDescriptor());
			}
			//check for new connections
			if(FD_ISSET(socketDescriptor,&readfds)){
				//	cout<<"Incoming Connection!\n";
				exitIfError(peerDescriptor=accept(socketDescriptor,(struct sockaddr*)&client, &len),"Accept call");
				if(activeConnections<3){
					Client *newClient=NULL;
					if(addNewPeerConnection(peerDescriptor,newClient)!=-1){
						activeConnections++;
						cout<<"Connection Established with peer on "<<getHostNameFromIP(newClient->getIPAddress())<<" IP:"<<newClient->getIPAddress()<<" PORT:"<<newClient->getPortNum()<<endl;
					}else{
						//this connection should be rejected
						cout<<"This is not a registered Client. Closing the connection!"<<endl;
						close(socketDescriptor);
					}
				}else{
					/*Find out if you can reject connections directly. If yes, then no need to accept connections if u have
					 * to reject it.
					 * */
					activeConnections--;
					cout<<"Already have max number of connections. Can't accept more\n";
					close(peerDescriptor);
				}
			}
			//Check for console input
			if(FD_ISSET(0,&readfds)){
				getline(cin,inputCommand);
				executeCommand(split(inputCommand));
			}
			//while each clients in list, send their clientID, IP n PORT
			registerClientList->seekFirst();
			while(registerClientList->hasNext()){
				registerClientObj=registerClientList->next();
				if(registerClientObj->connectionObj->isActive()){
					otherClientDescriptor=registerClientObj->connectionObj->getSocketDescriptor();
					if(FD_ISSET(otherClientDescriptor,&readfds)){
						//Incoming message from peer
						//			cout<<"Incoming message from:"<<registerClientObj->getClientID()<<endl;
						parseControlMsgAndCallFunction(otherClientDescriptor);


					}
				}
			}


		}

	}

	int establishConnection(string ip,string portNum,int type){
		//Referenced from Beej
		struct addrinfo hints;
		struct addrinfo *res;
		int serverSocketfd;
		int con;
		memset(&hints,0,sizeof(hints))		;
		hints.ai_family=AF_UNSPEC;
		hints.ai_socktype=SOCK_STREAM;
		int resultvalue=getaddrinfo(ip.c_str(),portNum.c_str(),&hints,&res);
		if(resultvalue!=0){
			cout<<"Error occured.\n"<<gai_strerror(resultvalue)<<endl;
			exit(-1);
		}
		exitIfError(serverSocketfd=socket(res->ai_family,res->ai_socktype,res->ai_protocol),"Socket");
		exitIfError(con=connect(serverSocketfd,res->ai_addr,res->ai_addrlen),"Connect");
		return serverSocketfd;
	}

	bool validPeer(string ip,string portNum){

		Client* clientObj=NULL;
		registerClientList->seekFirst();
		while(registerClientList->hasNext()){
			clientObj=registerClientList->next();
			if(clientObj->getIPAddress().compare(ip)==0 && clientObj->getPortNum().compare(portNum)==0){
				//We have found the client :)
				if(clientObj->getConnectionObj()->isActive()){
					cout<<"You are already connected to this client. New connection will not be established"<<endl;;
					return false;
				}else
					return true;
			}
		}
		cout<<"Not a registered Client. Please check IP/Host name and try again."<<endl;
		return false;

	}
	int addNewPeerConnection(int socketDescriptor,Client* &clientObj){
		/*
		 * Take client id from peer
		 * Find the client object in the list and add the peerDescriptor to the corresponding connection object
		 * Set connection active flag to true;
		 */

		string tokenStrings[5];
		//	Client* clientObj=NULL;
		if(readControlMsg(socketDescriptor,tokenStrings)==0)
			return -2;
		//cout<<"Message read\n";
		if(true){
			registerClientList->seekFirst();
			while(registerClientList->hasNext()){
				clientObj=registerClientList->next();
				if(clientObj->getIPAddress().compare(tokenStrings[2])==0 && clientObj->getPortNum().compare(tokenStrings[3])==0){
					//We have found the client :)
					clientObj->setConnectionObj(new Connection(socketDescriptor));
					return 0;
				}
			}
		}
		return -1;
	}
	void connectToPeer(string peerIp,string peerPortNum){
		if((ipAddress.compare(peerIp)==0) && (portNum.compare(peerPortNum)==0)){
			cout<<"You are trying to establish connection with yourself. Not allowed."<<endl;
			return;
		}
		else if(!connectionObj->isActive()){
			cout<<"You have not registered with the server yet. Please register first."<<endl;
			return;
		}

		if(activeConnections<3){
			//check if the connection to the given IP n port is already present. If yes, then reject this connection
			cout<<"Trying to establish connection..."<<endl;
			//Adding code
			if(!validPeer(peerIp,peerPortNum)){
				return ;
			}
			//end
			int serverSocketfd=establishConnection(peerIp,peerPortNum,1);
			if(serverSocketfd!=-1){
				//Find out the client
				//Set the corresponding connection object.
				registerClientList->seekFirst();
				Client* clientObj=NULL;
				//Avoid this by passing ClientObj* to establish Connection and setting it there.
				while(registerClientList->hasNext()){
					clientObj=registerClientList->next();
					if(clientObj->getIPAddress()==peerIp && clientObj->getPortNum()==peerPortNum){
						//We have found the client :)
						clientObj->setConnectionObj(new Connection(serverSocketfd));
					}
				}
				sendControlMsg('c',serverSocketfd);
				cout<<"Connection established! "<<endl;
				activeConnections++;
			}
		}else{
			cout<<"Already have max number of connections. Cannot establish more!"<<endl;
		}
	}
	void registerToServer(string serverIp,string serverPort){
		//Code directly taken from beej..check if this needs to be changed
		//Check if the client is already registered
		if(connectionObj->isActive()){
			cout<<"You are already registered with Server IP:"<<serverIpAddr<<" Port:"<<serverPortNum<<endl;
			return;
		}else if((ipAddress.compare(serverIp)==0) && (portNum.compare(serverPort)==0)){
			cout<<"You are trying to establish connection with yourself. Not allowed."<<endl;
			return;
		}
		string tokenStrings[5];
		int serverSocketfd=establishConnection(serverIp,serverPort,0);
		setConnectionObj(new Connection(serverSocketfd));
		serverIpAddr=serverIp;
		serverPortNum=serverPort;
		//cout<<"Server Connection is active?"<<connectionObj->isActive()<<endl;
		sendControlMsg('c',serverSocketfd);
		//receive client ID;
		if(readControlMsg(serverSocketfd,tokenStrings)==0)
			return;
		//cout<<"Setting client ID:"<<tokenStrings[1]<<endl;
		setClientID(tokenStrings[1]);
		cout<<"Registration with server successful....."<<endl;
		//write code for updating client list
		if(tokenStrings[2]=="1"){
			//	cout<<"Updating registered client list....."<<endl;
			if(readControlMsg(serverSocketfd,tokenStrings)==0)
				return;
			while(tokenStrings[0]!="k"){
				addClient(tokenStrings);
				if(readControlMsg(serverSocketfd,tokenStrings)==0)
					return;
			}
		}else{
			cout<<"You are the first client. You don't need to update your registered client list right now."<<endl;
		}

	}
	void addClient(string * tokenStrings){
		Client* clientObj=new Client();
		cout<<"Adding client "<<getHostNameFromIP(tokenStrings[2])<<" IP:"<<tokenStrings[2]<<" Port:"<<tokenStrings[3]<<endl;
		clientObj->setClientID(tokenStrings[1]);
		clientObj->setIPAddress(tokenStrings[2]);
		clientObj->setPortNum(tokenStrings[3]);
		registerClientList->add(clientObj);
	}
	void removeClient(string * tokenStrings){
		//add new client
		Client* clientObj=new Client();
		cout<<"Removing client "<<getHostNameFromIP(tokenStrings[2])<<" IP:"<<tokenStrings[2]<<" Port:"<<tokenStrings[3]<<endl;
		//Added 3.51 am 27 2
		if(clientObj->getConnectionObj()->isActive())
			activeConnections--;
		clientObj->setClientID(tokenStrings[1]);
		clientObj->setIPAddress(tokenStrings[2]);
		clientObj->setPortNum(tokenStrings[3]);
		registerClientList->remove(clientObj);


	}
	void parseControlMsgAndCallFunction(int socketDescriptor){
		//Handle Upload, Download and Terminate
		string tokenStrings[5];
		Client* clientObj=NULL;
		//	cout<<"Inside parseControlMsgAndCallFunction "<<endl;
		if(readControlMsg(socketDescriptor,tokenStrings)==0)
			return;
		switch(tokenStrings[0][0]){

		case 'a':
			addClient(tokenStrings);
			break;
		case 'r':
			//	cout<<"Trying to remove client"<<endl;
			removeClient(tokenStrings);
			break;
		case 'u':
			registerClientList->seekFirst();
			while(registerClientList->hasNext()){
				clientObj=registerClientList->next();
				if(clientObj->getConnectionObj()->getSocketDescriptor()==socketDescriptor){
					break;
				}
			}
			cout<<"File Uploading request received from Connection "<<clientObj->getConnectionObj()->getConnectionID()<<endl;
			receiveFile(socketDescriptor,atoi(tokenStrings[1].c_str()),clientObj);

			break;
		case 'd':
			registerClientList->seekFirst();
			while(registerClientList->hasNext()){
				clientObj=registerClientList->next();
				if(clientObj->getConnectionObj()->getSocketDescriptor()==socketDescriptor){
					break;
				}
			}
			cout<<"File Download request received from Connection "<<clientObj->getConnectionObj()->getConnectionID()<<endl;

			upload(clientObj->getConnectionObj()->getConnectionID(),tokenStrings[1],true);
			break;

		}

	}
	void receiveFile(int socketDescriptor, int fileSize,Client* clientObj){
		char fileDataPacket[packetSize];
		int totalBytesRead=fileSize;
		struct timeval begin,end;
		gettimeofday(&begin,NULL);
		if(readMessage(socketDescriptor,fileDataPacket,packetSize-controlByteSize)==0)
			return;
		string file;
		char c;
		int i=0;
		while((c=fileDataPacket[i])!='*')
		{	file.append(1,c);
		i++;
		}
		FILE* writeFp= fopen(file.c_str(),"wb");
		if(writeFp==NULL){
			cout<<"File Receive failed"<<endl;
			return;
		}
		int byteWritten=0;
		if(fileSize>=packetSize-file.size()-1-controlByteSize){
			byteWritten=fwrite(fileDataPacket+file.size()+1/*Remove this number*/,sizeof(char),packetSize-file.size()-1-controlByteSize,writeFp);
		}else{
		}
		fileSize-=byteWritten;
		while(fileSize!=0){
			if(readMessage(socketDescriptor,fileDataPacket,packetSize)==0)
				return;
			if(fileSize>=packetSize){
				byteWritten=fwrite(fileDataPacket,sizeof(char),packetSize,writeFp);

			}else{
				byteWritten=fwrite(fileDataPacket,sizeof(char),fileSize,writeFp);
				fileSize-=byteWritten;
				break;
			}
			fileSize-=byteWritten;
		}
		gettimeofday(&end,NULL);
		double timeElapsed=(end.tv_sec-begin.tv_sec)+(end.tv_usec-begin.tv_usec)/1000000.0;

		fclose(writeFp);
		cout<<"File Successfully received."<<endl;

		string myHost=getHostNameFromIP(getIPAddress());//
		cout<<"Rx("<<myHost<<"):"<<getHostNameFromIP(clientObj->getIPAddress())<<"->"<<myHost<<",File size:"<<totalBytesRead<<" Bytes,Time Taken:"<<timeElapsed<<"sec Rx Rate:"<<((double)totalBytesRead*8.0/(timeElapsed*1000))<<" Kbits/second"<<endl;
		fflush(stdout);

	}
	int setClientFDS(fd_set* readfds,int myListenPortsocketDescriptor){
		int maxDescriptor=setFDS(readfds, myListenPortsocketDescriptor);
		//this is for server socket descriptor
		if(connectionObj->isActive()){
			FD_SET(connectionObj->getSocketDescriptor(),readfds);
			if(maxDescriptor<connectionObj->getSocketDescriptor()){
				maxDescriptor=connectionObj->getSocketDescriptor();
			}
		}
		return maxDescriptor;
	}
	void sendControlMsg(char controlChar,int peerDescriptor){
		string controlMsg;

		switch(controlChar){
		case 'c':
			controlMsg.append("c*");
			controlMsg.append(clientId);
			controlMsg.append("*");
			controlMsg.append(ipAddress);
			controlMsg.append("*");
			controlMsg.append(portNum);
			controlMsg.append("*");
			while(controlMsg.size()<controlByteSize){
				controlMsg.append(" ");
			}
			sendMessage(peerDescriptor,controlMsg,controlByteSize);
			break;
		default:
			cout<<"No Control Parameter specified\n";
			break;
		}

	}

	void terminate(string connectionId){
		//later change connectionID type to string instead of int..It will also handle invalid input
		int conId=atoi(connectionId.c_str());
		if(conId==0){
			cout<<"Invalid Connection ID. Please try again"<<endl;
			return;
		}
		if(conId==connectionObj->getConnectionID()){
			/*Client is trying to end its connection with the server.
			 *The result will be same as EXIT command
			 */
			/*Change suggested by TA.Client should not be allowed to terminate its connection with the server.
			 * Commenting the gracefulExit() funtion call and printing out the "Connection Not allowed" Message.
			 * */
			//gracefulExit();
			cout<<"Sorry. You cannot terminate your connection with the server."<<endl;
			return;
		}
		Client* clientObj;
		Connection* conObj;
		registerClientList->seekFirst();
		while(registerClientList->hasNext()){
			clientObj=registerClientList->next();
			conObj=clientObj->getConnectionObj();
			if(conObj->getConnectionID()==conId){
				//we have found the connection.
				close(conObj->getSocketDescriptor());
				clientObj->setConnectionObj(new Connection());
				//delete old connection obj
				activeConnections--;
				cout<<"Connection with client ID:"<<clientObj->getClientID()<<" IP:"<<clientObj->getIPAddress()<<" Port:"<<clientObj->getPortNum()<<" has been terminated."<<endl;
				return;
			}
		}
		cout<<"Provided Connection Id is Invalid."<<endl;
	}
	int terminateConnection(int socketDescriptor){
		//	cout<<"Inside Client Terminate connection"<<endl;
		if(connectionObj->getSocketDescriptor()==socketDescriptor){
			/* Connection Closed by Server
			 * As of now call gracefull Exit.
			 * */
			cout<<"Server Exited. We have to exit too."<<endl;
			gracefulExit();
		}
		registerClientList->seekFirst();
		Client* clientObj;
		while(registerClientList->hasNext()){
			clientObj=registerClientList->next();
			Connection *conn=clientObj->getConnectionObj();
			if(conn->getSocketDescriptor()==socketDescriptor){
				//we have found the client
				cout<<"Client ID:"<<clientObj->getClientID()<<" IP:"<<clientObj->getIPAddress()<<" PortNum:"<<clientObj->getPortNum()<<" has terminated the connection"<<endl;
				close(socketDescriptor);
				clientObj->setConnectionObj(new Connection());
				//delete current connection OBJ
				activeConnections--;
				return 0;
			}

		}
		return -1;
	}
	void executeCommand(string inputCommand[]){
		//cout<<inputCommand[1];
		int i=0;
		for(i=0;i<inputCommand->size();i++){
			if(inputCommand[i].compare("#")==0){
				break;
			}
		}
		transform(inputCommand[0].begin(),inputCommand[0].end(),inputCommand[0].begin(),::toupper);

		if(inputCommand[0].compare("HELP")==0){

			help();

		}else if(inputCommand[0].compare("MYIP")==0){

			cout<<getIPAddress()<<"\n";

		}else if(inputCommand[0].compare("MYPORT")==0){
			cout<<getPortNum()<<"\n";
		}else if(inputCommand[0].compare("LIST")==0){

			list();

		}else if(inputCommand[0].compare("LISTFULL")==0){

			list(2);

		}else if(inputCommand[0].compare("REGISTER")==0){
			if(i<3){
				cout<<"Invalid number of inputs for this command. Please try again."<<endl;
				return;
			}
			if(atoi(inputCommand[2].c_str())==0){
				cout<<"Invalid Port number. Please try again."<<endl;
				return;
			}
			inputCommand[1]=getIPFromHostName(inputCommand[1],inputCommand[2]);
			if(inputCommand[1].compare("")==0){
				cout<<"Invalid IP/Host Name. Please try again"<<endl;
				return;
			}
			registerToServer(inputCommand[1],inputCommand[2]);

		}else if(inputCommand[0].compare("CONNECT")==0){
			if(i<3){
				cout<<"Invalid number of inputs for this command. Please try again."<<endl;
				return;
			}
			inputCommand[1]=getIPFromHostName(inputCommand[1],inputCommand[2]);
			if(inputCommand[1].compare("")==0){
				cout<<"Invalid IP/Host Name. Please try again"<<endl;
				return;
			}
			connectToPeer(inputCommand[1],inputCommand[2]);

		}else if(inputCommand[0].compare("TERMINATE")==0){
			if(i<2){
				cout<<"Invalid number of inputs for this command. Please try again."<<endl;
				return;
			}
			terminate(inputCommand[1]);

		}else if(inputCommand[0].compare("UPLOAD")==0){
			if(i<3){
				cout<<"Invalid number of inputs for this command. Please try again."<<endl;
				return;
			}
			int conId=atoi(inputCommand[1].c_str());
			if(conId==0){
				cout<<"Invalid Connection ID. Please try again."<<endl;
				return;
			}
			upload(conId,inputCommand[2],false);

		}else if(inputCommand[0].compare("DOWNLOAD")==0){
			if(i<3||i%2==0||i>=8){
				cout<<"Invalid number of inputs for this command. Please try again."<<endl;
				return;
			}
			download(inputCommand);

		}else if(inputCommand[0].compare("EXIT")==0){

			gracefulExit();

		}else if(inputCommand[0].compare("CREATOR")==0){
			printCreator();
		}else{
			cout<<"Invalid command. Type HELP\n";

		}

	}
	void gracefulExit(){
		/*Close all the active connections first
		 *Close server connection.This should trigger client remove operation at the server side
		 *and consequently should inform other registered clients
		 **/
		cout<<"Exit requested."<<endl;
		Client* clientObj;
		Connection* conObj;
		registerClientList->seekFirst();
		while(registerClientList->hasNext()){
			clientObj=registerClientList->next();
			conObj=clientObj->getConnectionObj();
			if(conObj->isActive()){
				//Active connection. Close it.
				cout<<"Closing the connection with client with IP:"<<clientObj->getIPAddress()<<" Port:"<<clientObj->getPortNum()<<endl;
				close(conObj->getSocketDescriptor());
				//delete old connection obj
			}
		}
		if(connectionObj->isActive()){
			close(connectionObj->getSocketDescriptor());
			cout<<"Closing the connection with server."<<endl;
		}
		cout<<"Gracefully exiting!"<<endl;
		exit(0);

	}

	void display(){
		cout<<"Client Object Created..PortNum:"<<portNum<<"\n\n";
	}
	bool equals(Client * obj){

		if(obj!=NULL)
			if(getClientID()==obj->getClientID())
				return true;
		return false;
	}
	Connection* getConnectionObj(){
		return connectionObj;
	}
	void setConnectionObj(Connection* obj){
		connectionObj=obj;
	}
	string getClientID(){
		return clientId;
	}
	void setClientID(string id){
		clientId=id;
	}
	void updateRegisteredClientList(){
		cout<<" Updating server list of client:"<<clientId<<"...\n";
	}

	void list(){

		registerClientList->seekFirst();
		if(!connectionObj->isActive()&&!registerClientList->hasNext()){
			cout<<"You have not established connection with anyone.\n";
			return;
		}
		cout<<"id\t"<<"Hostname\t\t\t"<<"Ip address\t"<<"Port No.\t"<<endl;

		if(connectionObj->isActive())
			cout<<connectionObj->getConnectionID()<<"\t"<<getHostNameFromIP(serverIpAddr)<<"\t\t"<<serverIpAddr<<"\t"<<serverPortNum<<"\t"<<endl;
		Client * currentClient;
		Connection * connObj;
		while(registerClientList->hasNext()){
			currentClient=registerClientList->next();
			connObj=currentClient->getConnectionObj();
			if(connObj->isActive())
				cout<<connObj->getConnectionID()<<"\t"<<getHostNameFromIP(currentClient->getIPAddress())<<"\t\t"<<currentClient->getIPAddress()<<"\t"<<currentClient->getPortNum()<<"\t"<<endl;
		}
	}
	//Remove it after testing
	void list(int i){
		registerClientList->seekFirst();
		if(!registerClientList->hasNext()){
			cout<<"Error...Empty List\n";
			return;
		}
		Client * currentClient;
		Connection * connObj;
		while(registerClientList->hasNext()){
			currentClient=registerClientList->next();
			connObj=currentClient->getConnectionObj();
			cout<<connObj->getConnectionID()<<"\t"<<getHostNameFromIP(currentClient->getIPAddress())<<"\t\t"<<currentClient->getIPAddress()<<"\t"<<currentClient->getPortNum()<<"\n";
		}
	}

	void download(string inputParams[]){

		//check for  a valid connectionID..it should exist and should not be with a server
		//create control MSGs for each download
		//add the descriptors to a select fds call
		//Send to each of the connections
		//wait in while loop for select..Exit when the download is complete.
		int connection;
		Client* clientObj;
		Connection * conn;
		//	string fileName;
		int connIndex=0;
		int socketDescriptors[3];
		FILE * filePointers[3];
		string controlMsg[3];
		bool flags[3]={false,false,false};
		int fileSize[3];
		int totalBytesTransferred[3];
		string fileName[3];
		int fd_max=0;
		fd_set readfds;
		string tokenStrings[5];
		char fileDataPacket[packetSize];
		struct timeval begin[3],end[3];
		Client* peerList[3];
		string onlyFileName;
		int fileNameIndex=0;
		for(int i=1;i<inputParams->size()&&inputParams[i].compare("#")!=0&&i<8;i=i+2){
			//change this later
			connection=atoi(inputParams[i].c_str());
			if(connection==0){
				cout<<"Invalid connection ID. Please try again."<<endl;
				return;
			}
			if(connectionObj->getConnectionID()==connection){
				cout<<"Error. You cannot download file from server."<<endl;
				return;
			}
			if((fileName[fileNameIndex]=inputParams[i+1]).compare("#")==0){
				cout<<"Error while fetching fileNames..cancelling.."<<endl;
				return;
			}
			registerClientList->seekFirst();
			while(registerClientList->hasNext()){
				clientObj=registerClientList->next();
				conn=clientObj->getConnectionObj();
				if(conn->getConnectionID()==connection){
					socketDescriptors[connIndex]=conn->getSocketDescriptor();
					peerList[connIndex]=clientObj;
					onlyFileName=fileName[fileNameIndex].c_str();
					int index=onlyFileName.find_last_of("/",onlyFileName.size());
					if(index!=-1){
						onlyFileName=onlyFileName.substr(index+1,onlyFileName.size()-index);
					}
					filePointers[connIndex]=fopen(onlyFileName.c_str(),"wb");
					//	cout<<"Complete path:"<<fileName[fileNameIndex]<<" only file name:"<<onlyFileName;
					//	filePointers[connIndex]=fopen(fileName[fileNameIndex].c_str(),"wb");
					if(filePointers[connIndex]==NULL){
						cout<<"Error while opening files for writing..cancelling.."<<endl;
						return;
					}

					controlMsg[connIndex].append("d*");
					if(fileName[fileNameIndex].size()>controlByteSize-3){
						controlMsg[connIndex].append(" ");
					}else{
						controlMsg[connIndex].append(fileName[fileNameIndex]);
					}
					controlMsg[connIndex].append("*");
					while(controlMsg[connIndex].size()<controlByteSize){
						controlMsg[connIndex].append(" ");
					}

					if(fd_max<socketDescriptors[connIndex])
						fd_max=socketDescriptors[connIndex];

					connIndex++;
					fileNameIndex++;
					break;
				}
			}
		}

		/*********************************ALL well till here*******************************/
		//Send ControlMsges
		for(int k=0;k<connIndex;k++){
			//cout<<"Sending file download request for file:"<<fileName[k]<<endl;
			cout<<"File Download started for "<<fileName[k]<<endl;
			sendMessage(socketDescriptors[k],controlMsg[k],controlByteSize);
			if(fileName[k].size()>controlByteSize-3){
				//	cout<<"fileName:"<<fileName[k]<<endl
				string largeFileName=fileName[k];
				largeFileName.append("*");
				while(largeFileName.size()<255){
					largeFileName.append(" ");
				}
				sendMessage(socketDescriptors[k],largeFileName,255);
				//	cout<<"File name too large. Sending separately.:"<<largeFileName<<endl;
			}
		}
		int downloadCompleted=0;
		while(downloadCompleted!=connIndex){

			FD_ZERO(&readfds);
			for(int m=0;m<connIndex;m++){
				FD_SET(socketDescriptors[m],&readfds);
			}

			select(fd_max+1,&readfds,NULL,NULL,NULL);
			for(int i=0;i<connIndex;i++){
				if(FD_ISSET(socketDescriptors[i],&readfds)){
					//read
					//when byte complete, close n output, swap socketdescriptor with last connIndex. dec connIndex
					//if 0, error,
					if(flags[i]==false){
						//this is the first message
						gettimeofday(&begin[i],NULL);
						if(readControlMsg(socketDescriptors[i],tokenStrings)==0)
						{
							cout<<"Error while download"<<endl;
							fclose(filePointers[i]);
							downloadCompleted++;
							fflush(stdout);
							return;
						}else if(tokenStrings[0].compare("e")==0){
							cout<<"Error downloading file "<<fileName[i]<<"Exiting.."<<endl;
							fclose(filePointers[i]);
							fflush(stdout);
							downloadCompleted++;
							continue;
						}

						fileSize[i]=atoi(tokenStrings[1].c_str());
						totalBytesTransferred[i]=fileSize[i];
						fflush(stdout);
						if(readMessage(socketDescriptors[i],fileDataPacket,packetSize-controlByteSize)==0)
							return;

						char c;
						int j=0;
						while((c=fileDataPacket[j])!='*')
							j++;
						if(fileSize[i]>packetSize-j-1-controlByteSize){
							fileSize[i]-=fwrite(fileDataPacket+j+1,sizeof(char),packetSize-j-1-controlByteSize,filePointers[i]);
						}else{
							fileSize[i]-=fwrite(fileDataPacket+j+1,sizeof(char),fileSize[i],filePointers[i]);
							fclose(filePointers[i]);
							downloadCompleted++;
							gettimeofday(&end[i],NULL);
							double timeElapsed=(end[i].tv_sec-begin[i].tv_sec)+(end[i].tv_usec-begin[i].tv_usec)/1000000.0;
							string myHost=getHostNameFromIP(getIPAddress());
							cout<<"Rx("<<myHost<<"):"<<getHostNameFromIP(peerList[i]->getIPAddress())<<"->"<<myHost<<",File size:"<<totalBytesTransferred[i]<<" Bytes,Time Taken:"<<timeElapsed<<"sec Tx Rate:"<<((double)totalBytesTransferred[i]*8.0/(timeElapsed*1000))<<" Kbits/second"<<endl;
							cout<<"File Download completed for "<<fileName[i]<<endl;
							fflush(stdout);
						}
						//	cout<<"Current FileSize "<<fileSize[i];
						flags[i]=true;

					}else{
						//this is the later packets
						if(readMessage(socketDescriptors[i],fileDataPacket,packetSize)==0)
							return;
						if(fileSize[i]>packetSize){
							fileSize[i]-=fwrite(fileDataPacket,sizeof(char),packetSize,filePointers[i]);
						}else{
							fileSize[i]-=fwrite(fileDataPacket,sizeof(char),fileSize[i],filePointers[i]);
							//i shd close
							fclose(filePointers[i]);
							gettimeofday(&end[i],NULL);
							double timeElapsed=(end[i].tv_sec-begin[i].tv_sec)+(end[i].tv_usec-begin[i].tv_usec)/1000000.0;
							string myHost=getHostNameFromIP(getIPAddress());
							cout<<"Rx("<<myHost<<"):"<<getHostNameFromIP(peerList[i]->getIPAddress())<<"->"<<myHost<<",File size:"<<totalBytesTransferred[i]<<" Bytes,Time Taken:"<<timeElapsed<<"sec Tx Rate:"<<((double)totalBytesTransferred[i]*8.0/(timeElapsed*1000))<<" Kbits/second"<<endl;
							cout<<"File Download completed for "<<fileName[i]<<endl;
							downloadCompleted++;
							fflush(stdout);

						}

					}


				}

			}

		}
		cout<<"Download operation completed."<<endl;


	}

	void upload(int conId,string fileName,bool downloadFlag){
		//check for  a valid connectionID..it should exist and should not be with a server
		//check for a valid fileName
		//create a control MSG
		//Open File..
		//In first read, read (Packet size- ControlMsgsize) byte, and send
		//For further call, read Packet size bytes and send
		//the first byte will always be a continuation byte.'1'- One more packet coming. '0'- this is the last packet.

		//	int conId=atoi(connectionId.c_str());
		Client *clientObj=NULL;
		Connection* connObj=NULL;
		int socketDescriptor;
		FILE * readFp;
		struct timeval begin,end;
		bool validConnectionFlag=false;
		char fileNameHolder[255];
		string errorMsg;
		errorMsg.append("e*");
		errorMsg.append("*");
		while(errorMsg.size()<controlByteSize){
			errorMsg.append(" ");
		}
		if(connectionObj->getConnectionID()==conId){
			cout<<"You cannot send file to a server. Operation Not Allowed."<<endl;
			return;
		}else {
			registerClientList->seekFirst();
			while(registerClientList->hasNext()){
				clientObj=registerClientList->next();
				connObj=clientObj->getConnectionObj();
				if(connObj->isActive()&&(connObj->getConnectionID()==conId)){
					validConnectionFlag=true;
					break;
				}
			}
		}
		socketDescriptor=connObj->getSocketDescriptor();

		if(!validConnectionFlag){
			cout<<"Invalid connection Id provided. Please check and try again."<<endl;
			return;
		}
		//cout<<"Before opening fileName:"<<fileName<<endl;
		if(fileName.compare(" ")==0){
			//	cout<<"fileName too large. need to read again."<<endl;
			if(readMessage(socketDescriptor,fileNameHolder,255)==0)
				return;
			string tokenStrings[5];
			split(fileNameHolder,"*",tokenStrings);
			fileName=tokenStrings[0];
			//		cout<<"File name obtained in the next read:"<<fileName<<endl;

		}
		readFp= fopen(fileName.c_str(),"rb");
		if(readFp==NULL){

			cout<<"Invalid File. Please try again."<<endl;
			if(downloadFlag==true)
				sendMessage(socketDescriptor,errorMsg,controlByteSize);
			return;
		}
		int index=fileName.find_last_of("/",fileName.size());
		if(index!=-1){
			fileName=fileName.substr(index+1,fileName.size()-index);
		}
		//	cout<<"after opening file fileName:"<<fileName<<endl;

		char fileDataPacket[packetSize];
		int fileSize;
		fseek(readFp,0L,SEEK_END);
		fileSize=ftell(readFp);
		stringstream val;
		val<<fileSize;
		fseek(readFp,0L,SEEK_SET);
		string controlMsg;
		controlMsg.append("u*");
		controlMsg.append(val.str());
		controlMsg.append("*");
		while(controlMsg.size()<controlByteSize){
			controlMsg.append(" ");
		}

		//	cout<<"Control Msg1:"<<controlMsg<<endl;
		controlMsg.append(fileName);
		controlMsg.append("*");
		//cout<<"Control Msg2:"<<controlMsg<<endl;
		strcpy(fileDataPacket,controlMsg.c_str());
		//cout<<"String length:"<<strlen(fileDataPacket)<<endl;

		int bytesRead=0;int totalBytesTransferred=0;
		bytesRead=fread(fileDataPacket+strlen(fileDataPacket),sizeof(char),packetSize-strlen(fileDataPacket),readFp);
		totalBytesTransferred=packetSize;
		//Sending the first chunk with header info
		cout<<"File Uploading Started.."<<"File Size:"<<fileSize<<endl;
		gettimeofday(&begin,NULL);
		sendMessage(socketDescriptor,fileDataPacket,packetSize);
		while((bytesRead=fread(fileDataPacket,sizeof(char),packetSize,readFp))!=0){
			//sending the rest of the file chunk
			sendMessage(socketDescriptor,fileDataPacket,packetSize);
		}
		gettimeofday(&end,NULL);
		double timeElapsed=(end.tv_sec-begin.tv_sec)+(end.tv_usec-begin.tv_usec)/1000000.0;
		cout<<"File Uploaded successfully."<<endl;
		string myHost=getHostNameFromIP(getIPAddress());
		cout<<"Tx("<<myHost<<"):"<<myHost<<"->"<<getHostNameFromIP(clientObj->getIPAddress())<<",File size:"<<fileSize<<" Bytes,Time Taken:"<<timeElapsed<<"sec Tx Rate:"<<((double)fileSize*8.0/(timeElapsed*1000))<<" Kbits/second"<<endl;
		fclose(readFp);

	}
	void help(){
		cout<<"Available Commands\t\n";
		cout<<"1. MYIP: Print my IP"<<"\t\t\t\t"<<"2. MYPORT: Print my Port\n";
		cout<<"3. REGISTER: Register with server"<<"\t\t"<<"4. CONNECT: Connect with other Client\n";
		cout<<"5. LIST: Print active connections information\t"<<"6. UPLOAD: Upload file to other client\n";
		cout<<"7. DOWNLOAD: Download file from other client"<<"\t"<<"8. TERMINATE: Terminate connection\n";
		cout<<"9. EXIT: Exit\n";
	}
};

int Program::setFDS(fd_set* readfds,int myListenPortsocketDescriptor){
	int maxDescriptor=myListenPortsocketDescriptor;
	FD_ZERO(readfds);
	FD_SET(myListenPortsocketDescriptor,readfds);
	FD_SET(0,readfds);
	Client *registerClientObj=NULL;
	int otherClientDescriptor;
	registerClientList->seekFirst();
	while(registerClientList->hasNext()){
		registerClientObj=registerClientList->next();
		if(registerClientObj->getConnectionObj()->isActive()){
			otherClientDescriptor=registerClientObj->getConnectionObj()->getSocketDescriptor();
			FD_SET(otherClientDescriptor,readfds);
			if(maxDescriptor<otherClientDescriptor){
				maxDescriptor=otherClientDescriptor;
			}
		}
	}

	return maxDescriptor;
}


class Server: public Program{
public :
	void registerClientAndAcceptCommands(int socketDescriptor){

		struct sockaddr_in client;
		unsigned int len=sizeof(struct sockaddr_in);
		int clientDescriptor;
		fd_set readfds;
		Client * registerClientObj;
		int otherClientDescriptor;
		int fd_max=socketDescriptor;
		string inputCommand="";
		string tokenStrings[5];
		cout<<"Server Started...."<<endl;
		while(1){
			fd_max=setFDS(&readfds,socketDescriptor);
			select(fd_max+1,&readfds,NULL,NULL,NULL);

			if(FD_ISSET(socketDescriptor,&readfds)){
				exitIfError(clientDescriptor=accept(socketDescriptor,(struct sockaddr*)&client, &len),"Accept call");
				//cout<<"Incoming Connection!\n";
				Client * newClient=new Client();
				string tokenStrings[5];
				if(readControlMsg(clientDescriptor,tokenStrings)==0)
					return;
				newClient->setIPAddress(tokenStrings[1]);
				newClient->setPortNum(tokenStrings[2]);
				stringstream val;
				val<<++clientUniqueId;
				newClient->setClientID(val.str());
				//Establish connection 	on given port. add descriptor to list,
				newClient->setConnectionObj(new Connection(clientDescriptor));
				cout<<"New Connection Established with client "<<getHostNameFromIP(newClient->getIPAddress())<<" IP:"<<newClient->getIPAddress()<<" PORT:"<<newClient->getPortNum()<<"\n";
				sendRegisteredClientList(newClient);
				registerClient(newClient);


			}
			if(FD_ISSET(0,&readfds)){
				getline(cin,inputCommand);
				executeCommand(split(inputCommand));
			}
			//while each clients in list, read their terminate signal
			registerClientList->seekFirst();
			while(registerClientList->hasNext()){
				registerClientObj=registerClientList->next();
				if(registerClientObj->getConnectionObj()->isActive()){
					otherClientDescriptor=registerClientObj->getConnectionObj()->getSocketDescriptor();
					if(FD_ISSET(otherClientDescriptor,&readfds)){
						//Incoming message from peer
						//			cout<<"Incoming msg from client. Must be a terminate..socketDes:"<<otherClientDescriptor<<endl;
						readControlMsg(otherClientDescriptor,tokenStrings);


					}
				}
			}
		}
	}


	void executeCommand(string inputCommand[]){
		//cout<<inputCommand[1];
		transform(inputCommand[0].begin(),inputCommand[0].end(),inputCommand[0].begin(),::toupper);

		if(inputCommand[0].compare("HELP")==0){
			help();
		}else if(inputCommand[0].compare("MYIP")==0){
			cout<<getIPAddress()<<"\n";
		}else if(inputCommand[0].compare("MYPORT")==0){
			cout<<getPortNum()<<"\n";
		}else if(inputCommand[0].compare("LIST")==0){
			list();
		}else if(inputCommand[0].compare("EXIT")==0){
			gracefulExit();
		}else if(inputCommand[0].compare("CREATOR")==0){
			printCreator();
		}else{
			cout<<"Invalid command. Type HELP\n";
		}

	}

	void sendRegisteredClientList(Client* clientObj){

		int clientDescriptor=clientObj->getConnectionObj()->getSocketDescriptor();
		//Send Control Message
		sendControlMsg('s',clientDescriptor,clientObj);

		if(!registerClientList->hasNext()){
			//	cout<<"This is the first client!\n";
			return;
		}

		Client * registerClientObj;

		registerClientList->seekFirst();
		//	cout<<"****************************************************************************\n";
		//while each clients in list, send their clientID, IP n PORT
		while(registerClientList->hasNext()){
			string tcpMessage;
			registerClientObj=registerClientList->next();
			//	cout<<"Sending data abt Client:"<<registerClientObj->getClientID()<<endl;
			tcpMessage.append("a*");
			tcpMessage.append(registerClientObj->getClientID());
			tcpMessage.append("*");
			tcpMessage.append(registerClientObj->getIPAddress());
			tcpMessage.append("*");
			tcpMessage.append(registerClientObj->getPortNum());
			tcpMessage.append("*");
			while(tcpMessage.size()<controlByteSize){
				tcpMessage.append(" ");
			}
			//		cout<<"Control MSg:"<<tcpMessage<<" size:"<<tcpMessage.size()<<endl;
			fflush(stdout);
			sendMessage(clientDescriptor,tcpMessage,controlByteSize);
		}
		sendControlMsg('k',clientDescriptor,NULL);
		//	cout<<"Sending complete\n";
		//	cout<<"****************************************************************************\n";

	}
	void sendControlMsg(char controlChar,int clientDescriptor,Client * clientObj){
		string controlMsg;
		switch(controlChar){
		case 's':
			//	cout<<"Inside COntrol message for s"<<endl;
			controlMsg.append("s*");
			controlMsg.append(clientObj->getClientID());
			controlMsg.append("*");
			registerClientList->seekFirst();
			if(registerClientList->hasNext()){
				controlMsg.append("1");
			}else{
				controlMsg.append("0");
			}
			controlMsg.append("*");
			break;
		case 'a':
			//	cout<<"Inside Control message for a"<<endl;
			controlMsg.append("a*");
			controlMsg.append(clientObj->getClientID());
			controlMsg.append("*");
			controlMsg.append(clientObj->getIPAddress());
			controlMsg.append("*");
			controlMsg.append(clientObj->getPortNum());
			controlMsg.append("*");
			break;
		case 'r':
			//	cout<<"Inside Control message for r"<<endl;
			controlMsg.append("r*");
			controlMsg.append(clientObj->getClientID());
			controlMsg.append("*");
			controlMsg.append(clientObj->getIPAddress());
			controlMsg.append("*");
			controlMsg.append(clientObj->getPortNum());
			controlMsg.append("*");
			break;

		case 'k':
			//	cout<<"Inside COntrol message for k"<<endl;
			controlMsg.append("k*");
			break;
		default:
			cout<<"No Control Parameter specified\n"<<endl;
			return;

		}
		//Increase control message size to 30
		while(controlMsg.size()<controlByteSize){
			controlMsg.append(" ");
		}
		//	cout<<"Inside send message:"<<controlMsg<<endl;
		sendMessage(clientDescriptor,controlMsg,controlByteSize);


	}

	void display(){
		cout<<"Server Object Created..PortNum:"<<portNum<<"\n\n";
	}
	void registerClient(Client * clientObject){
		notifyClients(clientObject,'a');
		registerClientList->add(clientObject);

	}
	void removeClient(Client * clientObject){
		//Need to revisit the order of below function call
		registerClientList->remove(clientObject);
		notifyClients(clientObject,'r');

	}
	void notifyClients(Client * clientObject,const char controlChar){
		registerClientList->seekFirst();
		if(!registerClientList->hasNext()){
			//			cout<<"No Other Client Registered. Not Sending any Update!\n";
			return;
		}
		//	cout<<"This will send the updated list of registered clients to each of the clients\n";
		Client * currentClient;
		//	cout<<"****************************************************************************\n";

		while(registerClientList->hasNext()){
			currentClient=registerClientList->next();
			//	cout<<"Sending Update to Client:"<<currentClient->getClientID()<<controlChar<<": "<<clientObject->getClientID()<<endl;
			int clientDescriptor=currentClient->getConnectionObj()->getSocketDescriptor();
			sendControlMsg(controlChar,clientDescriptor,clientObject);
		}
		//	cout<<"****************************************************************************\n";



	}
	void help(){
		cout<<"Available Commands\t\n";
		cout<<"1. MYIP: Print my IP\t\t2. MYPORT: Print my Port\n";
		cout<<"3. LIST: Print active connections information\t4.CREATOR: List creator details\n";
		cout<<"5. EXIT: Exit\n";
	}
	void list(){
		registerClientList->seekFirst();
		if(!registerClientList->hasNext()){
			cout<<"There are no registered clients.\n";
			return;
		}
		Client * currentClient;
		Connection * connObj;
		cout<<"id\t"<<"Hostname\t\t\t"<<"Ip address\t"<<"Port No.\t"<<endl;

		while(registerClientList->hasNext()){
			currentClient=(Client *)registerClientList->next();
			connObj=currentClient->getConnectionObj();
			cout<<connObj->getConnectionID()<<"\t"<<getHostNameFromIP(currentClient->getIPAddress())<<"\t\t"<<currentClient->getIPAddress()<<"\t"<<currentClient->getPortNum()<<"\n";
		}
	}

	int terminateConnection(int socketDescriptor){
		//cout<<"Inside Server Terminate connection"<<endl;
		registerClientList->seekFirst();
		Client* clientObj;
		while(registerClientList->hasNext()){
			clientObj=registerClientList->next();
			Connection *conn=clientObj->getConnectionObj();
			if(conn->getSocketDescriptor()==socketDescriptor){
				//we have found the client
				cout<<"Client ID:"<<clientObj->getClientID()<<" IP:"<<clientObj->getIPAddress()<<" PortNum:"<<clientObj->getPortNum()<<" has terminated the connection"<<endl;
				close(socketDescriptor);
				clientObj->setConnectionObj(new Connection());
				removeClient(clientObj);
				return 0;
			}

		}
		return -1;
	}

	void gracefulExit(){
		/*Close all the active with the clients first.Server
		 **/
		cout<<"Exit requested."<<endl;
		Client* clientObj;
		Connection* conObj;
		registerClientList->seekFirst();
		while(registerClientList->hasNext()){
			clientObj=registerClientList->next();
			conObj=clientObj->getConnectionObj();
			if(conObj->isActive()){
				//Active connection. Close it.
				cout<<"Closing the connection with client IP:"<<clientObj->getIPAddress()<<" Port:"<<clientObj->getPortNum()<<endl;
				close(conObj->getSocketDescriptor());
				//delete old connection obj
			}
		}
		cout<<"Gracefully exiting!"<<endl;
		exit(0);

	}




};
void exitIfError(int descriptor,const char msg[]){
	if(descriptor==-1){
		cout<<"Error with "<<msg<<"..Exiting\n";
		exit(-1);
	}
}

char * getMyPublicIp(){
	//Referenced from Beej
	int socketDescriptor,conDescriptor;
	struct sockaddr_in publicDNSServer;
	publicDNSServer.sin_family = AF_INET;
	publicDNSServer.sin_port=htons(53);
	inet_pton(AF_INET,"8.8.8.8",&publicDNSServer.sin_addr);
	bzero(&publicDNSServer.sin_zero,8);
	exitIfError(socketDescriptor= socket(AF_INET, SOCK_DGRAM,0),"socket");
	exitIfError(conDescriptor=connect(socketDescriptor,(struct sockaddr*)&publicDNSServer, sizeof(struct sockaddr_in)),"connect");
	struct sockaddr_in myLocalAddress;
	socklen_t socketLength=sizeof(myLocalAddress);
	getsockname(socketDescriptor,(struct sockaddr*)&myLocalAddress,&socketLength);
	return inet_ntoa(myLocalAddress.sin_addr);
}

int main(int argc, char * argv[]){
	if((argc-1)!=2){
		cout<<"Invalid Number of arguments. Only Two arguments allowed\n";
	}else{
		int port=atoi(argv[2]);
		if(port==0 ||(port<0 ||port >65535)){
			cout<<"Invalid port number. Please try again"<<endl;
			return 0;
		}
		int socketDescriptor,bindDescriptor;
		struct sockaddr_in thisServer;
		thisServer.sin_family = AF_INET;
		thisServer.sin_port=htons(port);
		thisServer.sin_addr.s_addr=INADDR_ANY;
		bzero(&thisServer.sin_zero,8);
		exitIfError(socketDescriptor= socket(AF_INET, SOCK_STREAM,0),"socket");
		exitIfError(bindDescriptor=bind(socketDescriptor,(struct sockaddr*)&thisServer, sizeof(struct sockaddr_in)),"bind");
		exitIfError(listen(socketDescriptor,5),"listen");

		if(*argv[1]=='c'){
			Client*  clientInstance=new Client();
			//cout<<"Inside Main:"<<getMyPublicIp()<<endl;
			clientInstance->setIPAddress(getMyPublicIp());
			clientInstance->setPortNum(argv[2]);
			clientInstance->registerAndAcceptCommands(socketDescriptor);

			//	clientInstance->display();
		}else if(*argv[1]=='s'){
			Server*  serverInstance=new Server();
			serverInstance->setIPAddress(getMyPublicIp());
			serverInstance->setPortNum(argv[2]);
			serverInstance->registerClientAndAcceptCommands(socketDescriptor);

		}else{
			cout<<"Invalid Input"<<endl;
			cout<<"Arguments are : <program Name> s/c portnum";
		}
	}

	return 1;
}

