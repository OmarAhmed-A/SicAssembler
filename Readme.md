# Simplified Instructional Computer (SIC) Assembler

This repository contains an assembler for the Simplified Instructional Computer (SIC), a hypothetical computer architecture. The assembler translates assembly code written for the SIC architecture into machine code, generating an object program that can be executed on the SIC machine.

## Project Contributors

Presented by:
- Anton Ashraf
- Omar Ahmed
- Salma Ismail
- Marwan Hazem

Under the supervision of:
Dr. Nsrin Ashraf

A big thank you to these amazing contributors for their dedication and hard work! 🚀✨

## Features

- **Assembler:** The main component of the repository is the SIC assembler, implemented in the `sicAssembly.c` file. It reads assembly code from input files (e.g., `Program1.asm`, `Program2.asm`) and produces the corresponding object program using the `OPTable.instr` to map assembly instructions to machine code.

## How to Use

1. **Compile:** Use a C compiler (e.g., gcc) to compile the `sicAssembly.c` file.

    ```bash
    gcc sic/sicAssembly.c -o sicAssembler
    ```

2. **Run:** Execute the compiled assembler, providing the desired program file.

    ```bash
    ./sicAssembler <program_file>
    ```

    If no program file is provided, the assembler defaults to using "Program1.asm."

3. **Optional Cleanup:** To remove intermediate files generated during assembly, use the `--clean` flag.

    ```bash
    ./sicAssembler <program_file> --clean
    ```

## Dependencies

- C compiler (e.g., gcc) for compiling the SIC assembler.
- Input assembly code files (e.g., `Program1.asm`, `Program2.asm`) written for the SIC architecture.

## Notes

- The SIC assembler processes assembly code and generates an object program suitable for execution on the Simplified Instructional Computer.
- Check specific program files and the `OPTable.instr` for details on supported instructions and their corresponding machine code.
- Explore the `sic/` directory for example SIC assembly programs and experiment with the assembler for a hands-on experience.

Feel free to delve into the repository, explore the source code, and experiment with SIC assembly programming!