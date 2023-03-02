#include "zf_common_headfile.h"
#include "math.h"

#define Max_Column              150
#define Max_Row  60
#define GrayScale 256
int midpoint=0,endline=45;
//120↓→188
 uint8    dbimage[MT9V03X_H][MT9V03X_W],ipsdbimage[MT9V03X_H][MT9V03X_W];//二值化数组
 uint8    edgeL[MT9V03X_H],edgeR[MT9V03X_H],leftlineflag[MT9V03X_H],rightlineflag[MT9V03X_H];     //边界数组 flag：0丢线 1不确定 2确定
 uint8    midlinex[MT9V03X_H],midliney[MT9V03X_H];

uint8 BlackEndMR      = 0;
uint8 BlackEndML      = 0;
uint8 BlackEndLL      = 0;
uint8 BlackEndRR      = 0;
uint8 BlackEndL       = 0;
uint8 BlackEndM       = 0;
uint8 BlackEndR       = 0;
uint8 BlackEndMaxMax  = 0;
uint8 BlackEndMax     = 0;
uint8 DropRow         = 0;
uint8 Weight[60]={
0,0,0,0,0,0,0,0,0,0,           //0-9行，基本用不到
0,0,0,0,0,0,0,0,0,0,             //0-19行，基本用不到
1,1,1,2,2,3,4,4,5,5,        //20-29行 2,2,2,2,3,4,4,4,5,5,   6,6,7,7,8,8,9,9,9,9,
5,5,8,7,8,9,8,8,9,9,            //30-39行
8,8,7,7,6,6,5,5,4,4,           //40-49行
4,3,3,2,2,1,0,0,0,0,           //最近十行
                  };

uint8 frontimage[MT9V03X_H][MT9V03X_W];
int pixelCount[GrayScale];
float pixelPro[GrayScale];
int T_need,i,j,k,b,m;


int whiteblacklinenum2=0;

