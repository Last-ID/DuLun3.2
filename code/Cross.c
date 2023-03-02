#include "zf_common_headfile.h"
uint8 up_leftpoint=0,up_rightpoint=0,down_leftpoint=0,down_rightpoint=0,crossflag=0,up_leftflag=0,down_leftflag,up_rightflag,down_rightflag;//十字变量



void cross_find(void)
{
    int i,j,k,m;
    leftloss=Judge_leftlossline(edgeL);
    rightloss=Judge_rightlossline(edgeR);
    if(leftloss==1&&rightloss==1&&LeftHDflag==0&&RightHDflag==0)
    {
        //十字检测
        //  printf("crossstart-test\n");
        for(i=MT9V03X_H-5,j=0;i>10;i--)
        {
            if(edgeL[i]<10&&edgeR[i]>140)
            {
                j++;
                //printf("j=%d",j);
            }

            if(j>30)
            {
                for(m=10,k=0;m<30;m++)//中间20*20 白点数量检测
                {
                    for(j=54;j<74;j++)
                    {
                        if(dbimage[m][j]==1)
                            k++;

                    }

                }
                if(k>350)
                {
                    crossflag=1;//两边丢线且中间多白点 为十字
                    break;
                }
            }
        }
    }
}

void crossroad(void)//找四个拐点，计算斜率并补线
{

        for(int i=MT9V03X_H-5;i>10;i--)
        {
            if(down_leftflag==0&&(edgeL[i]-edgeL[i-3]>15||(edgeL[i]>10&&edgeL[i-3]<5)))
            {
                down_leftpoint = i;
                down_leftflag = 1;
            }
            if(up_leftflag==0&&(edgeL[i]-edgeL[i-3]<-15||(edgeL[i]<5&&edgeL[i-3]>10)))
            {
                up_leftpoint = i-10;
                up_leftflag=1;
            }
            if(down_rightflag==0&&(edgeR[i-3]-edgeR[i]>15||(edgeR[i]<160&&edgeR[i-3]>115)))
            {
                down_rightpoint = i;
                down_rightflag = 1;
            }
            if(up_rightflag==0&&(edgeR[i-3]-edgeR[i]<-15||(edgeR[i-3]<110&&edgeR[i]>115)))
            {
                up_rightpoint = i-3;
                up_rightflag = 1;
            }
            if(up_leftflag&&up_rightflag&&down_leftflag&&down_rightflag)
            {
                printf("GET4POINT\nur:%d dr:%d\nul:%d dl:%d\n",up_rightpoint,down_rightpoint,up_leftpoint,down_leftpoint);
                break;
            }
        }
        if(up_leftflag&&up_rightflag&&down_leftflag&&down_rightflag)
        {
            connect_line(up_leftpoint,down_leftpoint,edgeL);
//          for(k=up_leftpoint,j=edgeL[up_leftpoint];k<down_leftpoint;k++)
//          {
//
//              if(ipsdbimage[k][j]!=0&&dbimage[k][j]!=0)
//              {
//              //  dbimage[k][j]=0;
//                  edgeL[k]=j;
//                  ipsdbimage[k][j]=0;
//                  j = edgeL[up_leftpoint]+(int)(((edgeL[down_leftpoint] - edgeL[up_leftpoint])*1.0/(down_leftpoint - up_leftpoint)) * (k - up_leftpoint));
//                  printf("jl=%d",j);
//              }
//          }
            connect_line(up_rightpoint,down_rightpoint,edgeR);
//          for(k=up_rightpoint,j=edgeR[up_rightpoint];k<down_rightpoint;k++)
//          {
//
//              if(ipsdbimage[k][j]!=0&&dbimage[k][j]!=0)
//              {
//
//                  edgeR[k]=j;
//                  ipsdbimage[k][j]=0;
//                  j = edgeR[up_rightpoint]+(int)(((edgeR[down_rightpoint] - edgeR[up_rightpoint])*1.0/(down_rightpoint - up_rightpoint)) * (k - up_rightpoint));
//                  printf("jr8=%d",j);
//              }
//          }

        }
        up_leftflag=0;
        up_rightflag=0;
        down_leftflag=0;
        down_rightflag=0;
        crossflag = 0;
}
