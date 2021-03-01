#include <stdio.h>
#include <math.h>



float compute(float Prin, float Rpcnt, int Num)
{ 
   float R ;
   R = Rpcnt / 100;
   return (Prin * R * pow(1+R, Num) / (12 * (pow(1+R, Num) -1)));
}

float ComputeFahtoCel(float fahr)
{
  float celsius;
  
  celsius = (5.0/9.0) * (fahr-32.0);
  
  return celsius;	
}	

float ComputeCeltoFah(float Celc)
{
   float fahrenheit;
   
   fahrenheit = (9.0*Celc) / 5.0 + 32.0;
   
   return fahrenheit;	
}	
