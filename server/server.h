#ifndef _CLIENT_FUNC_H_
#define _CLIENT_FUNC_H_

#include"../constants.h"

#define ALL_CLIENTS	-1   /* ??????????????????????????????? */

/* server_net.c */
extern int SetUpServer(int num, u_short port);
extern void Ending(void);
extern int RecvIntData(int pos,int *intData);
extern void SendData(int pos,void *data,int dataSize);
extern int RecvData(int pos,void *data,int dataSize);
extern int SendRecvManager(void);

/* server_command.c */
extern int ExecuteCommand(char command,int pos);

extern int	gClientNum; //クライアント数
extern Player *player;
#endif