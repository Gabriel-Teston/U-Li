#include "api_robot2.h"

int main(){
    char string [7]= "mc404\n";
    //set_torque(100,100);
    //set_head_servo(0, 90);
    //set_head_servo(1, 90);
    //set_head_servo(2, 0);
    int dist;
    while(0){
        dist = get_us_distance();
        if(dist > 0 && dist < 600){
            puts(string);
        }
    }
    Vector3 aux = {.x = 0, .y = 0, .z = 0};
    while(1){
        get_current_GPS_position(&aux);
        if(aux.x + 5000 <= 715 || aux.x - 5000 <= 715){
            puts(string);
        }
    }
    //puts(string);
    return 0;
}