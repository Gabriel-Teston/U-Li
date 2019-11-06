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

float* newton_method(float x, float z, float xo, float zo, float a, float b, float c) {
    float J[4], Fx[2], det, invDet, inv_J[4], aux[2], s[2], res[2], k = 0;
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

    res[0] = x;
    res[1] = z;
    return res;
}


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
    int ipart = (int)(n-0.5);
  
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

double fatorial(int x){
    if (x)
        return 1.0;
    else
        return x*fatorial(x-1);
}

double rad(double x){
    while (x >= 360){
        x -= 360;
    }

    return (x * 3.1415) / 180;
}

double sin_table = { 0, 0.017452 ,0.034899 ,0.052336 ,0.069756 ,0.087156 ,0.104528 ,0.121869 ,0.139173 ,0.156434 ,0.173648 ,0.190809 ,0.207912 ,0.224951 ,0.241922 ,0.258819 ,0.275637 ,0.292372 ,0.309017 ,0.325568 ,0.34202 ,0.358368 ,0.374607 ,0.390731 ,0.406737 ,0.422618 ,0.438371 ,0.45399 ,0.469472 ,0.48481 ,0.5 ,0.515038 ,0.529919 ,0.544639 ,0.559193 ,0.573576 ,0.587785 ,0.601815 ,0.615661 ,0.62932 ,0.642788 ,0.656059 ,0.669131 ,0.681998 ,0.694658 ,0.707107 ,0.71934 ,0.731354 ,0.743145 ,0.75471 ,0.766044 ,0.777146 ,0.788011 ,0.798636 ,0.809017 ,0.819152 ,0.829038 ,0.838671 ,0.848048 ,0.857167 ,0.866025 ,0.87462 ,0.882948 ,0.891007 ,0.898794 ,0.906308 ,0.913545 ,0.920505 ,0.927184 ,0.93358 ,0.939693 ,0.945519 ,0.951057 ,0.956305 ,0.961262 ,0.965926 ,0.970296 ,0.97437 ,0.978148 ,0.981627 ,0.984808 ,0.987688 ,0.990268 ,0.992546 ,0.994522 ,0.996195 ,0.997564 ,0.99863 ,0.999391 ,0.999848 ,1 };

double cos_table = { 1, 0.999848 ,0.999391 ,0.99863 ,0.997564 ,0.996195 ,0.994522 ,0.992546 ,0.990268 ,0.987688 ,0.984808 ,0.981627 ,0.978148 ,0.97437 ,0.970296 ,0.965926 ,0.961262 ,0.956305 ,0.951057 ,0.945519 ,0.939693 ,0.93358 ,0.927184 ,0.920505 ,0.913545 ,0.906308 ,0.898794 ,0.891007 ,0.882948 ,0.87462 ,0.866025 ,0.857167 ,0.848048 ,0.838671 ,0.829038 ,0.819152 ,0.809017 ,0.798636 ,0.788011 ,0.777146 ,0.766044 ,0.75471 ,0.743145 ,0.731354 ,0.71934 ,0.707107 ,0.694658 ,0.681998 ,0.669131 ,0.656059 ,0.642788 ,0.62932 ,0.615661 ,0.601815 ,0.587785 ,0.573576 ,0.559193 ,0.544639 ,0.529919 ,0.515038 ,0.5 ,0.48481 ,0.469472 ,0.45399 ,0.438371 ,0.422618 ,0.406737 ,0.390731 ,0.374607 ,0.358368 ,0.34202 ,0.325568 ,0.309017 ,0.292372 ,0.275637 ,0.258819 ,0.241922 ,0.224951 ,0.207912 ,0.190809 ,0.173648 ,0.156434 ,0.139173 ,0.121869 ,0.104528 ,0.087156 ,0.069756 ,0.052336 ,0.034899 ,0.017452 ,0 };

double tan_table = { 0, 0.017455 ,0.034921 ,0.052408 ,0.069927 ,0.087489 ,0.105104 ,0.122785 ,0.140541 ,0.158384 ,0.176327 ,0.19438 ,0.212557 ,0.230868 ,0.249328 ,0.267949 ,0.286745 ,0.305731 ,0.32492 ,0.344328 ,0.36397 ,0.383864 ,0.404026 ,0.424475 ,0.445229 ,0.466308 ,0.487733 ,0.509525 ,0.531709 ,0.554309 ,0.57735 ,0.600861 ,0.624869 ,0.649408 ,0.674509 ,0.700208 ,0.726543 ,0.753554 ,0.781286 ,0.809784 ,0.8391 ,0.869287 ,0.900404 ,0.932515 ,0.965689 ,1 ,1.03553 ,1.072369 ,1.110613 ,1.150368 ,1.191754 ,1.234897 ,1.279942 ,1.327045 ,1.376382 ,1.428148 ,1.482561 ,1.539865 ,1.600335 ,1.664279 ,1.732051 ,1.804048 ,1.880726 ,1.962611 ,2.050304 ,2.144507 ,2.246037 ,2.355852 ,2.475087 ,2.605089 ,2.747477 ,2.904211 ,3.077684 ,3.270853 ,3.487414 ,3.732051 ,4.010781 ,4.331476 ,4.70463 ,5.144554 ,5.671282 ,6.313752 ,7.11537 ,8.144346 ,9.514364 ,11.43005 ,14.30067 ,19.08114 ,28.63625 ,57.28996 , -1 };

double sin(double x){
    double ret = 0.0;
    const unsigned int BIG_NUM = 100;

    for (int n = 0; n < BIG_NUM; n++) {
        int k = n * 2 + 1; // odd number
        double pot = pow(x, k);
        double fat = fatorial(k);
        
        
        ret += (pow(-1, n)*pot)/fat;
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
    puts(ftoa(sin(rad(90.0)), "", 10));
    //puts(s);
    return 0;
}
