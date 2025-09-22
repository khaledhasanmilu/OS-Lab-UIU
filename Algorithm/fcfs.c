#include<stdio.h>

struct process{
  int pid;
  int arrival,burst,completion,waiting,turnaround;
};

int main(){
  int n;
  scanf("%d",&n);
  struct process p[n];
  for(int i=0;i<n;i++){
    p[i].pid=i+1;
    printf("Arrival Time :");
    scanf("%d",&p[i].arrival);
    printf("Burst Time :");
    scanf("%d",&p[i].burst);
  }
  for(int i=0;i<n-1;i++){
    for(int j=0;j<n-i-1;j++){
      if(p[j].arrival>p[j+1].arrival){
        struct process temp=p[j];
        p[j]=p[j+1];
        p[j+1]=temp;
      }
    }
  }
  int current_time=0, totalwt=0, totaltat=0;
  for(int i=0;i<n;i++){
    if(current_time<p[i].arrival){
      current_time=p[i].arrival;
    }
    p[i].completion=current_time+p[i].burst;
    p[i].turnaround=p[i].completion-p[i].arrival;
    p[i].waiting=p[i].turnaround-p[i].burst;
    current_time=p[i].completion;
    totalwt+=p[i].waiting;  
    totaltat+=p[i].turnaround;
  }
  for(int i=0;i<n;i++){
    printf("%d ",p[i].pid);
    printf("at:%d, bt:%d, ct:%d, tat:%d, wt:%d\n",p[i].arrival,p[i].burst,p[i].completion,p[i].turnaround,p[i].waiting);
  }
}