//二值化
void db_image(uint8 mode)
{
    if(mode==0)//用优化后的大津算法
        T_need = GetOSTU(mt9v03x_image);
    else if(mode==1)//固定阈值
        T_need=200;
    else
        T_need = my_adapt_threshold(mt9v03x_image[0],MT9V03X_W,MT9V03X_H);

    for (i = 0; i < MT9V03X_H; i++)
    {
        for (j = 0; j < MT9V03X_W; j++)
        {
            if (mt9v03x_image[i][j] > T_need)
            {
                ipsdbimage[i][j]=80;
                dbimage[i][j] = 1;
            }
            else
            {
                ipsdbimage[i][j]=150;
                dbimage[i][j] = 0;
            }
        }
    }
}
int startline(void)
{
    int whiteblacklinenum=0;
    static int fangchongfu;
    //检测黑白跳变点数量
        for(j=edgeL[MT9V03X_H-4],i=30,k=0;j<edgeR[MT9V03X_H-4];j++)
        {
            if(dbimage[i][j]==1&&dbimage[i][j+1]==0&&k==0)
            {
                k=1;
            }
            if(dbimage[i][j]==0&&dbimage[i][j+1]==1&&k==1)
            {
                whiteblacklinenum++;
                k=0;
            }
        }
        for(j=edgeL[MT9V03X_H-10],i=MT9V03X_H-1,k=0;j<edgeR[MT9V03X_H-10];j++)
        {
            if(dbimage[i][j]==1&&dbimage[i][j+1]==0&&k==0)
            {
                k=1;
            }
            if(dbimage[i][j]==0&&dbimage[i][j+1]==1&&k==1)
            {
                whiteblacklinenum2++;
                k=0;
            }
        }
        if(whiteblacklinenum>6&&fangchongfu!=1)
        {
            fangchongfu=1;
            whiteblacklinenum=0;
            return 1;
        }
        else if(fangchongfu==1&&whiteblacklinenum<=3)//当线消失后把防重置零
        {
            fangchongfu=0;
            whiteblacklinenum=0;
            return 0;
        }
    return 0;
}
int edge_line(uint8 dbimage[MT9V03X_H][MT9V03X_W])
{
  //初始化
  uint8 Lstart=0,L_max=0,L_min=0;
  uint8 Rstart=0,R_max=0,R_min=0;
  int range=65;

    for(i=0;i<MT9V03X_H-1;i++)
    {
        leftlineflag[i]=0;
        rightlineflag[i]=0;

    }
    seekLedge_frommid(59);
    seekRedge_frommid(59);
    for(uint8 hang=MT9V03X_H-2;hang>0;hang--)
  {
    //左边线
    //确定搜线范围
    //根据上一行确定本行寻线点
    if(leftlineflag[hang+1]==0)
    {
      L_max=MT9V03X_W-1;
      L_min=0;
      Lstart=MT9V03X_W*1/2-10;
    }
    else
    {
      L_max=edgeL[hang+1]+range;
      L_min=edgeL[hang+1]-range;
      Lstart=edgeL[hang+1];
      if(L_max>MT9V03X_W-1)
        L_max=MT9V03X_W-1;
      if(L_min<0)
        L_min=0;
    }
    //左边线
    if(dbimage[hang][Lstart]==0)
    {
      //向右找
      for(uint8 lie=Lstart;lie<L_max;lie++)
      {
        if(dbimage[hang][lie]==0&&dbimage[hang][lie+1]==1)
        {
            edgeL[hang]=lie;
            leftlineflag[hang]=1;
            break;
        }
      }
      //如果还没找到说明丢线
      if(leftlineflag[hang]==0)
      {
        edgeL[hang]=MT9V03X_W-2;
      }
    }
    else
    {
      //向左找
      for(uint8 lie=Lstart;lie>L_min;lie--)
      {
        if(dbimage[hang][lie]==1&&dbimage[hang][lie-1]==0)
        {
            edgeL[hang]=lie;
            leftlineflag[hang]=1;
            break;
        }
      }
      //如果还没找到说明丢线
      if(leftlineflag[hang]==0)
      {
        edgeL[hang]=1;
      }
    }
    //右边线
    //确定寻线范围
    if(rightlineflag[hang+1]==0)
    {
      Rstart=MT9V03X_W*1/2+10;
      R_max=MT9V03X_W-1;
      R_min=0;
    }
    else
    {
      Rstart=edgeR[hang+1];
      R_max=edgeR[hang+1]+range;
      R_min=edgeR[hang+1]-range;
      if(R_max>MT9V03X_W-1)
        R_max=MT9V03X_W-1;
      if(R_min<0)
        R_min=0;
    }
    //右边线
    if(dbimage[hang][Rstart]==0)
    {
      //向左找
      for(uint8 lie=Rstart;lie>R_min;lie--)
      {
        if(dbimage[hang][lie]==0&&dbimage[hang][lie-1]==1)
        {
          edgeR[hang]=lie;
          rightlineflag[hang]=1;
          break;
        }
      }
      //如果还没找到
      if(rightlineflag[hang]==0)
      {
        edgeR[hang]=1;
      }
    }
    else
    {
      //向右找
      for(uint8 lie=Rstart;lie<R_max;lie++)
      {
        if(dbimage[hang][lie]==1&&dbimage[hang][lie+1]==0)
        {
          edgeR[hang]=lie;
          rightlineflag[hang]=1;
          break;
        }
      }
      //如果还没找到
      if(rightlineflag[hang]==0)
      {
        edgeR[hang]=MT9V03X_W-2;
      }

      if(edgeR[hang]>=145) //这里有一个BUG没有解决，当右线没有找到时rightline应该是0，但是不知道为什么仍然是1，所以强制置零
      {
          rightlineflag[hang]=0;

      }
      if(edgeL[hang]<=5)
      {
          leftlineflag[hang]=0;

      }
    }
        midlinex[hang]=(edgeL[hang]+edgeR[hang])/2;
    }
    edgeL[0]=edgeL[1];
    leftlineflag[0]=leftlineflag[1];
    edgeR[0]=edgeR[1];
    rightlineflag[0]=rightlineflag[1];

    return 1;
}
int GetMidline(uint8 dbimage[MT9V03X_H][MT9V03X_W])//dbimage -> edimage,midlinex
{
    //求中点
    int WeightSum=0;
    for(uint8 i=57;i>DropRow;i--)//50
    {
        ipsdbimage[i][midlinex[i]]=0;//画中线
        midpoint+=midlinex[i]*Weight[i];
        WeightSum+=Weight[i];
    }
    //算出加权平均后中线的值
    if(WeightSum!=0)
    {
        midpoint=(midpoint/WeightSum);//+补偿值
    }
    midpoint = midlinex[endline];//重点！！45
    //标志位清零
    return 1;
}
uint8 Judge_leftlossline(uint8 edgeL[MT9V03X_H])//根据20-60行丢线数判断某行丢线 >20即
{
    int a,b;
    for( a=MT9V03X_H-1,b=0;a>20;a--)
    {
            if(leftlineflag[a]==0||edgeL[a]-edgeL[a+1]>10||edgeL[a]<10)
                b++;
    }
            if(b>25)
            {
                return 1;
            }
            else return 0;

    return 0;

}
uint8 Judge_rightlossline(uint8 edgeR[MT9V03X_H])
{
    int a,b;
    for(a=MT9V03X_H-1,b=0;a>20;a--)
    {
            if(rightlineflag[a]==0||edgeR[a-1]-edgeR[a]>=10||edgeR[a]>110)
                b++;
        }
            if(b>25)
            {
                return 1;
            }
            else return 0;

        return 0;
}
uint8 my_adapt_threshold(uint8 *image, uint16 col, uint16 row)//大津法阈值
{
        float w0, w1, u0tmp, u1tmp, u0, u1, u, deltaTmp, deltaMax = 0;
    uint16 width = col;
    uint16 height = row;
    int threshold;
    int pixelSum = width * height/4;

    uint8* data = image;  //指向像素数据的指针
    for (i = 0; i < GrayScale; i++)
    {
        pixelCount[i] = 0;
        pixelPro[i] = 0;
    }

    uint32 gray_sum=0;
    //统计灰度级中每个像素在整幅图像中的个数
    for (i = 0; i < height; i+=2)
    {
        for (j = 0; j < width; j+=2)
        {
            pixelCount[(int)data[i * width + j]]++;  //将当前的点的像素值作为计数数组的下标
            gray_sum+=(int)data[i * width + j];       //灰度值总和
        }
    }

    //计算每个像素值的点在整幅图像中的比例

    for (i = 0; i < GrayScale; i++)
    {
        pixelPro[i] = (float)pixelCount[i] / pixelSum;

    }

    //遍历灰度级[0,255]


    w0 = w1 = u0tmp = u1tmp = u0 = u1 = u = deltaTmp = 0;
    for (j = 0; j < GrayScale; j++)
    {

        w0 += pixelPro[j];  //背景部分每个灰度值的像素点所占比例之和   即背景部分的比例
        u0tmp += j * pixelPro[j];  //背景部分       每个灰度值的点的比例 *灰度值

       w1=1-w0;
       u1tmp=gray_sum/pixelSum-u0tmp;

        u0 = u0tmp / w0;              //背景平均灰度
        u1 = u1tmp / w1;              //前景平均灰度
        u = u0tmp + u1tmp;            //全局平均灰度
        deltaTmp = w0 * pow((u0 - u), 2) + w1 * pow((u1 - u), 2);
        if (deltaTmp > deltaMax)
        {
            deltaMax = deltaTmp;
            threshold = j;
        }
        if (deltaTmp < deltaMax)
        {
            break;
        }
     }
     return threshold;
}
int GetOSTU(uint8  image[MT9V03X_H][MT9V03X_W])//优化后的大津算法
{
        int16 i,j;
    uint32 Amount = 0;
    uint32 PixelBack = 0;
    uint32 PixelIntegralBack = 0;
    uint32 PixelIntegral = 0;
    int32 PixelIntegralFore = 0;
    int32 PixelFore = 0;
    float OmegaBack, OmegaFore, MicroBack, MicroFore, SigmaB, Sigma; // 类间方差;
    int16 MinValue, MaxValue;
    int Threshold = 0;
    uint8 HistoGram[256];

    for (j = 0; j < 256; j++)  HistoGram[j] = 0; //初始化灰度直方图

    for (j = 0; j < MT9V03X_H; j++)
    {
        for (i = 0; i < MT9V03X_W; i++)
        {
            HistoGram[image[j][i]]++; //统计灰度级中每个像素在整幅图像中的个数
        }
    }

    for (MinValue = 0; MinValue < 256 && HistoGram[MinValue] == 0; MinValue++) ;        //获取最小灰度的值
    for (MaxValue = 255; MaxValue > MinValue && HistoGram[MinValue] == 0; MaxValue--) ; //获取最大灰度的值

    if (MaxValue == MinValue)     return MaxValue;         // 图像中只有一个颜色
    if (MinValue + 1 == MaxValue)  return MinValue;        // 图像中只有二个颜色

    for (j = MinValue; j <= MaxValue; j++)    Amount += HistoGram[j];        //  像素总数

    PixelIntegral = 0;
    for (j = MinValue; j <= MaxValue; j++)
    {
        PixelIntegral += HistoGram[j] * j;//灰度值总数
    }
    SigmaB = -1;
    for (j = MinValue; j < MaxValue; j++)
    {
        PixelBack = PixelBack + HistoGram[j];   //前景像素点数
        PixelFore = Amount - PixelBack;         //背景像素点数
        OmegaBack = (float)PixelBack / Amount;//前景像素百分比
        OmegaFore = (float)PixelFore / Amount;//背景像素百分比
        PixelIntegralBack += HistoGram[j] * j;  //前景灰度值
        PixelIntegralFore = PixelIntegral - PixelIntegralBack;//背景灰度值
        MicroBack = (float)PixelIntegralBack / PixelBack;   //前景灰度百分比
        MicroFore = (float)PixelIntegralFore / PixelFore;   //背景灰度百分比
        Sigma = OmegaBack * OmegaFore * (MicroBack - MicroFore) * (MicroBack - MicroFore);//计算类间方差
        if (Sigma > SigmaB)                    //遍历最大的类间方差g //找出最大类间方差以及对应的阈值
        {
            SigmaB = Sigma;
            Threshold = j;
        }
    }
    return Threshold;                        //返回最佳阈值;
}
void get_route_endline(void)//寻找截止行
{
  //截止行的判断需要消除折点，这里不破坏原来左右线数组
  int left_line[MT9V03X_H],right_line[MT9V03X_H];

  //复制
  for(uint8 hang=MT9V03X_H-1;hang>0;hang--)
  {
    left_line[hang]=edgeL[hang];
    right_line[hang]=edgeR[hang];
  }
  //消折点（突变点）
  for(uint8 hang=MT9V03X_H-3;hang>0;hang--)
  {
    int temp=0;
    //左
    if((left_line[hang+1]-left_line[hang])>0)
    {
        temp=left_line[hang+1];
        left_line[hang+1]=left_line[hang];
        left_line[hang]=temp;
    }
    //右
    if((right_line[hang]-right_line[hang+1])>0)
    {
        temp=right_line[hang+1];
        right_line[hang+1]=right_line[hang];
        right_line[hang]=temp;
    }
  }
  endline=DropRow;
  //截止行判断
  for(uint8 hang=MT9V03X_H-5;hang>endline;hang--)
  {
    //隔离赛道外的边线
    if((right_line[hang-1]<left_line[hang])
       ||(left_line[hang-1]>right_line[hang]))
    {
        endline=hang;
        break;
    }
  }

  if(DropRow<10&&BlackEndM>50)
    endline=DropRow;

  if(endline>48)    endline=48;

}
void GetBlackEndParam(void)//寻黑线截止
{

  unsigned char LEndFlag  = 0;//标志位
  unsigned char MEndFlag  = 0;
  unsigned char REndFlag  = 0;
  unsigned char MREndFlag = 0;
  unsigned char MLEndFlag = 0;
  unsigned char LLEndFlag = 0;
  unsigned char RREndFlag = 0;

  int i=0;

  BlackEndMR=0;//清零
  BlackEndML=0;
  BlackEndLL=0;
  BlackEndRR=0;
  BlackEndL=0;
  BlackEndM=0;
  BlackEndR=0;

  for (i=MT9V03X_H-1;i>0;i--)
  {
    if(dbimage[i][75] == 1 && !MEndFlag )//
    {
        BlackEndM++;//中黑线截至行
    }
    else if(i > 1 && dbimage[i-1][75] == 0 && dbimage[i-2][75] == 0)//连续两行是黑色
    {
        MEndFlag = 1;
    }
    if(dbimage[i][37] == 1 && !LEndFlag )
        {
        BlackEndL++;//左黑线截至行
        }
    else if(i > 1 && dbimage[i-1][37] == 0 && dbimage[i-2][37] == 0)
    {
        LEndFlag = 1;
    }
    if(dbimage[i][112] == 1 && !REndFlag )
    {
        BlackEndR++;//右黑线截至行
        }
    else if(i > 1 && dbimage[i-1][112] == 0 && dbimage[i-2][112] == 0)
    {
        REndFlag = 1;
    }
    if(dbimage[i][56] == 1 && !MLEndFlag )
    {
        BlackEndML++;
    }
    else if(i > 1 && dbimage[i-1][56] == 0 && dbimage[i-2][56] == 0)
    {
        MLEndFlag = 1;
    }
    if(dbimage[i][93] == 1 && !MREndFlag )
    {
        BlackEndMR++;
    }
    else if(i > 1 && dbimage[i-1][93] == 0 && dbimage[i-2][93] == 0)
    {
        MREndFlag = 1;
        }
    if(dbimage[i][10] == 1 && !LLEndFlag )
    {
        BlackEndLL++;
        }
    else if(i > 1 && dbimage[i-1][10] == 0 && dbimage[i-2][10] == 0)
    {
        LLEndFlag = 1;
        }
    if(dbimage[i][MT9V03X_W-10] == 1 && !RREndFlag )
    {
        BlackEndRR++;
        }
    else if(i > 1 && dbimage[i-1][MT9V03X_W-10] == 0 && dbimage[i-2][MT9V03X_W-10] == 0)
    {
        RREndFlag = 1;
        }
   }
    BlackEndMax =max(BlackEndL,BlackEndM);//取大值
    BlackEndMax =max(BlackEndMax,BlackEndR);
    BlackEndMaxMax =max(BlackEndMax,BlackEndMR);
    BlackEndMaxMax =max(BlackEndMaxMax,BlackEndML);
    BlackEndMaxMax =max(BlackEndMaxMax,BlackEndLL);
    BlackEndMaxMax =max(BlackEndMaxMax,BlackEndRR);
    if(BlackEndMaxMax>60)
    {
        BlackEndMaxMax=57;
    }
    DropRow=60-BlackEndMaxMax;//封顶的行数
            //printf("DropRow:%d\n",DropRow);
    if(DropRow>=57) DropRow=57;

    if(DropRow<=10) DropRow=10;



 }
