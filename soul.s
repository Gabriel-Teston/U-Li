int_handler:
    # Save context
    csrrw a0, mscratch, a0	# troca valor de a0 com mscratch
	sw a1, 0(a0)
	sw a2, 4(a0)
	sw a3, 8(a0)
	sw a4, 12(a0)
	sw a5, 16(a0)
	sw a6, 20(a0)
	sw a7, 24(a0)
	sw s0, 28(a0)
	sw s1, 32(a0)
	sw s2, 36(a0)
	sw s3, 40(a0)
	sw s4, 44(a0)
	sw s5, 48(a0)
	sw s6, 52(a0)
	sw s7, 56(a0)
	sw s8, 60(a0)
	sw s9, 64(a0)
	sw s10, 68(a0)
	sw s11, 72(a0)
	sw t0, 76(a0)
	sw t1, 80(a0)
	sw t2, 84(a0)
	sw t3, 88(a0)
	sw t4, 92(a0)
	sw t5, 96(a0)
	sw t6, 100(a0)
	sw ra, 104(a0)
	sw sp, 108(a0)
	sw gp, 112(a0)
	sw tp, 116(a0)
    csrrw a0, mscratch, a0
    # end Save context

    # Select 
    csrr s0, mcause
    bgez s0, exception
    # Handle interruption
    andi s0, s0, 0x3f
    li a2, 11
    bne s0, a2, other_int
    # Machine timer interruption
        li s0, 0xFFFF0104
        sb zero, 0(s0)
        la s0, sys_time
        lw a2, 0(s0)
        addi a2, a2, 1
        sw a2, 0(a1)
        li s0, 0xFFFF0100
        li a2, 1
        sw a2, 0(s0)
        li a0, 1
        la a1, string
        li a2, 6
        j write_ecall
        j restore_context
    # end Machine timer interruption
    other_int:
        j restore_context
    # end Handle interruption
    # end Select
    
    # Handle ecall
    exception:
    
        csrr t0, mepc  # carrega endereÃ§o de retorno (endereÃ§o da instruÃ§Ã£o que invocou a syscall)
        addi t0, t0, 4 # soma 4 no endereÃ§o de retorno (para retornar apÃ³s a ecall) 
        csrw mepc, t0  # armazena endereÃ§o de retorno de volta no mepc

        li s0, 16
        beq s0, a7, read_ultrasonic_sensor_ecall
        addi s0, s0, 1
        beq s0, a7, set_servo_angles_ecall
        addi s0, s0, 1
        beq s0, a7, set_engine_torque_ecall
        addi s0, s0, 1
        beq s0, a7, read_gps_ecall
        addi s0, s0, 1
        beq s0, a7, read_gyroscope_ecall
        addi s0, s0, 1
        beq s0, a7, get_time_ecall
        addi s0, s0, 1
        beq s0, a7, set_time_ecall
        addi s0, s0, 42
        beq s0, a7, write_ecall
        j restore_context


        read_ultrasonic_sensor_ecall: # altera
            li t0, 0xFFFF0020
            sw zero, 0(t0)      # initialize process of reading ultrasonic sensor
            read_ultrasonic_sensor_ecall_while:
                lw t1, 0(t0)    # t1 <= value contained in 0xFFFF0020
                beqz t1, read_ultrasonic_sensor_ecall_while  # while t1 is equal to 0, continue reading; if it's not equal to 0, then it must be 1
            lw a0, 4(t0)        # a0 <= value contained in 0xFFFF0024
            j restore_context

        set_servo_angles_ecall: # (a0=servo_id, a1=angle) -> (servoID/angles validation)
            li t0, 1
            li t1, 2
            beqz a0, base
            beq a0, t0, mid
            beq a0, t1, top
            # in case the servoID isn't 0 nor 1 nor 2, then it's invalid.
            j invalid_servoID

            # once we have a valid servoID, we must check if the angle value is valid
            # limits for a0 = 0 => 16-116 (Base)
            # limits for a0 = 1 => 52-90 (Mid)
            # limits for a0 = 2 => 0-156 (Top)

            base:
                li t0, 16
                li t1, 116
                blt a1, t0, invalid_angle
                bgt a1, t1, invalid_angle
                li t0, 0xFFFF001E       # memory position associated with base servo motor
                sb a1, 0(t0)            # save the modified angle for the base servo motor
                j valid_parameters      # in case the angle parameter is in the range 16-116, then it is a valid angle for base
            
            mid:
                li t0, 52
                li t1, 90
                blt a1, t0, invalid_angle
                bgt a1, t1, invalid_angle
                li t0, 0xFFFF001D       # memory position associated with mid servo motor
                sb a1, 0(t0)            # save the modified angle for the mid servo motor
                j valid_parameters      # in case the angle parameter is in the range 52-90, then it is a valid angle for mid

            top:
                li t0, 0
                li t1, 156
                blt a1, t0, invalid_angle
                bgt a1, t1, invalid_angle
                li t0, 0xFFFF001C       # memory position associated with top servo motor
                sb a1, 0(t0)            # save the modified angle for the top servo motor
                j valid_parameters      # in case the angle parameter is in the range 0-156, then it is a valid angle for top

            valid_parameters:
                li a0, 0        # in case both parameters are valid, we must return 0
                j restore_context

            invalid_servoID:
                li a0, -2       # in case the servoID is invalid, we must return -2
                j restore_context

            invalid_angle:
                li a0, -1       # in case the angle is invalid, we must return -1
                j restore_context

        set_engine_torque_ecall: # (a0=engine_id, a1=torque) -> (engineID/angles validation)
            li t0, 1		# 1 is one of the possible Engine ID values; the other possible value is 0, which is in x0 register
            beqz a0, set_torque_motor1			# if engine_id equals to zero, branch to set_torque_motor1
            beq a0, t0, set_torque_motor2		# if engine_id equals to 1, branch to set_torque_motor2 
            j invalid_engineID				# else, branch to invalid_engineID

            set_torque_motor1:
                li t0, 0xFFFF001A
                sh a1, 0(t0)
                li a0, 0					# in case the engine_id is valid, we must return 0
                j restore_context

            set_torque_motor2:
                li t0, 0xFFFF0018
                sh a1, 0(t0)
                li a0, 0					# in case the engine_id is valid, we must return 0
                j restore_context

            invalid_engineID:
                li a0, -1					# in case the engine_id is invalid, we must return -1
                j restore_context
    
        read_gps_ecall: # nao altera
            li a1, 0xFFFF0004
            li a2, 0
            sw a2, 0(a1)
            read_gps_ecall_loop_1:
                lw a3, 0(a1)
                beq a2, a3, read_gps_ecall_loop_1
            li a1, 0xFFFF0008
            lw a2, 0(a1)
            sw a2, 0(a0)
            li a1, 0xFFFF000C
            lw a2, 0(a1)
            sw a2, 4(a0)
            li a1, 0xFFFF0010
            lw a2, 0(a1)
            sw a2, 8(a0)
            j restore_context

        read_gyroscope_ecall: # (a0=pointer to struct) -> ()
            li a1, 0xFFFF0004
            li a2, 0
            sw a2, 0(a1)
            read_gyroscope_ecall_loop_1:
                lw a3, 0(a1)
                beq a2, a3, read_gyroscope_ecall_loop_1
            li a1, 0xFFFF0014
            lw a2, 0(a1)
            sw a2, 0(a0)
            j restore_context

        get_time_ecall: # () -> (a0=sys_time)
            la a1, sys_time
            lw a0, 0(a1)
        set_time_ecall: # (a0=new sys_time) -> ()
            la a1, sys_time
            sw a0, 0(a1)
            j restore_context
        write_ecall: # (a0=file descriptor, a1=buffer addres, a2=buffer size) -> (a0=writen bytes)
            write_ecall_loop_1:
                li a4, 0xFFFF0109
                lbu a5, 0(a1)
                sb a5, 0(a4)
                li a4, 0xFFFF0108
                li a5, 1
                sb a5, 0(a4)
                write_ecall_loop_2:
                    lbu a6, 0(a4)
                    beq a5, a6, write_ecall_loop_2
                addi a1, a1, 1
                lbu a5, 0(a1)
                bne zero, a5, write_ecall_loop_1
            mv a0, a2
            j restore_context
    # end Handle ecall

    # Restore context
    restore_context:
        

        csrrw a0, mscratch, a0
        lw a1, 0(a0)
        lw a2, 4(a0)
        lw a3, 8(a0)
        lw a4, 12(a0)
        lw a5, 16(a0)
        lw a6, 20(a0)
        lw a7, 24(a0)
        lw s0, 28(a0)
        lw s1, 32(a0)
        lw s2, 36(a0)
        lw s3, 40(a0)
        lw s4, 44(a0)
        lw s5, 48(a0)
        lw s6, 52(a0)
        lw s7, 56(a0)
        lw s8, 60(a0)
        lw s9, 64(a0)
        lw s10, 68(a0)
        lw s11, 72(a0)
        lw t0, 76(a0)
        lw t1, 80(a0)
        lw t2, 84(a0)
        lw t3, 88(a0)
        lw t4, 92(a0)
        lw t5, 96(a0)
        lw t6, 100(a0)
        lw ra, 104(a0)
        lw sp, 108(a0)
        lw gp, 112(a0)
        lw tp, 116(a0)
        csrrw a0, mscratch, a0	# troca valor de a0 com mscratch
        mret

    # end Restore context
