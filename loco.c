#include "api_robot2.h"
/**************************************************************/
/* Solver of non-linear equations                             */
/**************************************************************/
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

float magnitude(Vector3* vec){
    int a = pow(vec->x, 2);
    int b = pow(vec->z, 2);
    return sqrt((int) (vec->x*vec->x + vec->z*vec->z));
}


float f1(float x, float z, float a, float b, float c) {
    return (a*x) + (b*z) + c;
}

float f2(float x, float z, float xo, float zo) {
    return (x-xo)*(x-xo) + (z-zo)*(z-zo) - 100;
}

float df1_x(a) {
    return a;
}

float df1_y() {
    return -1;
}

float df2_x(x,xo) {
    return 2*(x - xo);
}

float df2_y(z,zo) {
    return 2*(z - zo);
}

Vector3 newton_method(int x, int z, int xo, int zo, float a, int b, float c) {
    float J[4], Fx[2], det, invDet, inv_J[4], aux[2], s[2], k = 0;
    while (k < 30) {        // 10 iterações
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
        k++;
    }
    Vector3 res = {.x=(int)x, .y=105, .z=(int)z};

    puts(" Resultados Newton Method: \n");
    puts(itoa(res.x, "", 10));
    puts("\t");
    puts(itoa(res.z, "", 10));
    puts("\n");
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

char* ftoa(double n, char *res, int afterpoint) 
{ 
    int ipart;
    // Extract integer part
    if (n == 1 || n == -1){
        itoa(n, res, 10);
        return res;
    } else{
        ipart = (n >= 0) ? (int)(n - 0.5) : (int)(n + 0.5);
    }
    //(int)(n-0.5);
  
    // Extract floating part 
    double fpart = n - (double)ipart; 
  
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
Vector3 original_path[50] = {
  {.x = 728, .y = 105, .z = -56},
  {.x = 714, .y = 105, .z = -45},
  {.x = 695, .y = 105, .z = -35},
  {.x = 667, .y = 105, .z = -20},
  {.x = 626, .y = 105, .z = -30},
  {.x = 523, .y = 106, .z = -15},
  {.x = 471, .y = 105, .z = 34},
  {.x = 442, .y = 105, .z = 48},
  {.x = 435, .y = 105, .z = 98},
  {.x = 463, .y = 105, .z = 151},
  {.x = 494, .y = 106, .z = 190},
  {.x = 523, .y = 105, .z = 253},
  {.x = 557, .y = 105, .z = 332}
};

int is_in(int val, int* list, int size){
    for(int i = 0; i < size; i++){
        //puts(itoa(list[i], "", 10));
        //puts("+++\n");
        //puts(itoa(val, "", 10));
        if (list[i] == val){
            return 1;
        }
    }
    return 0;
}

void insert(Vector3** list, int index, Vector3* p, int size){
    for(int j = size; j > index; j--){
        *(list+(j*3)) = *(list+((j-1)*3));
        *(list+(j*3)+1) = *(list+((j-1)*3)+1);
        *(list+(j*3)+2) = *(list+((j-1)*3)+2);
    }
    *(list+(index*3)) = p->x;
    *(list+(index*3)+1) = p->y;
    *(list+(index*3)+2) = p->z;
}


void insert_used(int** list, int index, int* p, int size){
    for(int j = size; j > index; j--){
        *list[j] = *list[j-1];
    }
    *list[index] = *p;
}
int pow(int num, int pot){
    int p = 1;
    for(int i=0; i<pot; i++){
        p = p * num;
    }
    return p;
}

// Function to get absolute value of the number given by user.
float absolute(float num)
{
    if(num < 0){
        num = -num;
    }
    return num;
}

// Function to calculate square root of the number using Newton-Raphson method
float sqrt(int x)
{
    int cont = 0;
    float guess = x/2.0;
    while(cont < 30){
        guess = (x/guess + guess)/2.0;
        cont++;
    }
    return guess;
}

float dist(Vector3* v1, Vector3* v2){
    int a = pow(v1->x - v2->x, 2);
    int b = pow(v1->z - v2->z, 2);
    return sqrt(a + b);
}

float dist_to_line(Vector3* start, Vector3* end, Vector3* point){
    int a = (end->z-start->z)/(end->x-start->x);
    int b = start->z - (a*start->x);
    return absolute((a*point->x)-(point->z)+b)/sqrt(a*a+b*b);
}

void find_path(Vector3** original_path, Vector3* friends, Vector3* enemies, int n_originalPath, int n_friends, int n_enemies){
    /*Vector3 copy[n_originalPath];
    for(int loop = 0; loop < n_originalPath; loop++) {
        copy[loop] = original_path[loop];
    }*/
    int used[n_friends];
    for(int i = 0; i < n_friends; i++){
        used[i] = 0;
    }

    int min_dist;
    int closest;
    int this_dist;
    for(int friend = 0; friend < n_friends; friend++){
        min_dist = -1;
        closest = -1;
        for(int point = 0; point < n_originalPath; point++){
            this_dist = dist(&friends[friend], &(original_path[point*3]));
            //puts(ftoa(this_dist, "", 0));
            //puts("\n");
            if (min_dist == -1){
                min_dist = this_dist;
            }
            //puts(itoa(is_in(point, used, n_originalPath), "", 10));
            if (this_dist <= min_dist && !is_in(point, &used, n_friends)){
                closest = point;
                min_dist = this_dist;
            }
        }
        insert(original_path, closest+1, &friends[friend], n_originalPath);
        insert(original_path, closest+2, &(original_path[closest*3]), n_originalPath);
        
        used[friend] = closest;

    }
    for(int point = 0; point < n_originalPath-1; point++){
        for(int enemie = 0; enemie < n_enemies; enemie++){
            int start_x = original_path[point*3], start_z = original_path[(point*3)+2];
            int end_x = original_path[((point+1)*3)], end_z = original_path[((point+1)*3)+2];
            int point_x = enemies[enemie].x, point_z = enemies[enemie].z;
            float a = (end_z - start_z) / (end_x - start_x);
            float b = start_z - (a*start_x);
            float dtl = absolute((a*point_x)-(point_z)+b)/sqrt(a*a+1.0);
            puts("start_x: ");
            puts(ftoa(start_x, "", 3));
            puts(" ");
            puts("start_z: ");
            puts(ftoa(start_z, "", 3));
            puts("\n");
            puts("end_x: ");
            puts(ftoa(end_x, "", 3));
            puts(" ");
            puts("end_z: ");
            puts(ftoa(end_z, "", 3));
            puts("\n");
            
            puts("a: ");
            puts(ftoa(a, "", 3));
            puts("\n");
            puts("b: ");
            puts(ftoa(b, "", 3));
            puts("\n");
            if( dtl < 10.0){
                int d = sqrt((start_x-end_x)*(start_x-end_x)+(start_z-end_z)*(start_z-end_z));
                int d1 = dist(&(original_path[point*3]), &enemies[enemie]);
                int d2 = dist(&(original_path[(point+1)*3]), &enemies[enemie]);
                puts("Com distancia: \n");
                puts(itoa(dtl, "", 10));
                puts("\n");
                if( d1 < d &&  d2 < d ){
                    Vector3 aux = newton_method(start_x, start_z, point_x, point_z, a, -1, b);
                    Vector3 aux1 = newton_method(end_x, end_z, point_x, point_z, a, -1, b);
                    
                    puts("Intersectou entre: \n");
                    puts(itoa(start_x, "", 10));
                    puts(" ");
                    puts(itoa(start_z, "", 10));
                    puts("\n");
                    puts(itoa(end_x, "", 10));
                    puts(" ");
                    puts(itoa(end_z, "", 10));
                    puts("\n");
                    puts("Com distancia: \n");
                    puts(itoa(dtl, "", 10));
                    puts("\n");
                    puts("Bateu no inimigo: \n");
                    puts(itoa(point_x, "", 10));
                    puts(" ");
                    puts(itoa(point_z, "", 10));
                    puts("\n");
                    puts("Nos pontos:\n");
                    puts(itoa(aux.x, "", 10));
                    puts(" ");
                    puts(itoa(aux.z, "", 10));
                    puts("\n");
                    puts(itoa(aux1.x, "", 10));
                    puts(" ");
                    puts(itoa(aux1.z, "", 10));
                    puts("\n");
                    
                    
                    aux.x = aux.x  - point_x;
                    aux.y = 0;
                    aux.z=aux.z  - point_z;

                    aux1.x = aux1.x  - point_x;
                    aux1.y=0;
                    aux1.z = aux1.z  - point_z;
                    
                    Vector3 vec = {.x=aux.x+aux1.x, .y=0, .z=aux.z+aux1.z};
                    float mag = magnitude(&vec);

                    vec.x/=mag;
                    vec.z/=mag;

                    vec.x*=10;
                    vec.z*=10;

                    vec.x+=enemies[enemie].x;
                    vec.z+=enemies[enemie].z;

                    
                    puts("Gerando o ponto:\n");
                    puts(itoa(vec.x, "", 10));
                    puts(" ");
                    puts(itoa(vec.z, "", 10));
                    puts("\n");


                    insert(original_path, point+1, &vec, n_originalPath);

                    point--;
                }
            }
        }
    }
}


/**************************************************************/
/* Movement_controller                                        */
/**************************************************************/

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

float sin_table[] = { 0, 0.017452 ,0.034899 ,0.052336 ,0.069756 ,0.087156 ,0.104528 ,0.121869 ,0.139173 ,0.156434 ,0.173648 ,0.190809 ,0.207912 ,0.224951 ,0.241922 ,0.258819 ,0.275637 ,0.292372 ,0.309017 ,0.325568 ,0.34202 ,0.358368 ,0.374607 ,0.390731 ,0.406737 ,0.422618 ,0.438371 ,0.45399 ,0.469472 ,0.48481 ,0.5 ,0.515038 ,0.529919 ,0.544639 ,0.559193 ,0.573576 ,0.587785 ,0.601815 ,0.615661 ,0.62932 ,0.642788 ,0.656059 ,0.669131 ,0.681998 ,0.694658 ,0.707107 ,0.71934 ,0.731354 ,0.743145 ,0.75471 ,0.766044 ,0.777146 ,0.788011 ,0.798636 ,0.809017 ,0.819152 ,0.829038 ,0.838671 ,0.848048 ,0.857167 ,0.866025 ,0.87462 ,0.882948 ,0.891007 ,0.898794 ,0.906308 ,0.913545 ,0.920505 ,0.927184 ,0.93358 ,0.939693 ,0.945519 ,0.951057 ,0.956305 ,0.961262 ,0.965926 ,0.970296 ,0.97437 ,0.978148 ,0.981627 ,0.984808 ,0.987688 ,0.990268 ,0.992546 ,0.994522 ,0.996195 ,0.997564 ,0.99863 ,0.999391 ,0.999848 ,1 };

float cos_table[] = { 1, 0.999848 ,0.999391 ,0.99863 ,0.997564 ,0.996195 ,0.994522 ,0.992546 ,0.990268 ,0.987688 ,0.984808 ,0.981627 ,0.978148 ,0.97437 ,0.970296 ,0.965926 ,0.961262 ,0.956305 ,0.951057 ,0.945519 ,0.939693 ,0.93358 ,0.927184 ,0.920505 ,0.913545 ,0.906308 ,0.898794 ,0.891007 ,0.882948 ,0.87462 ,0.866025 ,0.857167 ,0.848048 ,0.838671 ,0.829038 ,0.819152 ,0.809017 ,0.798636 ,0.788011 ,0.777146 ,0.766044 ,0.75471 ,0.743145 ,0.731354 ,0.71934 ,0.707107 ,0.694658 ,0.681998 ,0.669131 ,0.656059 ,0.642788 ,0.62932 ,0.615661 ,0.601815 ,0.587785 ,0.573576 ,0.559193 ,0.544639 ,0.529919 ,0.515038 ,0.5 ,0.48481 ,0.469472 ,0.45399 ,0.438371 ,0.422618 ,0.406737 ,0.390731 ,0.374607 ,0.358368 ,0.34202 ,0.325568 ,0.309017 ,0.292372 ,0.275637 ,0.258819 ,0.241922 ,0.224951 ,0.207912 ,0.190809 ,0.173648 ,0.156434 ,0.139173 ,0.121869 ,0.104528 ,0.087156 ,0.069756 ,0.052336 ,0.034899 ,0.017452 ,0 };

float tan_table[] = { 0, 0.017455 ,0.034921 ,0.052408 ,0.069927 ,0.087489 ,0.105104 ,0.122785 ,0.140541 ,0.158384 ,0.176327 ,0.19438 ,0.212557 ,0.230868 ,0.249328 ,0.267949 ,0.286745 ,0.305731 ,0.32492 ,0.344328 ,0.36397 ,0.383864 ,0.404026 ,0.424475 ,0.445229 ,0.466308 ,0.487733 ,0.509525 ,0.531709 ,0.554309 ,0.57735 ,0.600861 ,0.624869 ,0.649408 ,0.674509 ,0.700208 ,0.726543 ,0.753554 ,0.781286 ,0.809784 ,0.8391 ,0.869287 ,0.900404 ,0.932515 ,0.965689 ,1 ,1.03553 ,1.072369 ,1.110613 ,1.150368 ,1.191754 ,1.234897 ,1.279942 ,1.327045 ,1.376382 ,1.428148 ,1.482561 ,1.539865 ,1.600335 ,1.664279 ,1.732051 ,1.804048 ,1.880726 ,1.962611 ,2.050304 ,2.144507 ,2.246037 ,2.355852 ,2.475087 ,2.605089 ,2.747477 ,2.904211 ,3.077684 ,3.270853 ,3.487414 ,3.732051 ,4.010781 ,4.331476 ,4.70463 ,5.144554 ,5.671282 ,6.313752 ,7.11537 ,8.144346 ,9.514364 ,11.43005 ,14.30067 ,19.08114 ,28.63625 ,57.28996 , -1 };

float sin(int x){
    while(x >= 360){
        x -= 360;
    }
    if(x >= 270){
        return -sin_table[360-(int)x];
    }else if(x >= 180){
        return -sin_table[(int)x-180];
    }else if (x >= 90){
        return sin_table[180-(int)x];
    }else{
        return sin_table[(int)x];
    }
}

float cos(int x){
    while(x >= 360){
        x -= 360;
    }
    if(x >= 270){
        return cos_table[360-(int)x];
    }else if(x >= 180){
        return -cos_table[(int)x-180];
    }else if (x >= 90){
        return -cos_table[180-(int)x];
    }else{
        return cos_table[(int)x];
    }
}

void print_pos(){
    char* string;
    Vector3 aux = {.x = 0, .y = 0, .z = 0};
    get_current_GPS_position(&aux);
    itoa(aux.x, string, 10);
    puts(string);
    puts("\t");
    itoa(aux.y, string, 10);
    puts(string);
    puts("\t");
    itoa(aux.z, string, 10);
    puts(string);
    puts("\n");
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

int get_y_angle(){
    Vector3 aux = {.x = 0, .y = 0, .z = 0};
    get_gyro_angles(&aux);
    return aux.y;
}

void rotate(int angle){
    //puts("Rotacionando ");
    //puts(itoa(angle, "", 10));
    //puts(" graus\n\0");
    int initial = get_y_angle();
    int self = get_y_angle();
    set_torque(25,-25);
    //print_orientation();
    while(self < (initial+angle)%360){
        self = get_y_angle();
    }
    set_torque(0, 0);
}

void go_to_pos(Vector3* pos){
    Vector3 uoli = {.x = 0, .y = 0, .z = 0};
    Vector3 aux_vec = {.x = 0, .y = 0, .z = 0};
    int y_angle;
    get_current_GPS_position(&uoli);
    int distance = dist(pos, &uoli);
    int moving = 0;
    while(distance >= 5){
        get_current_GPS_position(&uoli);
        y_angle = get_y_angle();
        distance = dist(pos, &uoli);
        aux_vec.x = distance*sin(y_angle)+uoli.x;
        aux_vec.z = distance*cos(y_angle)+uoli.z;
        if (dist(&aux_vec, pos) <= sqrt(distance)){
            if (moving == 0){
                set_torque(5, 5);
            }
        }else{
            if (moving){
                set_torque(0, 0);
            }
            rotate(1);
        }
    }
    set_torque(0, 0);
    //int offset_angle = ((int)(pos->y-aux.y)/(pos->x-aux.x));
}



/**************************************************************/
/* Main                                                       */
/**************************************************************/

int main(){
    /* while(1){
        puts(itoa(get_time(), "", 10));
        puts("\n");
    }*/
    /*Vector3 start = {.x=0, .y=0, .z=-10};
    Vector3 end = {.x=5, .y=0, .z=25};
    Vector3 enemie = {.x=10, .y=0, .z=0};


    int a = 7;
    int b = -10;
    Vector3 aux = newton_method(start.x, start.z, enemie.x, enemie.z, a, -1, b);
    Vector3 aux1 = newton_method(end.x, end.z, enemie.x, enemie.z, a, -1, b);

    puts("ponto 1\n");
    puts(itoa(aux.x, "", 10));
    puts("\t");
    puts(itoa(aux.z, "", 10));
    puts("\n");
    puts("pont 2\n");
    puts(itoa(aux1.x, "", 10));
    puts("\t");
    puts(itoa(aux1.z, "", 10));
    puts("\n");


    aux.x = aux.x  - enemie.x;
    aux.y=0;
    aux.z=aux.z  - enemie.z;

    aux1.x = aux1.x  - enemie.x;
    aux1.y=0;
    aux1.z=aux1.z  - enemie.z;

    Vector3 vec = {.x=aux.x+aux1.x, .y=0, .z=aux.z+aux1.z};
    float mag = sqrt((int)(vec.x*vec.x+vec.z*vec.z));
    //magnitude(&vec);

    puts(itoa((int)mag, "", 10));
    puts("\n");

    vec.x/=mag;
    vec.z/=mag;

    vec.x*=10;
    vec.z*=10;

    vec.x+=enemie.x;
    vec.z+=enemie.z;

    puts(itoa(vec.x, "", 10));
    puts("\t");
    puts(itoa(vec.z, "", 10));
    puts("\n");*/

    int n_originalPath = sizeof(original_path) / sizeof(original_path[0]);
    int n_friends = sizeof(friends_locations) / sizeof(friends_locations[0]);
    int n_enemies = sizeof(dangerous_locations) / sizeof(dangerous_locations[0]);

    find_path(&original_path, friends_locations, dangerous_locations, n_originalPath, n_friends, n_enemies);

    for(int loop = 0; loop < n_originalPath; loop++) {
        puts(itoa(original_path[loop].x, "", 10));
        puts(" ");
        puts(itoa(original_path[loop].z, "", 10));
        puts("\n");
    }

    for(int point = 0; point < n_originalPath; point++){
        go_to_pos(&original_path[point]);
    }
    //Vector3 friend = {.x = 715, .y = 0, .z = -40};
    //go_to_pos(&friend);
    //puts(s
    return 0;
}
