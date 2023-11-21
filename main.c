
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
   printf("\n ending at (%.4lf, %.4lf) at time %.4lf seconds.\n", xf, yf, t);
   return;
}

int main(void){
   
   double T, x, y, theta, v[50], w[50], d[50], x_next, y_next, theta_next, x10, y10, theta10, b1, b2;
   int k=0, robotNum =0,first_line;
   char name[50][50], buf1[80], buf2[80], *end1, *end2;
   RobotControl u;
   RobotState p, s;
   
   while(scanf("%s %s", buf1, buf2) == 2){
   b1 = strtod(buf1, &end1);
   b2 = strtod(buf2, &end2);
   if(b1==-1||b2==-1){break;}
   if(strcmp(buf1, "robot")==0){//
      if(robotNum>0){PrintRobot(name[robotNum-1], d[k], x, y, s.x, s.y, T*k);}
      robotNum++;
      strcpy(&name[robotNum-1][0], buf2);
      first_line = 1;
   }
   
   if(first_line == 1){
   scanf("%lf %lf %lf %lf", &T, &x, &y, &theta);
   s.x = x;
   s.y = y;
   s.theta = theta;
   d[0] = 0;
   k=0;
   }
   
   if(first_line == 0){
      v[k] = b1;
      w[k] = b2;
      
      u.v = v[k]; 
      u.w = w[k];
      p = s;
      s = robot(T, s, u);
      d[k] = d[k-1] + distance(p, s);
   }
   
   first_line=0;
   k++;
   
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
