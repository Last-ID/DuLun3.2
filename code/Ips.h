#ifndef __IPS_H
#define __IPS_H
#include "zf_common_headfile.h"
typedef struct sMenu{

    struct sMenu* up;
    struct sMenu* down;
//    struct sMenu* left;
//    struct sMenu* right;

    char* name;
    uint32* value;
}Menu,* PMenu;
extern uint32 canshu;
void IPS_init(void);
void IPS_show(void);
void Key_operation(void);
void page_show(void);

void Menu_Item_Init(PMenu destination, PMenu Up, PMenu Down, char* Name,uint32 *p);
void Menu_Init(void);
#endif