void connect_line(uint8  hang_start,uint8 hang_end,uint8 edima[MT9V03X_H])//连线 hang_start<hang_end
{
    int k,j;
    if(hang_start<=hang_end)
    {
        for(k=hang_start,j=edima[hang_start];k<hang_end;k++)
        {
            if(dbimage[k][j]!=0)
            {
                dbimage[k][j]=0;
                edima[k]=j;

                j = edima[hang_start]+(int)(((edima[hang_end] - edima[hang_start])*1.0/(hang_end - hang_start)) * (k - hang_start));
            }
        }
    }
}
void seekLedge_frommid(int a)
{
    for(i=a,j=MT9V03X_W /2;j>0;j--)
    {
        if(dbimage[i][j-1]==0&&dbimage[i][j]==0&&dbimage[i][j+1]==1)
        {
            edgeL[i]=j+1;
            leftlineflag[i]=1;
            break;
        }
        if(j<5&&dbimage[i][j]==1)
        {
            edgeL[i]=j;
            leftlineflag[i]=0;
            break;
        }
    }
}
void seekRedge_frommid(int a)
{
    for(i=a,j=MT9V03X_W /2;j<MT9V03X_W;j++)
    {
        if(dbimage[i][j-1]==1&&dbimage[i][j]==0&&dbimage[i][j+1]==0)
        {
            edgeR[i]=j-1;
            rightlineflag[i]=1;
            break;
        }
        if(j>MT9V03X_W-5&&dbimage[i][j]==1)
        {
            edgeR[i]=j;
            rightlineflag[i]=0;
            break;
        }
    }
}
int variance(uint8 edge[MT9V03X_H])
{


    int sum;
    uint8 average;
    uint8 variance;
    int i;
    for(sum=0,i=0;i<MT9V03X_H;i++)
    {
        sum+=edge[i];

    }
    average = sum/MT9V03X_H;
    for(variance=0,i=0;i<MT9V03X_H;i++)
    {
        variance += pow(edge[i]-average,2)/MT9V03X_H;
    }
    return variance;
}
int max(int a,int b)
{
  return a>b?a:b;
}
