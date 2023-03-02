#include "zf_common_headfile.h"

#define BYTE0(dwTemp)  (*(char*)(&dwTemp))
#define BYTE1(dwTemp)  (*((char*)(&dwTemp)+1))
#define BYTE2(dwTemp)  (*((char*)(&dwTemp)+2))
#define BYTE3(dwTemp)  (*((char*)(&dwTemp)+3))

#define Data_lenth 5
uint8 Data[5];

fifo_struct uart_data_fifo;
uint8 uart_get_data[Data_lenth];                                                        // 串口接收数据缓冲区
unsigned char DatatoSend[100];

uint8 result;
void Send1(int16 a,int16 b, int16 c,int16 d)
{
    unsigned char _cnt=0,sc=0,ac=0,i;

    DatatoSend[_cnt++]=0xAA;
    DatatoSend[_cnt++]=0xFF;
    DatatoSend[_cnt++]=0xF1;
    DatatoSend[_cnt++]=8;
    DatatoSend[_cnt++]=BYTE0(a);
//  result = BYTE0(a);
//  printf("BYTE0(a):%d\n",result);
    DatatoSend[_cnt++]=BYTE1(a);
//  printf("BYTE1(a):%d\n",(BYTE1(a)<<8)+(uint8)(BYTE0(a)));
    DatatoSend[_cnt++]=BYTE0(b);
    DatatoSend[_cnt++]=BYTE1(b);

    DatatoSend[_cnt++]=BYTE0(c);
    DatatoSend[_cnt++]=BYTE1(c);

    DatatoSend[_cnt++]=BYTE0(d);
    DatatoSend[_cnt++]=BYTE1(d);

    for(i=0;i<DatatoSend[3]+4;i++)
    {
        sc+=DatatoSend[i];
        ac+=sc;
    }
    DatatoSend[_cnt++]=sc;
    DatatoSend[_cnt++]=ac;

    uart_write_buffer(WIRELESS_UART_INDEX,DatatoSend,_cnt);
}
