#include "server.h"
#include<sys/socket.h>
#include<netinet/in.h>
#include<netdb.h>


CLIENT	gClients[MAX_CLIENTS]; // clients data

static fd_set	gMask;					/* select() mask */
static int	gWidth;						/* gMask sides */

static int MultiAccept(int request_soc,int num);
static void Enter(int pos, int fd);
static void SetMask(int maxfd);
static void SendAllName(void);
bool InputEvent(void);


// SetUpServer()
int SetUpServer(int num, u_short port)
{
    struct sockaddr_in	server;
    int			request_soc;
    int                 maxfd;
    int			val = 1;
 
    assert(0<num && num<=MAX_CLIENTS);

    bzero((char*)&server,sizeof(server));
    server.sin_family = AF_INET;
    server.sin_port = htons(port);
    server.sin_addr.s_addr = htonl(INADDR_ANY);

    // create socket
    if((request_soc = socket(AF_INET,SOCK_STREAM,0)) < 0){
		fprintf(stderr,"Socket allocation failed\n");
		return -1;
    }
    setsockopt(request_soc,SOL_SOCKET,SO_REUSEADDR,&val,sizeof(val));

    /* bind */
    if(bind(request_soc,(struct sockaddr*)&server,sizeof(server))==-1){
		fprintf(stderr,"Cannot bind\n");
		close(request_soc);
		return -1;
    }
    fprintf(stderr,"Successfully bind!\n");
    
    /* listen */
    if(listen(request_soc, gClientNum) == -1){
		fprintf(stderr,"Cannot listen\n");
		close(request_soc);
		return -1;
    }
    fprintf(stderr,"Listen OK\n");

    /* accept */
    maxfd = MultiAccept(request_soc, gClientNum);
    close(request_soc);
    if(maxfd == -1)return -1;

    SendAllName();

    SetMask(maxfd);

    return 0;
}

// SendRecvManager()
int SendRecvManager(void)
{
    char	command;
    fd_set	readOK;
    int		i;
    int		endFlag = 1;

    readOK = gMask;
    /* select */
    if(select(gWidth,&readOK,NULL,NULL,NULL) < 0){
      return endFlag;
    }
    for(i=0;i<gClientNum;i++){
      if(FD_ISSET(gClients[i].fd,&readOK)){
        RecvData(i,&command,sizeof(char));
        endFlag = ExecuteCommand(command,i);
        if(endFlag == 0)break;
      }
    }
    return endFlag;
}

// SendData()
void SendData(int pos,void *data,int dataSize)
{
    int	i;
  
    assert(0<=pos && pos<gClientNum || pos==ALL_CLIENTS);
    assert(data!=NULL);
    assert(0<dataSize);

    if(pos == ALL_CLIENTS){
      for(i=0;i<gClientNum;i++){
        write(gClients[i].fd,data,dataSize);
      }
    }
    else{
		  write(gClients[pos].fd,data,dataSize);
    }
}

// RecvData()
int RecvData(int pos,void *data,int dataSize)
{
    int n;
    
    assert(0<=pos && pos<gClientNum);
    assert(data!=NULL);
    assert(0<dataSize);

    n = read(gClients[pos].fd,data,dataSize);
    
    return n;
}


// Ending()
void Ending(void)
{
    int	i;

    printf("... Connection closed\n");
    for(i=0;i<gClientNum;i++)close(gClients[i].fd);
}

// MultiAccept()
static int MultiAccept(int request_soc,int num)
{
    int	i,j;
    int	fd;
    int c_num = 0;
    fd_set	readOK;
    int ret_select;
    struct timeval	timeout;

    /* select timeout settings */
    timeout.tv_sec = 0;
    timeout.tv_usec = 10;
    
    while(1){

      FD_ZERO(&readOK);
      FD_SET(request_soc,&readOK);
      /* select */
      ret_select = select(request_soc+1,&readOK,NULL,NULL,&timeout);
      if(ret_select == -1)
      {
        fprintf(stderr,"select error\n");
        return -1;
      }
      else if(ret_select == 0){
        //fprintf(stderr,"timeout\n");
      }
      else{
        if((fd = accept(request_soc,NULL,NULL)) == -1){
          fprintf(stderr,"Accept error\n");
          for(j=c_num-1;j>=0;j--)close(gClients[j].fd);
          return -1;
        }
        Enter(c_num,fd);
        c_num++;
        gUi.currentNum = c_num;
      }
      RenderWaitClientWindow();

      if(!InputEvent()){
        exit(0);
      }
      if(c_num == num){
        return fd;
      }
    }
    
}

// Enter
static void Enter(int pos, int fd)
{
	read(fd,gClients[pos].name,sizeof(char)*(NAME_MAX_LENGTH+1));
#ifndef NDEBUG
	printf("%s is accepted\n",gClients[pos].name);
#endif
	gClients[pos].fd = fd;
}

// SetMask()
static void SetMask(int maxfd)
{
    int	i;

    gWidth = maxfd+1;

    FD_ZERO(&gMask);    
    for(i=0;i<gClientNum;i++)FD_SET(gClients[i].fd,&gMask);
}

// SendAllName()
static void SendAllName(void)
{
  int	i,j,tmp1,tmp2;

    tmp2 = htonl(gClientNum);
    for(i=0;i<gClientNum;i++){
		tmp1 = htonl(i);
		SendData(i,&tmp1,sizeof(int));
		SendData(i,&tmp2,sizeof(int));
		for(j=0;j<gClientNum;j++){
			SendData(i,gClients[j].name,sizeof(char)*(NAME_MAX_LENGTH+1));

		}
	}
}

// InputEvent()
bool InputEvent(void)
{
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
        case SDL_QUIT:
            return false;
        case SDL_KEYDOWN:
            if (event.key.repeat)
                break;
            switch (event.key.keysym.sym) {
            case SDLK_ESCAPE:
                return false;
            default:
                break;
            }
            break;
        
        }
    }
    return true;
}
