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
    csrr a1, mcause
    bgez a1, exception
    # Handle interruption
    andi a1, a1, 0x3f
    li a2, 7
    bne a1, a2, other_int
    # Machine timer interruption

        j restore_context
    # end Machine timer interruption
    other_int:
        j restore_context
    # end Handle interruption
    # end Select
    
    # Handle ecall
    exception:
        li a1, 16
        beq a1, a7, read_ultrasonic_sensor_ecall
        addi a1, 1
        beq a1, a7, set_servo_angles_ecall
        addi a1, 1
        beq a1, a7, set_engine_torque_ecall
        addi a1, 1
        beq a1, a7, read_gps_ecall
        addi a1, 1
        beq a1, a7, read_gyroscope_ecall
        addi a1, 1
        beq a1, a7, get_time_ecall
        addi a1, 1
        beq a1, a7, set_time_ecall
        addi a1, 42
        beq a1, a7, write_ecall
        j restore_context


        read_ultrasonic_sensor_ecall: # altera
        set_servo_angles_ecall: # altera
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
                j restore_context

            invalid_servoID:
                li a0, -1       # in case the servoID is invalid, we must return -1
                j restore_context

            invalid_angle:
                li a0, -2       # in case the angle is invalid, we must return -2
                j restore_context

        set_engine_torque_ecall: # altera
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
                j restore_context

            invalid_engineID:
                li a0, -2					# in case the engine_id is invalid, we must return -2
                j restore_context

            invalid_torque:
                li a0, -1					# in case the torque is invalid, we must return -1
                j restore_context
    
        read_gps_ecall: # nao altera
        read_gyroscope_ecall: # nao altera
        get_time_ecall: # altera
        set_time_ecall: # nao altera
        write_ecall: # altera
    # end Handle ecall

    # Restore context
    restore_context:
        csrr t0, mepc  # carrega endereÃ§o de retorno (endereÃ§o da instruÃ§Ã£o que invocou a syscall)
        addi t0, t0, 4 # soma 4 no endereÃ§o de retorno (para retornar apÃ³s a ecall) 
        csrw mepc, t0  # armazena endereÃ§o de retorno de volta no mepc

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
