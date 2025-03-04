add x5, x1, x2         
sub x6, x3, x4         
sll x7, x5, x2         
slt x8, x3, x4         
sltu x9, x5, x6        
xor x10, x7, x8        
srl x11, x6, x2        
sra x12, x5, x2        
or x13, x7, x8         
and x14, x9, x10       

addi x15, x1, 10       
slti x16, x2, 5        
sltiu x17, x3, 5       
xori x18, x4, 0xF      
ori x19, x5, 0xF       
andi x20, x6, 0xF      
slli x21, x1, 2        
srli x22, x2, 2        
srai x23, x3, 2        
    
lb x25, 10(x24)         
lh x26, 100(x24)         
lw x27, 10(x24)         
lbu x28, 3(x24)        
lhu x29, 2(x24)        

sb x25, 2(x24)         
sh x26, 1(x24)         
sw x27, 3(x24)         

beq x1, x2, label      
bne x1, x3, label      
blt x2, x3, label      
bge x3, x4, label      
bltu x4, x5, label     
bgeu x5, x6, label     

lui x30, 0x12345       
auipc x31, 0x12345     

jal x1, label          
jalr x2, x1, 4      

label:
