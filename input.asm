## f(n) = f(n-1) + f(n-2)

.data
.word 7

.text
lui x20, 0x10000
lw x10, 0, x20    #x10 = n

#Initalizations
addi x5, x0, 2    #x5 = 2

# Answer in x11
jal x1, fib
jal x0, full_exit

fib:
    # Store return location, n and x13 (partial answer fib(n-1)) for this
    addi sp,sp,-12
    sw x10,4(sp)
    sw x1,0(sp)
    
    # if n > 2: recursion
    blt x5, x10, recur
    
    # else return 1
    addi sp,sp,12
    addi x11,x0,1
    jalr x0, x1, 0
    
recur:
    addi x10, x10, -1 # n-1
    jal x1, fib
    
    sw x11, 8(sp) #store f(n-1)
    
    # Restore
    lw x10, 4(sp)
    lw x1, 0(sp)
    
    addi x10, x10, -2
    jal x1, fib
    
    # x11 = f(n-2)
    
    # Restore
    lw x13, 8(sp)  # x13 = f(n-1)
    lw x10, 4(sp)
    lw x1, 0(sp)
    
    add x11, x11, x13   # x11 = f(n-2) + f(n-1)
    addi sp,sp,12
    jalr x0, x1, 0

full_exit: