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