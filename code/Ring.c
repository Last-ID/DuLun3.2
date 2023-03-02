#include "zf_common_headfile.h"


int leftflag=0,rightflag=0,Rhd=0,Lhd=0,
RightLowPoint,Rightuppoint,LeftLowPoint,Leftuppoint,LeftRing,RightRing,RTurningPoint,LTurningPoint,
RightHDflag=0,LeftHDflag=0,leftloss,rightloss,leftlosspoint=0,rightlosspoint=0,incirclepoint,outcirclepoint,midoutpoint;
uint8 roadw2[MT9V03X_H]=
{
    0x10    ,0x11   ,0x13   ,0x15   ,0x17   ,0x18   ,0x1A   ,0x1C   ,   0x1E    ,0x20   ,
    0x22    ,0x24   ,0x26   ,0x28   ,0x2A   ,0x2C   ,0x2E   ,0x30   ,   0x31    ,0x33   ,
    0x36    ,0x38   ,0x3A   ,0x3B   ,0x3D   ,0x3E   ,0x3F   ,0x41   ,   0x43    ,0x45   ,
    0x48    ,0x4A   ,0x4C   ,0x4E   ,0x50   ,0x52   ,0x52   ,0x55   ,   0x57    ,0x59   ,
    0x5B    ,0x5C   ,0x5E   ,0x60   ,0x62   ,0x63   ,0x65   ,0x67   ,   0x69    ,0x6B   ,
    0x6D    ,0x6E   ,0x70   ,0x72   ,0x74   ,0x76   ,0x78,0x7A  ,   0x7B    ,0x7C
};
//-----------------------------------------
//  @brief      环岛识别
//  @param
//  @return      LeftHDflag
//  @note       右丢+右有环岛形状+还没进右环岛-》RightHDflag
//-----------------------------------------
void Ring_find(void)
{
    int i,j,k,m;
    leftloss=Judge_leftlossline(edgeL);
    rightloss=Judge_rightlossline(edgeR);
    //printf("lf:%d rf:%d\n",leftloss,rightloss);
    int Rhdjudge,Lhdjudge;

    int hang,lie;
    for(i=MT9V03X_H-1,Rhdjudge=0,hang=0;i>0;i--)//右环岛判别
    {
        if(Rhdjudge==0&&rightlineflag[i]!=0&&(rightlineflag[i-1]==0||edgeR[i-1]>135))//白
        {
            Rhdjudge=1;
            hang=i-1;//记下此行
            lie = (edgeR[i]+edgeR[i-1])/2;
        }
        if(Rhdjudge==1&&i<hang&&(rightlineflag[i]==0||edgeR[i]>140)&&rightlineflag[i-1]!=0)//黑
        {
            Rhdjudge=2;
            hang = i-1;
        }
        if(Rhdjudge==2&&i<hang&&((rightlineflag[i]!=0&&(rightlineflag[i]==0||edgeR[i]>140))||(dbimage[i][lie]==1)||edgeR[i-1]-edgeR[i]>0))//白
        {
            Rhdjudge=3;
        }
    }

    for( i=MT9V03X_H-1,Lhdjudge=0,hang=0;i>0;i--)//左环岛判别
    {
        if(Lhdjudge==0&&leftlineflag[i]!=0&&(leftlineflag[i-1]==0||edgeL[i-1]<5))//白
        {
            Lhdjudge=1;
            hang=i-1;//记下此行
            lie = (edgeL[i]+edgeL[i-1])/2;
        }
        if(Lhdjudge==1&&i<hang&&(leftlineflag[i]==0||edgeL[i]<5)&&leftlineflag[i-1]!=0)//黑
        {
            Lhdjudge=2;
            hang = i-1;
        }
        if(Lhdjudge==2&&i<hang&&((leftlineflag[i]!=0&&(leftlineflag[i]==0||edgeL[i]<5))||(dbimage[i][lie]==1)))//白
        {
            Lhdjudge=3;
        }
    }


    if(leftloss==0&&Rhdjudge==3&&Rhd==0)//左不丢线 右环岛 无标志位
    {
        rightflag=0;
        Rhdjudge=0;

        for (i = MT9V03X_H-1; i > 0; i--)
        {
            if(rightflag == 0&& i>50 &&((rightlineflag[i+1]!=0&&rightlineflag[i]!=0&&rightlineflag[i-1]==0)||(edgeR[i-1] -edgeR[i]>=3 && edgeR[i-1]>135)))
            {
                RightLowPoint = i;//找到右下拐点
                rightflag=1;
                //  printf("RightLowPoint[%d][%d]\n",RightLowPoint,edgeR[RightLowPoint]);
            }
            //找小圆
            //  if(rightflag == 1 && i < RightLowPoint && edgeR[i+1]>=175&&edgeR[i] >=175  && edgeR[i-1] <170)
            if(rightflag==1&&rightlineflag[i+2]==0&&rightlineflag[i+1]==0&&rightlineflag[i]!=0)
            {
                rightflag=2;
                RightRing = i-1;
            //printf("RightRing [%d][%d]\n",RightRing,edgeR[RightRing]);
            }
            if(rightflag == 2 && i < RightRing&& rightlineflag[i]!=0 && edgeR[i] > edgeR[i-3] && edgeR[i-3] < edgeR[i-6]&&edgeR[i]-edgeR[i-3]<5&&edgeR[i-3]-edgeR[i-6]<-5 )
            {
                rightflag=3;
                RTurningPoint = i-3;
            //printf("RTurningPoint:[%d][%d]\n",RTurningPoint,edgeR[RTurningPoint]);
            }
            if(rightflag == 3)
            {
            //printf("Find it !\n");
                for(j=RTurningPoint;j>0;j--)
                {
                    if(edgeR[j]-edgeR[j-1]>5)
                    {
                        Rightuppoint=j-1;
                        break;
                    }
                }
                RightHDflag = 1;
                rightflag = 0;
            }
        }
    }
    //左环岛
    //找左下拐点
    else if(Lhdjudge==3&&rightloss==0&&Lhd==0)
    {
        leftflag=0;
        Lhdjudge=0;
        for (i = MT9V03X_H-1; i > 25; i--)
        {
            if(leftflag == 0 && i>50 &&((leftlineflag[i+1]!=0&&leftlineflag[i]!=0&&leftlineflag[i-1]==0)||(edgeL[i] -edgeL[i-1]>=10&&edgeL[i-1]<5)))
            {
                LeftLowPoint = i;//找到左下拐点
                leftflag=1;
                //  printf("LeftLowPoint[%d][%d]\n",LeftLowPoint,edgeL[LeftLowPoint]);

            }
    //找小圆
            if(leftflag == 1 && leftlineflag[i+2]==0&&leftlineflag[i+1]==0&&leftlineflag[i]!=0)
            {
                leftflag=2;
                LeftRing = i-1;
                //      printf("LeftRing [%d][%d]\n",LeftRing,edgeL[LeftRing]);
            }
            if(leftflag == 2 && i < LeftRing && leftlineflag[i]!=0 && edgeL[i] <edgeL[i-3] && edgeL[i-3] > edgeL[i-6] && edgeL[i-3]-edgeL[i]<15&&edgeL[i-6]-edgeL[i-3]<-15 )
            {
                leftflag=3;
                LTurningPoint = i-5;
    //      printf("LTurningPoint:[%d][%d]\n",LTurningPoint,edgeL[LTurningPoint]);
            }
            if(leftflag == 3)
            {
            //  printf("Find it !\n");

                for(j=LTurningPoint;j>0;j--)
                {

                    if(edgeL[j-1]-edgeL[j]>3)
                    {
                        Leftuppoint=j-1;
                        //printf("Leftuppoint:[%d][%d]\n",Leftuppoint,edimagex[Leftuppoint][0]);
                        break;
                    }
                }
                LeftHDflag = 1;
                leftflag = 0;
            }
        }
    }
}

