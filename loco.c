#include "api_robot2.h"
/**************************************************************/
/* Solver of non-linear equations                             */
/**************************************************************/
<<<<<<< HEAD
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
=======
int df1_x(int a) {
    return a;
}

int df1_y(int b) {
    return -1 * b;
}

int df2_x(int x, int xo) {
    return 2*(x - xo);
}

int df2_y(int y, int yo) {
    return 2*(y - yo);
>>>>>>> a1dac295199a1628170d4a0dae9ddd8671a9924b
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

int intToStr(int x, char str[], int d) 
{ 
    int i = 0; 
    while (x) 
    { 
        str[i++] = (x%10) + '0'; 
        x = x/10; 
    } 
  
    // If number of digits required is more, then 
    // add 0s at the beginning 
    while (i < d) 
        str[i++] = '0'; 
  
    reverse(str); 
    str[i] = '\0'; 
    return i; 
} 

char* ftoa(float n, char *res, int afterpoint) 
{ 
    // Extract integer part 
    int ipart = (int)n; 
  
    // Extract floating part 
    float fpart = n - (float)ipart; 
  
    // convert integer part to string
    itoa(ipart, res, 10);
    int  i = string_length(res); 
    //int i = intToStr(ipart, res, 0); 
  
    // check for display option after point 
    if (afterpoint != 0) 
    { 
        res[i] = '.';  // add dot 
  
        // Get the value of fraction part upto given no. 
        // of points after dot. The third parameter is needed 
        // to handle cases like 233.007
        for(int j = 1; j <= afterpoint; j++){
            fpart = fpart * 10;
            itoa((int) fpart, res + i + j, 10);
            fpart = fpart - (int) fpart;
        }
        //fpart = fpart * pow(10, afterpoint); 
  
        //itoa((int)fpart, res + i + 1, 10); 
    } 
    return res;
} 

/**************************************************************/
/* Path_Finder                                                */
/**************************************************************/

/**************************************************************/
/* Movement_controller                                        */
/**************************************************************/
double pow(double num, int pot){
    double p = 1;
    for(int i=0; i<pot; i++){
        p = p * num;
    }
    return p;
}

double arctan(double x) {
    double ret = 0.0;
	const unsigned int BIG_NUM = 1E2;//1E6; // the larger this number, the more precise the approximation
    
    for (int i = 0; i < BIG_NUM; ++i) {
        double n = i * 2 + 1; // odd number
		int c = pow(-1, i); // coefficient alternating signs
        ret += c * pow(x, n) / n;
    }
    
    return ret;
}

void print_pos(){
    char* string;
    Vector3 aux = {.x = 0, .y = 0, .z = 0};
    get_current_GPS_position(&aux);
    itoa(aux.x, string, 10);
    puts(string);
    puts(" \0");
    itoa(aux.y, string, 10);
    puts(string);
    puts(" \0");
    itoa(aux.z, string, 10);
    puts(string);
    puts("\n\0");
}

void print_orientation(){
    char* string;
    Vector3 aux = {.x = 0, .y = 0, .z = 0};
    get_gyro_angles(&aux);
    itoa(aux.x, string, 10);
    puts(string);
    puts(" \0");
    itoa(aux.y, string, 10);
    puts(string);
    puts(" \0");
    itoa(aux.z, string, 10);
    puts(string);
    puts("\n\0");
}

/*Vector3* get_pos(){
    Vector3* vec = (Vector3*) malloc(sizeof(Vector3));
    get_current_GPS_position(&vec);
    return vec;
}*/

int get_y_angle(){
    Vector3 aux = {.x = 0, .y = 0, .z = 0};
    get_gyro_angles(&aux);
    return aux.y;
}

void go_to_pos(Vector3* pos){
    Vector3 aux = {.x = 0, .y = 0, .z = 0};
    get_current_GPS_position(&aux);
    int offset_angle = ((int)(pos->y-aux.y)/(pos->x-aux.x));
}

void rotate(int angle){
    puts("Rotacionando ");
    puts(itoa(angle, "", 10));
    puts(" graus\n\0");
    int initial = get_y_angle();
    int self = get_y_angle();
    set_torque(10,-10);
    print_orientation();
    while(self < (initial+angle)%360){
        self = get_y_angle();
    }
    print_orientation();
    set_torque(0, 0);
}

/**************************************************************/
/* Main                                                       */
/**************************************************************/
int main(){
    char string [7]= "mc404\n";
    //set_torque(100,100);  
    //set_torque(0,0);
    //rotate(90);
    //set_head_servo(0, 90);
    //set_head_servo(1, 90);
    //set_head_servo(2, 0);
    int dist;
    while(0){
        dist = get_us_distance();
        itoa(dist, string, 10);
        puts(string);
        puts("\n\0");
    }
    Vector3 aux = {.x = 0, .y = 0, .z = 0};
    while(0){
        //get_current_GPS_position(&aux);
        //itoa(aux.x, string, 10);
        //puts(string);
        //puts("\n\0");
        //print_pos();
        print_orientation();
    }
    float f = 2112.01234;//arctan(1);
    puts(ftoa(arctan(0.1), "", 3));
    char* s = "sdfsd\0";
    
    //puts(s);
    return 0;
}
