
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<math.h> // do not forget to compile : gcc main.c -lm
#define PI 3.141592654


typedef struct RobotState_struct{
	double x;
	double y;
	double theta;
}RobotState;

typedef struct RobotControl_struct{
   double v;
   double w;
}RobotControl;
/*
typedef struct RobotNode_struct{
   char name[50];
   RobotState f;
   RobotState i;
   struct RobotNode_struct *nextNodePtr;
}RobotNode;

void InsertAfter(RobotNode * thisNode, RobotNode * newNode){
   RobotNode* tmpNext = NULL;
   tmpNext = thisNode->nextNodePtr; // Remember next
   thisNode->nextNodePtr = newNode; // this -- new -- 
   newNode->nextNodePtr = tmpNext;
   return;
}
void FreeRobotList(RobotNode* head){
   RobotNode* tmp;
   while (head != NULL)
    {
       tmp = head;
       head = head->nextNodePtr;
       free(tmp);
    }
    return;
}*/

double X_k_1(double x_k, double T, double theta_k, double v_k, double w_k){
   if(fabs(w_k) < 0.000000001){
      return x_k + T*cos(theta_k)*v_k;
   }
   else{
      return x_k + (v_k/w_k)*(sin(theta_k + T*(w_k))-sin(theta_k));
   }
}

double Y_k_1(double y_k, double T, double theta_k, double v_k, double w_k){
   if(fabs(w_k) < 0.000000001){
      return y_k + T*sin(theta_k)*v_k;
   }
   else{
      return y_k - (v_k/w_k)*(cos(theta_k + T*(w_k))-cos(theta_k));
   }
}

double theta_k_1(double theta_k, double T, double v_k, double w_k){
   if(fabs(w_k) < 0.000000001){
      return theta_k;
   }
   else{ 
      return theta_k + T*w_k;
   }
   
}
RobotState robot(double T, RobotState s, RobotControl ctrl){
   RobotState next;
   next.x = X_k_1(s.x, T, s.theta, ctrl.v, ctrl.w);
   next.y = Y_k_1(s.y, T, s.theta, ctrl.v, ctrl.w);
   next.theta = theta_k_1(s.theta, T, ctrl.v, ctrl.w);
   return next;
}
double sq(double x);

double distance(RobotState s1, RobotState s2);

void PrintRobot(char *name, double d, double x0, double y0, double xf, double yf, double t){
   printf("Robot %s has traveled %.4lf meters, starting at (%.4lf, %.4lf)", name, d, y0, x0);
   printf(" and ending at (%.4lf, %.4lf) at time %.4lf seconds.\n", xf, yf, t);
   return;
}

int main(void){
   
   double T[5], x[5], y[5], theta[5], v[150], w[150], d[150], b1, b2, distances[5], K[5];
   int k=0, robotNum =0,i;
   char name[5][30], buf1[10], buf2[30], *end1, *end2;
   RobotControl u;
   RobotState p[5], s[5];
   d[0]=0;
   while(scanf("%s %s", buf1, buf2) == 2){
   
   b1 = strtod(buf1, &end1);
   b2 = strtod(buf2, &end2);
   //if(b1==-1||b2==-1){break;}//PrintRobot(name[robotNum-1], d[k], x, y, s.x, s.y, T*k);
   if(strcmp(buf1, "robot")==0){//distances[robotNum-1] = d[k];
      if(robotNum>0){distances[robotNum-1] = d[k-1];K[robotNum-1]=k-1;}
      robotNum++;
      strcpy(&name[robotNum-1][0], buf2);
      i = 1;
   }
   
   if(i== 1){
   scanf("%lf %lf %lf %lf", &T[robotNum-1], &x[robotNum-1], &y[robotNum-1], &theta[robotNum-1]);
   s[robotNum-1].x = x[robotNum-1];
   s[robotNum-1].y = y[robotNum-1];
   s[robotNum-1].theta = theta[robotNum - 1];
   d[0] = 0;
   k=0;
   }
   
   if(i == 0){
      v[k] = b1;
      w[k] = b2;
      
      u.v = v[k]; 
      u.w = w[k];
      p[robotNum-1] = s[robotNum-1];
      s[robotNum -1] = robot(T[robotNum-1], s[robotNum-1], u);
      d[k] = d[k-1] + distance(p[robotNum-1], s[robotNum-1]);
   }
   
   i=0;
   k++;
   
   }//end of data aquisition
   //get distances and timestep for the last robot
   	distances[robotNum-1] = d[k-1];
   	K[robotNum-1]=k-1;
  
   //print all robots
   printf("Number of robots: %d\n", robotNum);
   for(i=0;i<robotNum; i++){
      if(K[i]>10){PrintRobot(name[i], distances[i], y[i], x[i], s[i].x, s[i].y, T[i]*K[i]);}
      else{
         PrintRobot(name[i], distances[i], x[i], y[i], s[i].x, s[i].y, T[i]*K[i]);
      }
   }
   return 0;
}

double sq(double x){
   return x*x;
}

double distance(RobotState s1, RobotState s2){
   double d, sx2 = sq(s1.x - s2.x), sy2 = sq(s1.y - s2.y);
   d = sqrt(sx2 + sy2);
   return d;
}
