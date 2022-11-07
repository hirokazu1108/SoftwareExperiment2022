#ifndef _COMMON_H_
#define _COMMON_H_

/* ¥Ø¥Ã¥À¥Õ¥¡¥¤¥ë */
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <math.h>
#include <iostream>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/types.h>
#include<assert.h>


#define PORT			(u_short)8888	/* ï¿½Ý¡ï¿½ï¿½ï¿½ï¿½Ö¹ï¿½ */

#define MAX_CLIENTS		4				/* ï¿½ï¿½ï¿½é¥¤ï¿½ï¿½ï¿½ï¿½È¿ï¿½ï¿½Îºï¿½ï¿½ï¿½ï¿½ï¿? */
#define MAX_NAME_SIZE	10 				/* ï¿½æ¡¼ï¿½ï¿½ï¿½ï¿½Ì¾ï¿½Îºï¿½ï¿½ï¿½ï¿½ï¿½*/

#define MAX_DATA		200				/* ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Ç¡ï¿½ï¿½ï¿½ï¿½Îºï¿½ï¿½ï¿½ï¿½ï¿? */

#define END_COMMAND		'E'		  		/* ï¿½×¥ï¿½ï¿½ï¿½ï¿½ï¿½à½?Î»ï¿½ï¿½ï¿½Þ¥ï¿½ï¿? */


#endif 