#include "api_robot2.h"
/**************************************************************/
/* Solver of non-linear equations                             */
/**************************************************************/
int df1_x() {
    return a;
}

int df1_y() {
    return -1 * b;
}

int df2_x() {
    return 2*(x - xo);
}

int df2_y() {
    return 2*(y - yo);
}


/* A utility function to reverse a string  */
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

int main(){
    //char string [7]= "mc404\n";
    set_torque(100,100);  
    //set_head_servo(0, 90);
    //set_head_servo(1, 90);
    //set_head_servo(2, 0);
    /* int dist;
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
