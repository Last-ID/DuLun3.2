#ifndef __CAMERA_H
#define __CAMERA_H
#include "zf_common_headfile.h"

extern uint8 dbimage[MT9V03X_H][MT9V03X_W],ipsdbimage[MT9V03X_H][MT9V03X_W];
extern uint8 crossflag,midlinex[MT9V03X_H],edgeL[MT9V03X_H],edgeR[MT9V03X_H],leftlineflag[MT9V03X_H],rightlineflag[MT9V03X_H];
extern uint8 BlackEndMaxMax,DropRow;
extern int midpoint,endline;

void db_image(uint8 mode);
int GetMidline(uint8 dbimage[MT9V03X_H][MT9V03X_W]);
int edge_line(uint8 dbimage[MT9V03X_H][MT9V03X_W]);
void get_route_endline(void);
uint8 Judge_leftlossline(uint8 edgeL[MT9V03X_H]);
uint8 Judge_rightlossline(uint8 edgeR[MT9V03X_H]);

uint8 my_adapt_threshold(uint8 *image, uint16 col, uint16 row);
int GetOSTU(uint8  image[MT9V03X_H][MT9V03X_W]);
void seekLedge_frommid(int a);
void seekRedge_frommid(int a);
void connect_line(uint8  hang_start,uint8 hang_end,uint8 edima[MT9V03X_H]);
int startline(void);
void GetBlackEndParam(void);

int variance(uint8 edge[MT9V03X_H]);//Çó±ßÏß·½²î

void sancha(void);
#endif




