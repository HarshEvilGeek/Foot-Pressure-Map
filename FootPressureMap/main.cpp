//
//  main.cpp
//  FootPressureMap
//
//  Created by Akhil Verghese on 4/12/13.
//  Copyright (c) 2013 Akhil Verghese. All rights reserved.
//

#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

bool myfunction(pair <int, int> x, pair <int, int> y)
{
    if(x.second==y.second)
        return(x.first<y.first);
    else
        return(x.second<y.second);
}
int main(int argc, const char * argv[])
{
    int frames,sensors,f;
    double slope1,slope2,slope3,slope4,p1slope,p2slope,p3slope,c1,c2,c3,x1,x2,x3,x4,y1,y2,y3,y4;
    bool gooddata=1;
    vector <pair <int, int> > Outline(50);
    cin>>frames>>sensors;
    double PressureMap[49][21];
    vector <pair <int, int> > SensorPositions(sensors);
    for(int i =0;i<sensors;i++)
    {
        cin>>f;
        cin>>SensorPositions[f-1].first;
        cin>>SensorPositions[f-1].second;
    }
    for(int i =0;i<49;i++)
    {
        cin>>f;
        cin>>Outline[f].first;
        cin>>Outline[f].second;
    }
    sort(SensorPositions.begin(),SensorPositions.begin()+sensors,myfunction);
    for(int i =0;i<sensors;i++)
    {
        if((i>1)&(i<(sensors-1)))
        {
            if((SensorPositions[i].second!=SensorPositions[i-1].second)&(SensorPositions[i].second!=SensorPositions[i+1].second))
                gooddata=0;
            
        }
    }
    if(gooddata==0)
    {
        cout<<"You need to have at least two sensors on every row. Otherwise, this algorithm will be innacurate"<<endl;
        return 1;
    }
    int x;
    
    int noofpoints;
    int rows[20],rowcount=0;
    vector <pair <int, double> > Pvalues(10),midpoint(5);
    for(int w=0;w<frames;w++)
    {
        for(int r=0;r<49;r++)
        {
            for(int j=0;j<21;j++)
                PressureMap[r][j]=0;
        }
        for(int j=0;j<sensors;j++)
        {
            cin>>x;
            cin>>f;
            cin>>PressureMap[SensorPositions[f-1].second][SensorPositions[f-1].first];
        }
        for(int sens=0;sens<sensors;sens++)
            rows[sens]=-1;
        for(int j=0;j<49;j++)
        {
            int rightrow=0;
            for(int s=0;s<sensors;s++)
            {
                if(j==SensorPositions[s].second)
                {
                    rightrow++;
                    if(rows[rowcount-1]!=j)
                    {
                        rows[rowcount]=j;
                        rowcount++;
                    }
                    Pvalues[rightrow].second=PressureMap[SensorPositions[s].second][SensorPositions[s].first];
                    Pvalues[rightrow].first=SensorPositions[s].first;
                }
            }
            if(rightrow>0)
            {
                Pvalues[0].second=0;
                Pvalues[0].first=Outline[j].first;
                Pvalues[rightrow+1].second=0;
                Pvalues[rightrow+1].first=Outline[j].second;
                if(rightrow==2)
                {
                    slope1=(Pvalues[1].second-Pvalues[0].second)/(Pvalues[1].first-Pvalues[0].first);
                    slope2=(Pvalues[2].second-Pvalues[1].second)/(Pvalues[2].first-Pvalues[1].first);
                    slope3=(Pvalues[3].second-Pvalues[2].second)/(Pvalues[3].first-Pvalues[2].first);
                    p1slope=((slope1*(Pvalues[1].first-Pvalues[0].first))+(slope2*(Pvalues[2].first-Pvalues[1].first)))/(Pvalues[2].first-Pvalues[0].first);
                    p2slope=((slope2*(Pvalues[2].first-Pvalues[1].first))+(slope3*(Pvalues[3].first-Pvalues[2].first)))/(Pvalues[3].first-Pvalues[1].first);
                    c1=Pvalues[1].second-(p1slope*Pvalues[1].first);
                    c2=Pvalues[2].second-(p2slope*Pvalues[2].first);
                    x1=0;
                    y1=c1;
                    x2=Pvalues[1].first;
                    y2=Pvalues[1].second;
                    x3=0;
                    y3=c2;
                    x4=Pvalues[2].first;
                    y4=Pvalues[2].second;
                    midpoint[1].first=((((x1*y2)-(y1*x2))*(x3-x4))-((x1-x2)*((x3*y4)-(y3*x4))))/(((x1-x2)*(y3-y4))-((y1-y2)-(x3-x4)));
                    midpoint[1].second=((((x1*y2)-(y1*x2))*(y3-y4))-((y1-y2)*((x3*y4)-(y3*x4))))/(((x1-x2)*(y3-y4))-((y1-y2)-(x3-x4)));
                    if(midpoint[1].first<Pvalues[1].first)
                    {
                        midpoint[1].first=Pvalues[1].first;
                        midpoint[1].second=Pvalues[1].second;
                    }
                    if(midpoint[1].first>Pvalues[2].first)
                    {
                        midpoint[1].first=Pvalues[2].first;
                        midpoint[1].second=Pvalues[2].second;
                    }
                    midpoint[0].first=(Pvalues[1].first+Pvalues[0].first)/2;
                    midpoint[2].first=(Pvalues[2].first+Pvalues[3].first)/2;
                    midpoint[0].second=(p1slope*midpoint[0].first)+c1;
                    midpoint[2].second=(p2slope*midpoint[2].first)+c2;
                    for(int k=0;k<21;k++)
                    {
                        if(k<Pvalues[0].first)
                            PressureMap[j][k]=0;
                        else if(k<midpoint[0].first)
                            PressureMap[j][k]=(((double)(k-Pvalues[0].first))*midpoint[0].second)/((double)(midpoint[0].first-Pvalues[0].first));
                        else if(k<Pvalues[1].first)
                            PressureMap[j][k]=midpoint[0].second+((((double)(k-midpoint[0].first))*(Pvalues[1].second-midpoint[0].second))/((double)(Pvalues[1].first-midpoint[0].first)));
                        else if(k<midpoint[1].first)
                            PressureMap[j][k]=Pvalues[1].second+((((double)(k-Pvalues[1].first))*(midpoint[1].second-Pvalues[1].second))/((double)(midpoint[1].first-Pvalues[1].first)));
                        else if(k<Pvalues[2].first)
                            PressureMap[j][k]=midpoint[1].second+((((double)(k-midpoint[1].first))*(Pvalues[2].second-midpoint[1].second))/((double)(Pvalues[2].first-midpoint[1].first)));
                        else if(k<midpoint[2].first)
                            PressureMap[j][k]=Pvalues[2].second+((((double)(k-Pvalues[2].first))*(midpoint[2].second-Pvalues[2].second))/((double)(midpoint[2].first-Pvalues[2].first)));
                        else if(k<Pvalues[3].first)
                            PressureMap[j][k]=midpoint[2].second+((((double)(k-midpoint[2].first))*(-midpoint[2].second))/((double)(Pvalues[3].first-midpoint[2].first)));
                        else
                            PressureMap[j][k]=0;
                    }
                }
                if(rightrow==3)
                {
                    slope1=(Pvalues[1].second-Pvalues[0].second)/(Pvalues[1].first-Pvalues[0].first);
                    slope2=(Pvalues[2].second-Pvalues[1].second)/(Pvalues[2].first-Pvalues[1].first);
                    slope3=(Pvalues[3].second-Pvalues[2].second)/(Pvalues[3].first-Pvalues[2].first);
                    slope4=(Pvalues[4].second-Pvalues[3].second)/(Pvalues[4].first-Pvalues[3].first);
                    p1slope=((slope1*(Pvalues[1].first-Pvalues[0].first))+(slope2*(Pvalues[2].first-Pvalues[1].first)))/(Pvalues[2].first-Pvalues[0].first);
                    p2slope=((slope2*(Pvalues[2].first-Pvalues[1].first))+(slope3*(Pvalues[3].first-Pvalues[2].first)))/(Pvalues[3].first-Pvalues[1].first);
                    p3slope=((slope3*(Pvalues[3].first-Pvalues[2].first))+(slope4*(Pvalues[4].first-Pvalues[3].first)))/(Pvalues[4].first-Pvalues[2].first);
                    c1=Pvalues[1].second-(p1slope*Pvalues[1].first);
                    c2=Pvalues[2].second-(p2slope*Pvalues[2].first);
                    c3=Pvalues[3].second-(p3slope*Pvalues[3].first);
                    x1=0;
                    y1=c1;
                    x2=Pvalues[1].first;
                    y2=Pvalues[1].second;
                    x3=0;
                    y3=c2;
                    x4=Pvalues[2].first;
                    y4=Pvalues[2].second;
                    midpoint[1].first=((((x1*y2)-(y1*x2))*(x3-x4))-((x1-x2)*((x3*y4)-(y3*x4))))/(((x1-x2)*(y3-y4))-((y1-y2)-(x3-x4)));
                    midpoint[1].second=((((x1*y2)-(y1*x2))*(y3-y4))-((y1-y2)*((x3*y4)-(y3*x4))))/(((x1-x2)*(y3-y4))-((y1-y2)-(x3-x4)));
                    if(midpoint[1].first<Pvalues[1].first)
                    {
                        midpoint[1].first=Pvalues[1].first;
                        midpoint[1].second=Pvalues[1].second;
                    }
                    if(midpoint[1].first>Pvalues[2].first)
                    {
                        midpoint[1].first=Pvalues[2].first;
                        midpoint[1].second=Pvalues[2].second;
                    }
                    x1=0;
                    y1=c2;
                    x2=Pvalues[2].first;
                    y2=Pvalues[2].second;
                    x3=0;
                    y3=c3;
                    x4=Pvalues[3].first;
                    y4=Pvalues[3].second;
                    midpoint[2].first=((((x1*y2)-(y1*x2))*(x3-x4))-((x1-x2)*((x3*y4)-(y3*x4))))/(((x1-x2)*(y3-y4))-((y1-y2)-(x3-x4)));
                    midpoint[2].second=((((x1*y2)-(y1*x2))*(y3-y4))-((y1-y2)*((x3*y4)-(y3*x4))))/(((x1-x2)*(y3-y4))-((y1-y2)-(x3-x4)));
                    if(midpoint[2].first<Pvalues[2].first)
                    {
                        midpoint[2].first=Pvalues[2].first;
                        midpoint[2].second=Pvalues[2].second;
                    }
                    if(midpoint[2].first>Pvalues[3].first)
                    {
                        midpoint[2].first=Pvalues[3].first;
                        midpoint[2].second=Pvalues[3].second;
                    }
                    midpoint[0].first=(Pvalues[1].first+Pvalues[0].first)/2;
                    midpoint[3].first=(Pvalues[3].first+Pvalues[4].first)/2;
                    midpoint[0].second=(p1slope*midpoint[0].first)+c1;
                    midpoint[3].second=(p3slope*midpoint[3].first)+c3;
                    for(int k=0;k<21;k++)
                    {
                        if(k<Pvalues[0].first)
                            PressureMap[j][k]=0;
                        else if(k<midpoint[0].first)
                            PressureMap[j][k]=(((double)(k-Pvalues[0].first))*midpoint[0].second)/((double)(midpoint[0].first-Pvalues[0].first));
                        else if(k<Pvalues[1].first)
                            PressureMap[j][k]=midpoint[0].second+((((double)(k-midpoint[0].first))*(Pvalues[1].second-midpoint[0].second))/((double)(Pvalues[1].first-midpoint[0].first)));
                        else if(k<midpoint[1].first)
                            PressureMap[j][k]=Pvalues[1].second+((((double)(k-Pvalues[1].first))*(midpoint[1].second-Pvalues[1].second))/((double)(midpoint[1].first-Pvalues[1].first)));
                        else if(k<Pvalues[2].first)
                            PressureMap[j][k]=midpoint[1].second+((((double)(k-midpoint[1].first))*(Pvalues[2].second-midpoint[1].second))/((double)(Pvalues[2].first-midpoint[1].first)));
                        else if(k<midpoint[2].first)
                            PressureMap[j][k]=Pvalues[2].second+((((double)(k-Pvalues[2].first))*(midpoint[2].second-Pvalues[2].second))/((double)(midpoint[2].first-Pvalues[2].first)));
                        else if(k<Pvalues[3].first)
                            PressureMap[j][k]=midpoint[2].second+((((double)(k-midpoint[2].first))*(Pvalues[3].second-midpoint[2].second))/((double)(Pvalues[3].first-midpoint[2].first)));
                        else if(k<midpoint[3].first)
                            PressureMap[j][k]=Pvalues[3].second+((((double)(k-Pvalues[3].first))*(midpoint[3].second-Pvalues[3].second))/((double)(midpoint[3].first-Pvalues[3].first)));
                        else if(k<Pvalues[4].first)
                            PressureMap[j][k]=midpoint[3].second+((((double)(k-midpoint[3].first))*(-midpoint[3].second))/((double)(Pvalues[4].first-midpoint[3].first)));
                        else
                            PressureMap[j][k]=0;
                    }
                }
            }
        }
        
        for(int j=0;j<21;j++)
        {
            noofpoints=0;
            int q=0;
            bool flag=0;
            for(int x=0;x<49;x++)
            {
                if((j>Outline[x].first)&(j<Outline[x].second)&(flag==0))
                {
                    noofpoints+=2;
                    Pvalues[q].first=x-1;
                    Pvalues[q].second=0;
                    q++;
                    flag=1;
                }
                if((j<=Outline[x].first|j>=Outline[x].second)&flag)
                {
                    flag=0;
                    Pvalues[q].first=x-1;
                    Pvalues[q].second=0;
                    q++;
                }
                for(int s=0;s<rowcount;s++)
                {
                    if(flag&(x==rows[s]))
                    {
                        noofpoints++;
                        Pvalues[q].first=x;
                        Pvalues[q].second=PressureMap[x][j];
                        q++;
                    }
                }
            }
            q=0;
            int stage=0,lastpos=0;
            double last=0;
            for(int k=0;k<49;k++)
            {
                if(k<Pvalues[stage].first)
                    PressureMap[k][j]=last+(((k-lastpos)*(Pvalues[stage].second-last))/(Pvalues[stage].first-lastpos));
                if(k==Pvalues[stage].first)
                {
                    PressureMap[k][j]=Pvalues[stage].second;
                    last=Pvalues[stage].second;
                    lastpos=Pvalues[stage].first;
                    stage++;
                }
                if(stage==noofpoints)
                    break;
            }
            
        }
        
        for(int j=0;j<49;j++)
        {
            for(int k=0;k<21;k++)
                cout<<(int)PressureMap[j][k]<<" ";
            cout<<endl;
        }
        
        
    }
    return 0;
}

