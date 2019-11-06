#include "api_robot2.h"
/**************************************************************/
/* Solver of non-linear equations                             */
/**************************************************************/
float f1(float x, float z, float a, float b, float c) {
    return (a*x) - (b*z) + c;
}

float f2(float x, float z, float xo, float zo) {
    return (x-xo)*(x-xo) + (z-zo)*(z-zo) - 100;
}

float df1_x(a) {
    return a;
}

float df1_y() {
    return 1;
}

float df2_x(x,xo) {
    return 2*(x - xo);
}

float df2_y(z,zo) {
    return 2*(z - zo);
}

float newton_method(float x, float z, float xo, float zo, float a, float b, float c) {
    float J[4], Fx[2], det, invDet, inv_J[4], aux[2], s[2], k = 0;
    while (k < 10) {        // 10 iterações
        J[0] = df1_x(a);
        J[1] = df1_y();
        J[2] = df2_x(x, xo);
        J[3] = df2_y(z, zo);
        Fx[0] = -1 * f1(x, z, a, b, c);
        Fx[1] = -1 * f2(x, z, xo, zo);
        // Aqui temos a matriz jacobiana e a matriz Fx. Agora, devemos achar o passo de Newton, dado por s = (J^-1)*Fx
        det = (J[0]*J[3]) - (J[1]*J[2]);
        invDet = 1/det;

        inv_J[0] = J[3];
        inv_J[1] = -1 * J[1];
        inv_J[2] = -1 * J[2];
        inv_J[3] = J[0];
        aux[0] = (inv_J[0] * Fx[0]) + (inv_J[1] * Fx[1]);
        aux[1] = (inv_J[2] * Fx[0]) + (inv_J[3] * Fx[1]);
        s[0] = invDet * aux[0];
        s[1] = invDet * aux[1];

        x = x + s[0];
        z = z + s[1];
    }
}

// Repetir 10 vezes



/**************************************************************/
/* Strings                                                    */
/**************************************************************/
void reverse(char *s)
{
   int length, c;
   char *begin, *end, temp;
 
   length = string_length(s);
   begin  = s;
   end    = s;
 
   for (c = 0; c < length - 1; c++)
      end++;
 
   for (c = 0; c < length/2; c++)
   {        
      temp   = *end;
      *end   = *begin;
      *begin = temp;
 
      begin++;
      end--;
   }
}
 
int string_length(char *pointer)
{
   int c = 0;
 
   while( *(pointer + c) != '\0' )
      c++;
 
   return c;
}
// Implementation of itoa() 
char* itoa(int num, char* str, int base) 
{ 
    int i = 0; 
    int isNegative = 0; 
  
    /* Handle 0 explicitely, otherwise empty string is printed for 0 */
    if (num == 0) 
    { 
        str[i++] = '0'; 
        str[i] = '\0'; 
        return str; 
    } 
  
    // In standard itoa(), negative numbers are handled only with  
    // base 10. Otherwise numbers are considered unsigned. 
    if (num < 0 && base == 10) 
    { 
        isNegative = 0; 
        num = -num; 
    } 
  
    // Process individual digits 
    while (num != 0) 
    { 
        int rem = num % base; 
        str[i++] = (rem > 9)? (rem-10) + 'a' : rem + '0'; 
        num = num/base; 
    } 
  
    // If number is negative, append '-' 
    if (isNegative) 
        str[i++] = '-'; 
  
    str[i] = '\0'; // Append string terminator 
  
    // Reverse the string 
    reverse(str); 
  
    return str; 
}

/**************************************************************/
/* Path_Finder                                                */
/**************************************************************/

/**************************************************************/
/* Movement_controller                                        */
/**************************************************************/

/**************************************************************/
/* Main                                                       */
/**************************************************************/
int main(){
    //char string [7]= "mc404\n";
    set_torque(100,100);  
    //set_head_servo(0, 90);
    //set_head_servo(1, 90);
    //set_head_servo(2, 0);
    int dist;
    while(1){
        dist = get_us_distance();
        itoa(dist, string, 10);
        puts(string);
        puts("\n\0");
    }
    Vector3 aux = {.x = 0, .y = 0, .z = 0};
    while(0){
        get_current_GPS_position(&aux);
        if(aux.x + 5000 <= 715 || aux.x - 5000 <= 715){
            puts(string);
        }
    }
    char s[] = "sdfsd";
    
    puts(s);
    return 0;
}
