#include "zf_common_headfile.h"


float Fuzzy(float P, float D)//学长代码
{
    /*输入量P语言值特征点*/
    float PFF[7] = {-24, -12, -6, 0, 6, 12, 24};
    /*输入量D语言值特征点*/
    float DFF[5] = {-6.5, -3, 0, 3, 6.5};
    /*输出量U语言值特征点(根据赛道类型选择不同的输出值)*/
    float UFF[7] = {0, 15, 20, 35, 50, 80, 110};

    int rule[7][5] = {
        //  -2  -1   0   1   2 ec    e
        {1, 1, 1, 0, 0}, //  -3
        {4, 2, 1, 0, 0}, //  -2
        {5, 4, 0, 1, 0}, //  -1
        {3, 1, 0, 1, 3}, //   0
        {0, 1, 0, 4, 5}, //   1
        {0, 0, 1, 2, 4}, //   2
        {0, 0, 1, 1, 1}  //   3
    };

    float U = 0; /*偏差,偏差微分以及输出值的精确量*/
    float PF[2] = {0}, DF[2] = {0}, UF[4] = {0};
    /*偏差,偏差微分以及输出值的隶属度*/
    int Pn = 0, Dn = 0, Un[4] = {0};
    float t1 = 0, t2 = 0, t3 = 0, t4 = 0, temp1 = 0, temp2 = 0;
    /*隶属度的确定*/
    /*根据PD的指定语言值获得有效隶属度*/
    if (P > PFF[0] && P < PFF[6])
    {
        if (P <= PFF[1])
        {
            Pn = -2;
            PF[0] = (PFF[1] - P) / (PFF[1] - PFF[0]);
        }
        else if (P <= PFF[2])
        {
            Pn = -1;
            PF[0] = (PFF[2] - P) / (PFF[2] - PFF[1]);
        }
        else if (P <= PFF[3])
        {
            Pn = 0;
            PF[0] = (PFF[3] - P) / (PFF[3] - PFF[2]);
        }
        else if (P <= PFF[4])
        {
            Pn = 1;
            PF[0] = (PFF[4] - P) / (PFF[4] - PFF[3]);
        }
        else if (P <= PFF[5])
        {
            Pn = 2;
            PF[0] = (PFF[5] - P) / (PFF[5] - PFF[4]);
        }
        else if (P <= PFF[6])
        {
            Pn = 3;
            PF[0] = (PFF[6] - P) / (PFF[6] - PFF[5]);
        }
    }

    else if (P <= PFF[0])
    {
        Pn = -2;
        PF[0] = 1;
    }
    else if (P >= PFF[6])
    {
        Pn = 3;
        PF[0] = 0;
    }

    PF[1] = 1 - PF[0];

    //判断D的隶属度
    if (D > DFF[0] && D < DFF[4])
    {
        if (D <= DFF[1])
        {
            Dn = -2;
            DF[0] = (DFF[1] - D) / (DFF[1] - DFF[0]);
        }
        else if (D <= DFF[2])
        {
            Dn = -1;
            DF[0] = (DFF[2] - D) / (DFF[2] - DFF[1]);
        }
        else if (D <= DFF[3])
        {
            Dn = 0;
            DF[0] = (DFF[3] - D) / (DFF[3] - DFF[2]);
        }
        else if (D <= DFF[4])
        {
            Dn = 1;
            DF[0] = (DFF[4] - D) / (DFF[4] - DFF[3]);
        }
    }
    //不在给定的区间内
    else if (D <= DFF[0])
    {
        Dn = -2;
        DF[0] = 1;
    }
    else if (D >= DFF[4])
    {
        Dn = 1;
        DF[0] = 0;
    }

    DF[1] = 1 - DF[0];
    Un[0] = rule[Pn - 1 + 3][Dn - 1 + 3];
    Un[1] = rule[Pn + 3][Dn - 1 + 3];
    Un[2] = rule[Pn - 1 + 3][Dn + 3];
    Un[3] = rule[Pn + 3][Dn + 3];

    if (PF[0] <= DF[0]) //求小
        UF[0] = PF[0];
    else
        UF[0] = DF[0];
    if (PF[1] <= DF[0])
        UF[1] = PF[1];
    else
        UF[1] = DF[0];
    if (PF[0] <= DF[1])
        UF[2] = PF[0];
    else
        UF[2] = DF[1];
    if (PF[1] <= DF[1])
        UF[3] = PF[1];
    else
        UF[3] = DF[1];
    if (Un[0] == Un[1])
    {
        if (UF[0] > UF[1])
            UF[1] = 0;
        else
            UF[0] = 0;
    }
    if (Un[0] == Un[2])
    {
        if (UF[0] > UF[2])
            UF[2] = 0;
        else
            UF[0] = 0;
    }
    if (Un[0] == Un[3])
    {
        if (UF[0] > UF[3])
            UF[3] = 0;
        else
            UF[0] = 0;
    }
    if (Un[1] == Un[2])
    {
        if (UF[1] > UF[2])
            UF[2] = 0;
        else
            UF[1] = 0;
    }
    if (Un[1] == Un[3])
    {
        if (UF[1] > UF[3])
            UF[3] = 0;
        else
            UF[1] = 0;
    }
    if (Un[2] == Un[3])
    {
        if (UF[2] > UF[3])
            UF[3] = 0;
        else
            UF[2] = 0;
    }
    t1 = UF[0] * UFF[Un[0]];
    t2 = UF[1] * UFF[Un[1]];
    t3 = UF[2] * UFF[Un[2]];
    t4 = UF[3] * UFF[Un[3]];
    temp1 = t1 + t2 + t3 + t4;
    temp2 = UF[0] + UF[1] + UF[2] + UF[3]; //模糊量输出
    U = temp1 / temp2;
    return U;
}
//使用的模糊PID程序，有几个特殊步骤，输入的参数E为err，EC为err的微分，即这次的err减去上次的err
//https://blog.csdn.net/piginthesouth/article/details/127659090?
int kp_m=200,kd_m=200;//kp最大值
float KP_Fuzzy(float E,float EC)
{

    int rule_p[7][7]=
    {
        { 6 , 5 , 4 , 4 , 3 , 0 , 0},//-36
        { 6 , 4 , 3 , 3 , 2 , 0 , 0},//-24
        { 4 , 3 , 2 , 1 , 0 , 1 , 2},//-12
        { 2 , 1 , 1 , 0 , 1 , 1 , 2},//0
        { 2 , 1 , 0 , 1 , 2 , 3 , 4},//12
        { 0 , 0 , 2 , 3 , 3 , 4 , 6},//24
        { 0 , 1 , 3 , 4 , 4 , 5 , 6},//36
    };//模糊规则表 P



    uint8 i2;
    /*输入量P语言值特征点*/
    float EFF[7]={-30,-20,-10,0,10,20,30};//EFF为E(即err的范围)
    /*输入量D语言值特征点*/
    float DFF[7]={-15,-10,-5,0,5,10,15};//DFF为EC( | err-last_err|  的范围)
    /*输出量U语言值特征点(根据赛道类型选择不同的输出值)*/
    float UFF[7];//UFF，即（err-last_err）的范围，

    for(i2=0;i2<7;i2++)
        UFF[i2]=kp_m/6*i2;


    float U=0;  /*偏差,偏差微分以及输出值的精确量*/
    float PF[2]={0},DF[2]={0},UF[4]={0};
    /*偏差,偏差微分以及输出值的隶属度*/
    int Pn=0,Dn=0,Un[4]={0};
    float t1=0,t2=0,t3=0,t4=0,temp1=0,temp2=0;
    /*隶属度的确定*/
    /*根据PD的指定语言值获得有效隶属度*/
    if(E>EFF[0] && E<EFF[6])
    {
        if(E<=EFF[1])
        {
            Pn=-2;
            PF[0]=(EFF[1]-E)/(EFF[1]-EFF[0]);
        }
        else if(E<=EFF[2])
        {
            Pn=-1;
            PF[0]=(EFF[2]-E)/(EFF[2]-EFF[1]);
        }
        else if(E<=EFF[3])
        {
            Pn=0;
            PF[0]=(EFF[3]-E)/(EFF[3]-EFF[2]);
        }
        else if(E<=EFF[4])
        {
            Pn=1;
            PF[0]=(EFF[4]-E)/(EFF[4]-EFF[3]);
        }
        else if(E<=EFF[5])
        {
            Pn=2;
            PF[0]=(EFF[5]-E)/(EFF[5]-EFF[4]);
        }
        else if(E<=EFF[6])
        {
            Pn=3;
            PF[0]=(EFF[6]-E)/(EFF[6]-EFF[5]);
        }
    }

    else if(E<=EFF[0])
    {
        Pn=-2;/*  ??? */
        PF[0]=1;
    }
    else if(E>=EFF[6])
    {
        Pn=3;
        PF[0]=0;
    }

    PF[1]=1-PF[0];


    //判断D的隶属度
    if(EC>DFF[0]&&EC<DFF[6])
    {
        if(EC<=DFF[1])
        {
            Dn=-2;
            DF[0]=(DFF[1]-EC)/(DFF[1]-DFF[0]);
        }
        else if(EC<=DFF[2])
        {
            Dn=-1;
            DF[0]=(DFF[2]-EC)/(DFF[2]-DFF[1]);
        }
        else if(EC<=DFF[3])
        {
            Dn=0;
            DF[0]=(DFF[3]-EC)/(DFF[3]-DFF[2]);
        }
        else if(EC<=DFF[4])
        {
            Dn=1;
            DF[0]=(DFF[4]-EC)/(DFF[4]-DFF[3]);
        }
        else if(EC<=DFF[5])
        {
            Dn=2;
            DF[0]=(DFF[5]-EC)/(DFF[5]-DFF[4]);
        }
        else if(EC<=DFF[6])
        {
            Dn=3;
            DF[0]=(DFF[6]-EC)/(DFF[6]-DFF[5]);
        }
    }
    //不在给定的区间内
    else if (EC<=DFF[0])
    {
        Dn=-2;
        DF[0]=1;
    }
    else if(EC>=DFF[6])
    {
        Dn=3;
        DF[0]=0;
    }

    DF[1]=1-DF[0];

    /*使用误差范围优化后的规则表rule[7][7]*/
    /*输出值使用13个隶属函数,中心值由UFF[7]指定*/
    /*一般都是四个规则有效*/
    Un[0]=rule_p[Pn+2][Dn+2];
    Un[1]=rule_p[Pn+3][Dn+2];
    Un[2]=rule_p[Pn+2][Dn+3];
    Un[3]=rule_p[Pn+3][Dn+3];

    if(PF[0]<=DF[0])    //求小
        UF[0]=PF[0];
    else
        UF[0]=DF[0];
    if(PF[1]<=DF[0])
        UF[1]=PF[1];
    else
        UF[1]=DF[0];
    if(PF[0]<=DF[1])
        UF[2]=PF[0];
    else
        UF[2]=DF[1];
    if(PF[1]<=DF[1])
        UF[3]=PF[1];
    else
        UF[3]=DF[1];
    /*同隶属函数输出语言值求大*/
    if(Un[0]==Un[1])
    {
        if(UF[0]>UF[1])
            UF[1]=0;
        else
            UF[0]=0;
    }
    if(Un[0]==Un[2])
    {
        if(UF[0]>UF[2])
            UF[2]=0;
        else
            UF[0]=0;
    }
    if(Un[0]==Un[3])
    {
        if(UF[0]>UF[3])
            UF[3]=0;
        else
            UF[0]=0;
    }
    if(Un[1]==Un[2])
    {
        if(UF[1]>UF[2])
            UF[2]=0;
        else
            UF[1]=0;
    }
    if(Un[1]==Un[3])
    {
        if(UF[1]>UF[3])
            UF[3]=0;
        else
            UF[1]=0;
    }
    if(Un[2]==Un[3])
    {
        if(UF[2]>UF[3])
            UF[3]=0;
        else
            UF[2]=0;
    }
    t1=UF[0]*UFF[Un[0]];
    t2=UF[1]*UFF[Un[1]];
    t3=UF[2]*UFF[Un[2]];
    t4=UF[3]*UFF[Un[3]];
    temp1=t1+t2+t3+t4;
    temp2=UF[0]+UF[1]+UF[2]+UF[3];//模糊量输出
    if(temp2!=0)
        U=temp1/temp2;
    else {
        U=0;
    }
//    temp1=PF[0]*UFF[Un[0]]+PF[1]*UFF[Un[1]]+PF[0]*UFF[Un[2]]+PF[1]*UFF[Un[3]]+DF[0]*UFF[Un[0]]+DF[0]*UFF[Un[1]]+DF[1]*UFF[Un[2]]+DF[0]*UFF[Un[3]];
//    U=temp1;
    return U;

}


float Kd_Fuzzy(float EC)
{

    int rule_d[7] = { 6 , 5 , 3 , 2 , 3 , 5 , 6};//模糊规则表 D

    float out=0;
    uint8 i=0;
    float degree_left = 0,degree_right = 0;
    uint8 degree_left_index = 0,degree_right_index = 0;
    float DFF[7]={-15,-10,-5,0,5,10,15};
    float UFF[7];

    for(i=0;i<7;i++)
            UFF[i]=kd_m/6*i;

    if(EC<DFF[0])
    {
        degree_left = 1;
        degree_right = 0;
        degree_left_index = 0;
    }
    else if (EC>DFF[6]) {
        degree_left = 1;
        degree_right = 0;
        degree_left_index = 6;
    }
    else {
        for(i=0;i<6;i++)
        {
            if(EC>=DFF[i]&&EC<DFF[i+1])
            {
                degree_left = (float)(DFF[i+1] - EC)/(DFF[i+1] - DFF[i]);
                degree_right = 1 - degree_left;
                degree_left_index = i;
                degree_right_index = i+1;
                break;
            }
        }
    }

    out = UFF[rule_d[degree_left_index]]*degree_left+UFF[rule_d[degree_right_index]]*degree_right;

    return out;

}
