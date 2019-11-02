 /**************************************************************/
/* Engines                                                    */
/**************************************************************/

/* Sets both engines torque. The torque value must be between -100 and 100.
* Parameter: 
*   engine_1: Engine 1 torque
*   engine_2: Engine 2 torque
* Returns:
*   -1 in case one or more values are out of range
*    0 in case both values are in range
*/
set_torque: # (a0=int engine_1, a1=int engine_2)
    # begin push to sp
    addi sp, sp, -4
    sw  ra, 0(sp)
    addi sp, sp, -4
    sw  a1, 0(sp)
    # end push

    # setting engine 0
    mv a1, a0 # a1=int engine_1
    li a0, 0 # a0=int engine_id
    call set_engine_torque # (a0=int engine_id, a1=int torque)
    bnez a0, set_engine_error # if setting engine 0 error
    # end setting engine 0

    # setting engine 1
    # begin pop a1 from sp
    lw a1, 0(sp)
    addi sp, sp, 4
    # end pop
    li a0, 1 # a0=int engine_id
    call set_engine_torque # (a0=int engine_id, a1=int torque)
    beqz a0, set_engine_ok # if setting engine 1 error
    # end setting engine 1
    
    set_engine_error:
        li a0, -1
    
    set_engine_ok:
    # begin pop from sp
    lw ra, 0(sp)
    addi sp, sp, 4
    # end pop
    
    ret

/* 
* Sets engine torque. Engine ID 0/1 identifies the left/right engine.
* The torque value must be between -100 and 100.
* Parameter: 
*   engine_id: Engine ID
*   torque: Engine torque
* Returns:
*   -1 in case the torque value is invalid (out of range)
*   -2 in case the engine_id is invalid
*    0 in case both values are valid
*/
set_engine_torque:		# (a0 = int engine_id; a1 = int torque)
    li t0, -100
    li t1, 100
    blt a1, t0, invalid_torque		# if torque is less than -100, then it is a invalid value
    bgt a1, t1, invalid_torque		# if torque is greater than 100, then it is a invalid value
    li a7, 18
    ecall # code for set_engine_torque syscall is 18
    beqz a0, valid_parameters       # if we passed through the invalid_torque comparisons but didn't pass through "valid_parameters", the engine_id is invalid
    li a0, -2
    ret

    invalid_torque:
        li a0, -1
        ret

    valid_parameters:
        li a0, 0
        ret

/* Sets the angle of three Servo motors that control the robot head. 
*   Servo ID 0/1/2 identifies the Base/Mid/Top servos.
* Parameter: 
*   servo_id: Servo ID 
*   angle: Servo Angle 
* Returns:
*   -1 in case the servo id is invalid
*   -2 in case the servo angle is invalid
*    0 in case the servo id and the angle is valid
*/
set_head_servo:     # (a0 = int servo_id; a1 = int angle)
    li a7, 17
    ecall # code for set_head_servo syscall is 17
    li t0, -2
    beq a0, t0, servo_id_error      # if syscall returned -2 in a0, servoID is invalid
    li t0, -1
    beq a0, t0, servo_angle_error   # if syscall returned -1 in a0, servo angle is invalid
    ## if it passed through the previous "ifs", the syscall returned 0 in a0, both servoID and angle are valid
    mv a0, zero
    ret

    servo_id_error:
        li a0, -1
        ret
    
    servo_angle_error:
        li a0, -2
        ret


/**************************************************************/
/* Sensors                                                    */
/**************************************************************/
/* 
 * Reads distance from ultrasonic sensor.
 * Parameter: 
 *   none
 * Returns:
 *   distance of nearest object within the detection range, in centimeters.
 */
get_us_distance:        # the function has no parameters
    li a7, 16           # code for read_ultrasonic_sensor syscall is 16
    ecall
    ret
/* 
* Reads current global position using the GPS device.
* Parameter: 
*   pos: pointer to structure to be filled with the GPS coordinates.
* Returns:
*   void
*/
get_current_GPS_position: # (a0=Vector3* pos)
    li a7, 19 # syscall read_gps (19)
    ecall
    ret
/* 
* Reads global rotation from the gyroscope device .
* Parameter: 
*   pos: pointer to structure to be filled with the Euler angles indicated by the gyroscope.
* Returns:
*   void
*/
get_gyro_angles: # (a0=Vector3* pos)
    li a7, 20 # syscall read_gyroscope (20)
    ecall
    ret
/**************************************************************/
/* Timer                                                      */
/**************************************************************/

/* 
* Reads the system time. 
* Parameter:
*   * t: pointer to a variable that will receive the system time.
* Returns:
*   The system time (in milliseconds)
*/
get_time: #
    li a7, 21 # syscall get_time (21)
    ecall
    ret

/* 
* Sets the system time.
* Parameter: 
*   t: the new system time.
* Returns:
*   void
*/
set_time: # (a0=unsigned int t)
    li a7, 22 # syscall set_time (22)
    ecall
    ret

/**************************************************************/
/* UART                                                       */
/**************************************************************/
/*

* Writes a string to the UART. Uses the syscall write with file 
* descriptor 1 to instruct the SOUL to write the string to the UART.
* Parameter:
*   * s: pointer to the string.
* Returns:
*   void
*/
puts: # (a0=char* s)
    li t1, 0
    mv t0, a0
    puts_loop_1:
        lb t2, 0(t0)
        addi t0, t0, 1
        addi t1, t1, 1
        bnez t2, puts_loop_1

    mv a1, a0 #  buffer
    li a0, 1 # file descriptor = 1 (stdout)
    mv a2, t1 # size
    li a7, 64 # syscall write (64)
    ecall
    ret