# RISC-V Assembler

## CS204 - Computer Architecture

### Project Phase 1 - Conversion of Assembly Code to Machine Code

**Group Members:**

- Shubham Aggarwal (2023CSB1162)
- Navneet Kaur (2023CSB1137)
- Yatri Sutariya (2023CSB1319)

---

## 📌 Project Overview

This project aims to build a **32-bit RISC-V assembler** that reads an assembly file (`input.asm`) and converts it into machine code (`output.mc`). This assembler will function similarly to the Venus assembler and will support a limited set of RISC-V instructions and assembler directives.

## 🔧 Features

- **Assembly to Machine Code Conversion**
- **Symbol Table Generation** for Labels
- **Support for RISC-V 32-bit Instructions** (31 instructions across different formats)
- **Support for Assembler Directives** (`.text`, `.data`, `.byte`, `.half`, `.word`, `.dword`, `.asciz`)
- **Segmentation**:
  - Code starts at `0x00000000`
  - Data starts at `0x10000000`
  - Heap at `0x10008000`
  - Stack at `0x7FFFFFFC`

## 📜 Input File Format (`input.asm`)

```
.data
.byte 1 2 3

.text
add x1, x2, x3
andi x5, x6, 10
...
```

## 🖥️ Output File Format (`output.mc`)

```
0x0 0x003100B3 , add x1,x2,x3 # 0110011-000-0000000-00001-00010-00011-NULL
0x4 0x00A37293 , andi x5,x6,10 # 0010011-111-NULL-00101-00110-000000001010
0x8 <termination code>
0x10000000 0x01
0x10000001 0x02
0x10000002 0x03
```

## 🏗️ Instruction Set Support

### ✅ Supported Instructions:

#### R-Format:

`add, and, or, sll, slt, sra, srl, sub, xor, mul, div, rem`

#### I-Format:

`addi, andi, ori, lb, ld, lh, lw, jalr`

#### S-Format:

`sb, sw, sd, sh`

#### SB-Format:

`beq, bne, bge, blt`

#### U-Format:

`auipc, lui`

#### UJ-Format:

`jal`

## 🛠️ How to Run the Assembler

### Compilation

```sh
g++ -o main main.cpp
```

### Running the Assembler

```sh
./main
```

## 📝 Implementation Details

### Pass 1: Symbol Table Construction

- Identify labels and their corresponding addresses.
- Allocate space for `.data` segment.
- Identify code and data segments and generate tokens accordingly.

### Pass 2: Instruction Encoding

- Convert instructions into **binary machine code**.
- Resolve label references.

### Contribution

- **Shubham Aggarwal (2023CSB1162)**: Project setup, input-output handling, Pass 2 (machine code to assembly).  
- **Navneet Kaur (2023CSB1137)**: Implemented data segment handling, and handling of assembly directives. (.data, .byte, etc.)
- **Yatri Sutariya (2023CSB1319)**: Pass 1 (label resolution, symbol table creation) and bug fixes in Pass 2.

---