sys_time:
.skip 4
reg_buffer: .skip 248

        
.globl _start
_start:
    /**************************************************************/
    /* Hardware Config                                            */
    /**************************************************************/

    
    # Config GPT
    li t0, 0xFFFF0100
    li t1, 1
    sw t1, 0(t0)
    li t0, 0xFFFF0104
    sb zero, 0(t0)
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

    csrr t1, mie # Seta o bit 11 (MEIE)
    li t2, 0x800 # do registrador mie
    or t1, t1, t2
    csrw mie, t1
    # end Enable Interruptions

    # Config User stack
    li sp, 0x7fffffc
    # end Config User stack

    # Config Sys stack
    la t0, reg_buffer
    csrw mscratch, t0
    # end Config Sys stack

    # Change to user mode
    csrr t1, mstatus # Seta os bits 11 e 12 (MPP)
    li t2, ~0x1800 # do registrador mstatus
    and t1, t1, t2 # com o valor 00
    csrw mstatus, t1
    # end Change to user mode

    # Call LoCo
    #call main
    la t0, main # Grava o endereço do rótulo user
    csrw mepc, t0 # no registrador mepc
    mret
    # end Call LoCo
    
    main_loop:
        #la a1, sys_time
        #lw a2, 0(a1)
        #li a0, 1
        #la a1, sys_time
        #li a2, 4 
        #li a7, 64
        #ecall
        j main_loop
string: .ascii "mc404\n"