int rightring(void)//右环岛处理
{
    int i,j,k,m,b;
    if(Rhd==0&&rightlineflag[MT9V03X_H-1]!=0)   //最初补线
    {

        for(k=RTurningPoint,j=edgeR[RTurningPoint];k<RightLowPoint;k++)
        {
            if(ipsdbimage[k][j]!=0&&dbimage[k][j]!=0)
            {
                edgeR[k]=j;
                ipsdbimage[k][j]=0;
            }
            j = edgeR[RTurningPoint]+(int)(((edgeR[RightLowPoint] - edgeR[RTurningPoint])*1.0/(RightLowPoint - RTurningPoint)) * (k - RTurningPoint));
        }

    }
    else if(rightlineflag[MT9V03X_H-1]==0&&Rhd==0)Rhd=1;

    if(Rhd==1)//丢拐角后补固定线
    {
//                    gpio_set_level(LED1,1);
//                    gpio_set_level(LED2,1);
//                    gpio_set_level(LED3,1);
//                    gpio_set_level(LED4,1);
        RightLowPoint = 59;
        edgeR[RightLowPoint]=147;
        for (i = MT9V03X_H-1; i > 10; i--)
        {
            if(rightlineflag[i]!=0 && edgeR[i] > edgeR[i-5] && edgeR[i-5] < edgeR[i-10] )
            {
                RTurningPoint = i-5;
                break;
            }
        }
//          printf("RTurningPoint:[%d][%d]\n",RTurningPoint,edgeR[RTurningPoint]);
//          printf("RightLowPoint:[%d][%d]\n",RightLowPoint,edgeR[RightLowPoint]);

        connect_line(RTurningPoint,RightLowPoint,edgeR);

        if(rightlineflag[MT9V03X_H-1]!=0)
        {
            Rhd=2;
        }
    }

    if(Rhd==2)//预备进环岛
    {
//                  gpio_set_level(LED1,1);
//                  gpio_set_level(LED2,1);
//                    gpio_set_level(LED3,1);
//                    gpio_set_level(LED4,0);
//
            if(rightlineflag[MT9V03X_H-1]==0)
            {
                Rhd=3;

            }


    }

    if(Rhd==3)//进环岛
    {
//                  gpio_set_level(LED1, 0);
//                  gpio_set_level(LED2, 0);
//                    gpio_set_level(LED3,0);
//                    gpio_set_level(LED4,1);

            static int asf;
            int leftmidpoint;
            for( i=MT9V03X_H-1, k=0;i>20;i--)
            {
                if(rightlineflag[i]==0&&rightlineflag[i-3]!=0&&edgeR[i-3]<130)
                {
                    incirclepoint = i-3;
                    edgeL[incirclepoint]=edgeR[incirclepoint];

                    k=1;
                    break;
                }
            }
            if(leftlineflag[MT9V03X_H-1]!=0)
            {
                leftmidpoint = MT9V03X_H-6;
            }
            else
            {
                leftmidpoint = MT9V03X_H-2;
            }
            //edgeL[leftmidpoint] = 60;

            connect_line(incirclepoint,leftmidpoint,edgeL);

            for( i=MT9V03X_H-1;i>DropRow;i--)
            {
                midlinex[i]=edgeL[i]+roadw2[i]/2;
            }
            for( i=MT9V03X_H-1, k=0;i>20;i--)
            {
                if(leftlineflag[i]==0||edgeL[i]<=3)k++;

            }
            if(k>30)
            {
                asf=1;
            }




            if(asf==1)
            {
                for( i=MT9V03X_H-1,j=0;i>50;i--)
                {
                    if(leftlineflag[i]!=0)
                        j++;
                }

                if(j>5)
                {
                    Rhd=4;
                    asf=0;
                    endline = 45;
                }
            }


    }
    else if(Rhd==4)//准备出
    {

//                              gpio_set_level(LED1, 1);
//                  gpio_set_level(LED2, 1);
//                    gpio_set_level(LED3,1);
//                    gpio_set_level(LED4,1);
//      static int jieduan1;
//          if(leftlineflag[MT9V03X_H-1]!=0&&rightlineflag[MT9  V03X_H-1]!=0)
//          {
//              jieduan1=1;
//          }

//          if(jieduan1)
//          for(i=MT9V03X_H-1,j=0;i>60;i--)
//          {
//              if((leftlineflag[i]!=0&&leftlineflag[i-1]==0&&leftlineflag[i-2]==0&&leftlineflag[MT9V03X_H-1]!=0))
//              {
//                  Rhd=5;
//                  jieduan1=0;
//                  break;
//              }
//          }
//              for(i=MT9V03X_H-1,j=0,k=0;i>MT9V03X_H-BlackEndMaxMax;i--)
//              {
//                  if(j==0&&leftlineflag[i]!=0&&leftlineflag[i-1]==0&&leftlineflag[i-2]==0&&rightlineflag[MT9V03X_H-1]!=0)
//                  {
//                      j=1;
//                      k=i-2;
//                  }
//                  if(j==1&&i<k&&leftlineflag[i]==0&&leftlineflag[i-1]==0&&leftlineflag[i-2]!=0)
//                  {
//                      Rhd=5;
//                      break;
//                  }
//              }
        for(i=MT9V03X_H-1,b=0;i>40;i--)
        {
            if(edgeL[i]-edgeL[i-1]>1&&leftlineflag[i]!=0&&leftlineflag[i-1]!=0)
            {
                b++;
            }
            if(b>5||(leftloss==1&&rightloss==1))
            {
                Rhd=5;

                break;
            }
        }



    }

    else if(Rhd==5)//绕出来
    {
//                  gpio_set_level(LED1, 0);
//                  gpio_set_level(LED2, 0);
//                    gpio_set_level(LED3,0);
//                    gpio_set_level(LED4,0);
        static int outgogo=0;
            for(i=MT9V03X_H-1,j=0;i>10;i--)
            {
                if(dbimage[i][80]==1&&dbimage[i+1][80]==1&&dbimage[i-1][80]==0)
                {
                    midoutpoint = i+1;
                    edgeL[i+1]=96;
                    j=1;
                    break;
                }
            }
            if(j==0)
            {
                midoutpoint = 60-BlackEndMaxMax;
                edgeL[midoutpoint]=96;
            }
            for(i=MT9V03X_H-1, j=0;i>midoutpoint;i--)
            {
                if(leftlineflag[i]!=0&&(leftlineflag[i-1]==0||edgeL[i-1]<4)&&(leftlineflag[i-2]==0||edgeL[i-2]<4))
                {
                    outcirclepoint = i;
                    j=1;
                    break;
                }
                else if(leftlineflag[MT9V03X_H-1]==0)
                    j=0;
            }

            if(j==0&&leftlineflag[MT9V03X_H-1]==0)
            {
                outcirclepoint = 58;
                edgeL[outcirclepoint] = 8;
                if(outgogo==0)outgogo=1;

            }

//              printf("outcirclepoint[%d][%d]\n",outcirclepoint,edgeL[outcirclepoint]);
//              printf("midoutpoint[%d][%d]\n",midoutpoint,edgeL[midoutpoint]);
            connect_line(midoutpoint,outcirclepoint,edgeL);

            if(leftlineflag[MT9V03X_H-1]==0&&outgogo==1)outgogo=2;//左下角丢线
//              for(i=MT9V03X_H-10,midpoint=0;i<MT9V03X_H;i++)
//              {
//                  ipsdbimage[i][midlinex[i]]=0;//画中线
//                  //ips114_draw_line(edgeR[RTurningPoint],RTurningPoint,edgeR[RightLowPoint],RightLowPoint,RGB565_RED);
//                  midpoint += midlinex[i];
//              }
//              midpoint /= 10;

            if(outgogo==2&&leftlineflag[MT9V03X_H-1]!=0)
                outgogo=3;

        //printf("outgogo:%d",outgogo);
        for(i=MT9V03X_H-1,j=0,k=0;i>30;i--)
        {
            if(outgogo==3)
            {
//                  if(j==0&&rightlineflag[i]!=0&&rightlineflag[i-1]==0)
                if(j==0&&rightlineflag[i]==0&&rightlineflag[i-1]==0)
                {
                    j=1;
                    k=i-1;
                }
                if(j==1&&i<k&&rightlineflag[i]==0&&rightlineflag[i-1]!=0)
                {
                    Rhd=6;
                    outgogo=0;
                }

            }

        }

//          if(leftloss==0||(leftlineflag[MT9V03X_H-5]!=0&&rightlineflag[MT9V03X_H-3]==0))
//          {
//              Rhd=6;
//          }
    }

    else if(Rhd==6)//再不进环岛
    {
//                  gpio_set_level(LED1, 1);
//                  gpio_set_level(LED2, 1);
//                    gpio_set_level(LED3,1);
//                    gpio_set_level(LED4,1);

        for(i=MT9V03X_H-1,j=0;i>40;i--)
            if(rightlineflag[i]!=0&&((edgeR[i-1]-edgeR[i]>0)||rightlineflag[i-1]==0))
            {
                RightLowPoint=i;
                edgeR[RightLowPoint]=edgeR[i];
                j=1;
                break;
            }
        if(j==0)
        {
            RightLowPoint = 59;
            edgeR[RightLowPoint]=145;
        }


        for (i = MT9V03X_H-10,k=0; i > 5; i--)
        {
            if((rightlineflag[i]==0||edgeR[i]>145) &&(rightlineflag[i-1]==0||edgeR[i]>145) && (rightlineflag[i-1]==0||edgeR[i]>145) && (rightlineflag[i-5]!=0&&edgeR[i-5]<140))
            {
                RTurningPoint = i-5;
                k=1;
                break;
            }
        }
        if(k)
        connect_line(RTurningPoint,RightLowPoint,edgeR);
        midpoint = edgeL[endline]+roadw2[endline]/2;
        for(i=MT9V03X_H-1,b=0;i>25;i--)
            if(rightlineflag[i]!=0&&leftlineflag[i]!=0)//结束
            {
                b++;
            }
        if(b>20)
        {
            Rhd=0;
            RightHDflag=0;
        }
    }
    for(m=MT9V03X_H-1;m>0;m--)
        midlinex[m]=(edgeL[m]+edgeR[m])/2;

    printf("RHD=%d",Rhd);
    return 1;
}
int leftring(void)//左环岛处理
{
    int i,j,k,m,b;
    if(Lhd==0&&leftlineflag[MT9V03X_H-1]!=0)    //最初补线
    {
        for(k=LTurningPoint,j=edgeL[LTurningPoint];k<LeftLowPoint;k++)
        {

            if(ipsdbimage[k][j]!=0&&dbimage[k][j]!=0)
            {
                edgeL[k]=j;
                ipsdbimage[k][j]=0;
            }
            j = edgeL[LTurningPoint]+(int)(((edgeL[LeftLowPoint] - edgeL[LTurningPoint])*1.0/(LeftLowPoint - LTurningPoint)) * (k - LTurningPoint));
        }
    }
    else if(leftlineflag[MT9V03X_H-1]==0&&Lhd==0)Lhd=1;

    if(Lhd==1)
    {
        LeftLowPoint = 59;
        edgeL[LeftLowPoint]=5;
        for (i = MT9V03X_H-1; i > 30; i--)
        {
            if(leftlineflag[i]!=0 && edgeL[i] < edgeL[i-3] && edgeL[i-3] > edgeL[i-5] )
            {
                LTurningPoint = i-3;
            }
        }
        connect_line(LTurningPoint,LeftLowPoint,edgeL);

        if(leftlineflag[MT9V03X_H-1]!=0)
        {

            Lhd=2;
        }
    }
  if(Lhd==2)//预备进环岛
    {
            if(leftlineflag[MT9V03X_H-1]==0)
            {
                Lhd=3;

            }

    }
    if(Lhd==3)//进环岛
    {
        for( i=MT9V03X_H, k=0,j=0;i>10;i--)
            {
                if(rightlineflag[i]==0||edgeR[i]<=5)k++;

            }
            if(k>25)
            {
                j=1;
            }
            if(j==1)
            {
                for( i=MT9V03X_H, k=0;i>30;i--)
                {
                    if(leftlineflag[i]!=0)k++;
                    if(k>15)
                    {
                        Lhd=4;

                    }

                }
            }
        }
    if(Lhd==4)//准备出
    {
        for(i=20,k=0;i<50;i++)
        {
                if(leftlineflag[i]==0&&rightlineflag[i]==0)k++;
        }

        if(leftloss==1&&rightloss==1&&k>20)
        {
            Lhd=5;
        }
    }
    if(Lhd==5)//出来后
    {
        if(rightloss==0)
        {
            Lhd=6;
        }

    }
    if(Lhd==6)
    {

        Lhd=0;
        LeftHDflag = 0;
    }

    for(m=MT9V03X_H-1;m>0;m--)
        midlinex[m]=(edgeL[m]+edgeR[m])/2;
    return 1;
}
