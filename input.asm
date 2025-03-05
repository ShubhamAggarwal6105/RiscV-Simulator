.data
q: .word 1
n: .word 10
arr: .word 3, 10, 2, 100, 23, 10, 20, 30, 1, 1

.text
lui x20, 0x10000
addi x30, x20, 0x500 # x30 = 0x10000500 (Store address)
lw x10, 0, x20   # x10 = q (either 0 or 1)
addi x20, x20, 4
lw x11, 0, x20
addi x20, x20, 4  # x20 = 0x10000008 (Base address of array)

# Copy array into new location
addi x25, x20, 0
addi x26, x30, 0
addi x27, x0, 0
copy: bge x27, x11, exit_copy
lw x28, 0, x25
sw x28, 0, x26
addi x27, x27, 1
addi x25, x25, 4
addi x26, x26, 4
beq x0, x0, copy
exit_copy:

# Switch Case
beq x10, x0, unoptimized_bs
beq x0, x0, optimized_bs

unoptimized_bs:
addi x5, x0, 0  # i(x5) = 0
uwhile1: bge x5, x11, uexit1 # Exit1 if i >= n-1
    addi x6, x0, 0  # j(x6) = 0
    addi x25, x30, 0 # x25 = &arr[0]
    uwhile2: bge x6, x11, uexit2 #Exit2 if j >= n-1
        # Next addr (j+=1)
        addi x6, x6, 1
        addi x25, x25, 4
        lw x7, -4, x25 # x7 = arr[j-1]
        lw x8, 0, x25 # x8 = arr[j]
        bge x8, x7, uwhile2 # Skip if arr[j] >= arr[j-1]
        # SWAP
        sw x7, 0, x25
        sw x8, -4, x25
        beq x0, x0, uwhile2
    uexit2:
    addi x5, x5, 1
    beq x0, x0, uwhile1
uexit1:
beq x0, x0, exit

optimized_bs:
addi x5, x0, 0  # i(x5) = 0
owhile1: bge x5, x11, oexit1 # Exit1 if i >= n-1
    addi x6, x0, 0  # j(x6) = 0
    addi x25, x30, 0 # x25 = &arr[0]
    sub x12, x11, x5 # x12 = n-i-1
    addi x9, x0, 0   # Set x9 (flag) = 0
    owhile2: bge x6, x12, oexit2 #Exit2 if j >= n-i-1
        # Next addr (j+=1)
        addi x6, x6, 1
        addi x25, x25, 4
        lw x7, -4, x25 # x7 = arr[j-1]
        lw x8, 0, x25 # x8 = arr[j]
        bge x8, x7, owhile2 # Skip if arr[j] >= arr[j-1]
        # SWAP
        addi x9, x0, 1 # Set x9 (flag) to 1, if swapped
        sw x7, 0, x25
        sw x8, -4, x25
        beq x0, x0, owhile2
    oexit2:
    beq x9, x0, oexit1 # Exit if flag is still 0 (Array already sorted)
    addi x5, x5, 1
    beq x0, x0, owhile1
oexit1:
beq x0, x0, exit

exit:
