# SIC Assembler for Object Program Generation

This repository contains an assembler for the Simplified Instructional Computer (SIC), which translates assembly code into machine code and produces an object program.

## Directory Structure

- `sic/`: This directory contains assembly language programs written for the Simplified Instructional Computer (SIC). The `sicAssembly.c` file is a SIC assembler that reads assembly code and generates an object program.

## How to Run

To run the SIC assembler, execute the following steps:

1. Compile the `sicAssembly.c` file:

    ```bash
    gcc sic/sicAssembly.c -o sicAssembler
    ```

2. Run the assembler with the desired program file:

    ```bash
    ./sicAssembler <program_file>
    ```

    If no program file is provided, the assembler will default to using "Program1.asm."

3. The assembler will perform two passes: PASS1 and PASS2.

   - **PASS1**: Reads and processes the assembly code to generate intermediate files.
   - **PASS2**: Generates the object program using the intermediate files.

4. Optionally, you can use the `--clean` flag to remove intermediate files:

    ```bash
    ./sicAssembler <program_file> --clean
    ```

    This step is useful to clean up temporary files generated during the assembly process.

## Usage

```bash
./sicAssembler <program_file> [--clean]
```

If no `<program_file>` is provided, the assembler will use the default "Program1.asm."

## Example

```bash
./sicAssembler Program2.asm
```

This command will assemble "Program2.asm" and generate the corresponding object program. If the `--clean` flag is included, it will also remove intermediate files.

## Dependencies

This project requires a C compiler (e.g., gcc) to compile the `sicAssembly.c` file.