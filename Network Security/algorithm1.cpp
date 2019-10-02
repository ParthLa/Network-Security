#include<iostream>
using namespace std;

double c,w,b,T0,s,Rcpu,sigma,i,r,p;

___ check(){
 c=c+1;
 r=USAGEavg();
 if (r>Rcpu){
  p=c*w+ r*b;
  if (Random(0,100)<=p){
   AbortRequest();
  }
  else {
   SuspendRequest();
  }
 }
}

___ init (){
 double c=0;
 double w=1;
 double b=1;
 double T0 = 10;
 double s=5;
 double Rcpu = 0.75;
 double sigma = StdDev();
 double i=max(T0,sigma);
 timer(check,i);
}

int main(){
//Write your code here

}
