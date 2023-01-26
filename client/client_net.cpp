#include "client.h"
#include<sys/socket.h>
#include<netdb.h>

#define	BUF_SIZE	100

static int	gSocket;	
static fd_set	gMask;	
static int	gWidth;	

static void GetAllName(int *clientID,int *num,char clientNames[][NAME_MAX_LENGTH+1]);
static void SetMask(void);


// SetUpClient()
int SetUpClient(char *hostName,u_short port,int *clientID,int *num,char clientNames[][NAME_MAX_LENGTH+1])
{
    struct hostent	*servHost;
    struct sockaddr_in	server;
    int			len;
    SaveData data;

    if((servHost = gethostbyname(hostName))==NULL){
		fprintf(stderr,"Unknown host\n");
		return -1;int SetUpServer(int num);int SetUpServer(int num);
    }

    bzero((char*)&server,sizeof(server));
    server.sin_family = AF_INET;
    server.sin_port = htons(port);
    bcopy(servHost->h_addr,(char*)&server.sin_addr,servHost->h_length);

    if((gSocket = socket(AF_INET,SOCK_STREAM,0)) < 0){
		fprintf(stderr,"socket allocation failed\n");
		return -1;
    }

    if(connect(gSocket,(struct sockaddr*)&server,sizeof(server)) == -1){
		fprintf(stderr,"cannot connect\n");
		close(gSocket);
		return -1;
    }
    fprintf(stderr,"connected\n");

    ReadDataFile(&data);
    SendData(data.clientName,sizeof(char)*(NAME_MAX_LENGTH+1));

    printf("Please Wait\n");

    GetAllName(clientID,num,clientNames);

    SetMask();
	
    WriteMatchFile(1);

    return 0;
}

// SendRecvManager()
int SendRecvManager(void)
{
    fd_set	readOK;
    char	command;
    int		i;
    int		endFlag = 1;
    struct timeval	timeout;

    /* select() timeout settings */
    timeout.tv_sec = 0;
    timeout.tv_usec = 20;

    readOK = gMask;

    select(gWidth,&readOK,NULL,NULL,&timeout);
    if(FD_ISSET(gSocket,&readOK)){

		RecvData(&command,sizeof(char));

		endFlag = ExecuteCommand(command);
    }
    return endFlag;
}

// RecvIntData()
int RecvIntData(int *intData)
{
    int n,tmp;
    
    assert(intData!=NULL);

    n = RecvData(&tmp,sizeof(int));
    (*intData) = ntohl(tmp);
    
    return n;
}

// SendData()
void SendData(void *data,int dataSize)
{
    assert(data != NULL);
    assert(0 < dataSize);

    write(gSocket,data,dataSize);
}

// CloseSoc()
void CloseSoc(void)
{
    printf("...Connection closed\n");
    close(gSocket);
}

// GetAllName()
static void GetAllName(int *clientID,int *num,char clientNames[][NAME_MAX_LENGTH+1])
{
    int	i;

    RecvIntData(clientID);
    RecvIntData(num);

    for(i=0;i<(*num);i++){
		  RecvData(clientNames[i],sizeof(char)*(NAME_MAX_LENGTH+1));
    }
#ifndef NDEBUG
    printf("#####\n");
    printf("client number = %d\n",(*num));
    for(i=0;i<(*num);i++){
		printf("%d:%s\n",i,clientNames[i]);
    }
#endif
}

// SetMask()
static void SetMask(void)
{
    int	i;

    FD_ZERO(&gMask);
    FD_SET(gSocket,&gMask);

    gWidth = gSocket+1;
}

// RecvData
int RecvData(void *data,int dataSize)
{
    assert(data != NULL);
    assert(0 < dataSize);

    return read(gSocket,data,dataSize);
}
