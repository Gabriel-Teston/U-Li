int_handler:
    # Select interruption type
    
    # end Select interruption type
    
    # Handle
    set_servo_angles_ecall:
        li t0, 1
        li t1, 2
        beq a0, t0, valid_servoID
        beq a0, t1, valid_servoID
        beqz a0, valid_servoID
        # in case the servoID isn't 0 nor 1 nor 2, then it's invalid.
        j invalid_servoID

        valid_servoID:      # once we have a valid servoID, we must check if the angle value is valid
            # limits for a0 = 0 => 16-116 (Base)
            # limits for a0 = 1 => 52-90 (Mid)
            # limits for a0 = 2 => 0-156 (Top)
            li t0, 0
            li t1, 1
            li t2, 2
            beq a0, t0, base
            beq a0, t1, mid
            beq a0, t2, top
            j invalid_servoID

            base:
                li t0, 16
                li t1, 116
                blt a1, t0, invalid_angle
                bgt a1, t1, invalid_angle
                j valid_parameters      # in case the angle parameter is in the range 16-116, then it is a valid angle for base
            
            mid:
                li t0, 52
                li t1, 90
                blt a1, t0, invalid_angle
                bgt a1, t1, invalid_angle
                j valid_parameters      # in case the angle parameter is in the range 52-90, then it is a valid angle for mid

            top:
                li t0, 0
                li t1, 156
                blt a1, t0, invalid_angle
                bgt a1, t1, invalid_angle
                j valid_parameters      # in case the angle parameter is in the range 0-156, then it is a valid angle for top

        valid_parameters:
            li a0, 0
            ret

        invalid_servoID:
            li a0, -1       # in case the servoID is invalid, we must return -1
            ret

        invalid_angle:
            li a0, -2       # in case the angle is invalid, we must return -2
            ret

    set_engine_torque_ecall:
        li t0, 1		# 1 is one of the possible Engine ID values; the other possible value is 0, which is in x0 register
        beq a0, t0, valid_engineID		# if engine_id equals to 1, branch to valid_engineID 
        beqz a0, valid_engineID			# if engine_id equals to zero, branch to valid_engineID
        j invalid_engineID				# else, branch to invalid_engineID

        valid_engineID:		# once we have a valid engine_id, we must check if the torque value is valid
            li t0, -100
            li t1, 100
            blt a1, t0, invalid_torque		# if torque is less than -100, then it is a invalid value
            bgt a1, t1, invalid_torque		# if torque is greater than 100, then it is a invalid value
            li a0, 0						# else, we have a valid torque and a valid engineID. then, we must return 0
            ret

        invalid_engineID:
            li a0, -2					# in case the engine_id is invalid, we must return -2
            ret

        invalid_torque:
            li a0, -1					# in case the torque is invalid, we must return -1
            ret
    # end Handle
        
.globl _start
_start:
    /**************************************************************/
    /* Hardware Config                                            */
    /**************************************************************/

    # Config GPT
    li t0, 0xFFFF0100
    li t1, 1
    sw t1, 0(t0)
    # end Config GPT

    # Config Engine 0 Torque
    li t0, 0xFFFF001A
    sw zero, 0(t0)
    # end Config Engine 0 Torque

    # Config Engine 1 Torque
    li t0, 0xFFFF0018
    sw zero, 0(t0)
    # end Config Engine 1 Torque

    # Config Servo Base
    li t0, 0xFFFF001E
    li t1, 31
    sb t1, 0(t0)
    # end Config Servo Base

    # Config Servo Mid
    li t0, 0xFFFF001D
    li t1, 80
    sb t1, 0(t0)
    # end Config Servo Mid

    # Config Servo Top
    li t0, 0xFFFF001C
    li t1, 78
    sb t1, 0(t0)
    # end Config Servo Top

    # Config Interruption Handler
    la t0, int_handler
    csrw mtvec, t0
    # end Config Interruption Handler
    

    /**************************************************************/
    /* Enable LoCo control                                        */
    /**************************************************************/

    # Enable Interruptions
    csrr t0, mstatus
    ori t0, t0, 0x80
    csrw mstatus, t0
    # end Enable Interruptions

    # Config User stack
    li sp, 0x7fffffc
    # end Config User stack

    # Config Sys stack
    reg_buffer: .skip 124
    la t0, reg_buffer
    csrw mscratch, t0
    # end Config Sys stack

    # Call LoCo
    call main
    # end Call LoCo
