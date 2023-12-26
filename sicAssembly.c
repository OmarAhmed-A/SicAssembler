#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>

int PASS1(const char *programFileName);
int PASS2();
void cleanFiles();

int main(int argc, char *argv[])
{
	const char *programFileName;

	if (argc >= 2)
	{
		programFileName = argv[1];
	}
	else
	{
		printf("\nUsage: %s <program_file> (Using Default \"Program1.asm\")\n", argv[0]);
		programFileName = "Program1.asm";
	}

	PASS1(programFileName);

	PASS2();

	if (argc == 3 && strcmp(argv[2], "--clean") == 0)
	{
		cleanFiles();
	}

	return 0;
}

int PASS1(const char *programFileName)
{
	char buffer[256], label[64], mnemonic[32], operand[64], mnem[32], op[64], symbol[64];
	int start = 0X0, locctr = 0X0, ret, flag = 0, address = 0X0, j, program_length = 0X0, count = 0X0;
	FILE *fProg, *fSymtab, *fOptab, *fIntrm, *fLength;

	fProg = fopen(programFileName, "r");
	if (fProg == NULL)
	{
		printf("Source file missing!!");
		return 1;
	}

	fSymtab = fopen("SYMTAB.txt", "w+");

	fOptab = fopen("OPTable.instr", "r");
	if (fOptab == NULL)
	{
		printf("OPTAB missing!!");
		return 1;
	}

	fIntrm = fopen("Intermediate_File.txt", "w");

	fgets(buffer, 256, fProg);
	sscanf(buffer, "%s %s %s", label, mnemonic, operand);

	if (strcmp(mnemonic, "START") == 0)
	{
		locctr = atoi(operand);
		while (locctr > 0)
		{
			start = start + (locctr % 10) * pow(16, count);
			locctr = locctr / 10;
			count++;
		}
		locctr = start;
		fprintf(fIntrm, "%X\t%s\t%s\t%s\n", start, label, mnemonic, operand);
	}
	else
	{
		locctr = 0X0;
	}

	while (!feof(fProg))
	{
		fgets(buffer, 256, fProg);
		ret = sscanf(buffer, "%s%s%s", label, mnemonic, operand);

		if (label[0] != ';' && label[0] != '.')
		{
			if (ret == 1)
			{
				strcpy(mnemonic, label);
				fprintf(fIntrm, "%04X\t\t%s\n", locctr, mnemonic);
			}
			if (ret == 2)
			{
				strcpy(operand, mnemonic);
				strcpy(mnemonic, label);
				fprintf(fIntrm, "%X\t\t%s\t%s\n", locctr, mnemonic, operand);
			}
			if (ret == 3)
			{
				rewind(fSymtab);
				while (!feof(fSymtab))
				{
					flag = 0;
					fscanf(fSymtab, "%s%X", symbol, &address);
					if (strcmp(label, symbol) == 0)
					{
						flag = 1;
						printf("\nDuplicate LABEL found: %s", label);
						return 1;
					}
				}

				if (flag == 0)
				{
					fprintf(fSymtab, "%s\t%X\n", label, locctr);
					fprintf(fIntrm, "%X\t%s\t%s\t%s\n", locctr, label, mnemonic, operand);
				}
			}

			rewind(fOptab);
			while (!feof(fOptab))
			{
				fscanf(fOptab, "%s%s", mnem, &op);
				if (strcmp(mnemonic, mnem) == 0)
				{
					locctr += 3;
					flag = 0;
					break;
				}
				else if (strcmp(mnemonic, "WORD") == 0 || strcmp(mnemonic, "word") == 0)
				{
					locctr += 3;
					flag = 0;
					break;
				}
				else if ((strcmp(mnemonic, "RESW") == 0) || (strcmp(mnemonic, "resw") == 0))
				{
					locctr += 3 * atoi(operand);
					flag = 0;
					break;
				}
				else if (strcmp(mnemonic, "RESB") == 0 || strcmp(mnemonic, "resb") == 0)
				{
					locctr += atoi(operand);
					flag = 0;
					break;
				}
				else if (strcmp(mnemonic, "BYTE") == 0 || strcmp(mnemonic, "byte") == 0)
				{
					j = strlen(operand);
					if (operand[0] != 'C' && operand[0] != 'X')
					{
						locctr += 1;
						flag = 0;
						break;
					}
					else if (strcmp(mnemonic, "BYTE") == 0 && operand[0] == 'C')
					{
						locctr += j - 3;
						flag = 0;
						break;
					}
					else if (strcmp(mnemonic, "BYTE") == 0 && operand[0] == 'X')
					{
						if ((j - 3) % 2 != 0)
							locctr += (j - 3) / 2 + 1;
						else
							locctr += (j - 3) / 2;
						flag = 0;
						break;
					}
					else
					{
						flag = 1;
					}
				}
				if (flag == 1)
				{
					printf("\n%s not present in OPTAB!", mnemonic);
					printf("\nExiting ...");
					return 1;
				}
			}
		}
		if (strcmp(mnemonic, "END") == 0)
		{
			break;
		}
	}
	printf("\nSYMTAB generated...\n");

	fLength = fopen("Program Length.txt", "w");
	program_length = locctr - start;
	fprintf(fLength, "%X", program_length);

	fclose(fProg);
	fclose(fSymtab);
	fclose(fOptab);
	fclose(fIntrm);
	fclose(fLength);
	return 0;
}

int PASS2()
{
	int locctr = 0X0, start = 0X0, sa = 0X0, program_length = 0X0, ret, op_status = 0, address = 0X0, target = 0X0, ascii = 0X0, temp1 = 0X0, j, k, count = 0X0, record_len = 0X0;
	char label[64], mnemonic[32], operand[64], buffer[256], mnem[32], op[2], symbol[64], opcode[64], cons[32];
	long int aseek, bseek;
	FILE *fIntrm, *fSymtab, *fOptab, *fLength, *fobj;

	fIntrm = fopen("Intermediate_File.txt", "r");
	if (fIntrm == NULL)
	{
		printf("\nIntermediate file missing!");
		return 1;
	}
	fSymtab = fopen("SYMTAB.txt", "r");
	if (fSymtab == NULL)
	{
		printf("\nSYMTAB missing!");
		return 1;
	}
	fOptab = fopen("OPTable.instr", "r");
	if (fOptab == NULL)
	{
		printf("\nOPTAB missing!");
		return 1;
	}
	fLength = fopen("Program Length.txt", "r");
	if (fLength == NULL)
	{
		printf("\nProgram_length file missing!");
		return 1;
	}
	fobj = fopen("Object_Program.txt", "w");

	fscanf(fIntrm, "%X%s%s%s", &locctr, label, mnemonic, operand);
	if (strcmp(mnemonic, "START") == 0)
	{
		start = (int)strtol(operand, NULL, 16);
		fscanf(fLength, "%X", &program_length);
		fprintf(fobj, "H^%6s^%06X^%06X", label, start, program_length);
		fprintf(fobj, "\nT^%06X^00^", start);
		bseek = ftell(fobj);
	}

	fgets(buffer, 256, fIntrm);
	while (!feof(fIntrm))
	{
		fgets(buffer, 256, fIntrm);
		ret = sscanf(buffer, "%X%s%s%s", &locctr, label, mnemonic, operand);

		if (ret == 2)
		{
			strcpy(mnemonic, label);
		}
		else if (ret == 3)
		{
			strcpy(operand, mnemonic);
			strcpy(mnemonic, label);
		}
		else
		{
		}

		if (count >= 0X3C || strcmp(mnemonic, "RESB") == 0 || strcmp(mnemonic, "RESW") == 0 || strcmp(mnemonic, "END") == 0)
		{
			aseek = ftell(fobj);
			fseek(fobj, -(aseek - bseek) - 3L, 1);
			record_len = count / 0X2;
			fprintf(fobj, "%02X^", record_len);
			fseek(fobj, 0L, 2);
			if (strcmp(mnemonic, "END") == 0)
			{
				break;
			}
			sa = locctr;
			if (strcmp(mnemonic, "RESW") != 0)
			{
				fprintf(fobj, "\nT^%06X^00^", sa);
			}
			bseek = ftell(fobj);
			count = 0X0;
		}

		rewind(fOptab);
		op_status = 0;
		while (!feof(fOptab))
		{
			fscanf(fOptab, "%s%s", mnem, op);
			if (strcmp(mnemonic, mnem) == 0)
			{
				strcpy(opcode, op);
				op_status = 1;
				break;
			}
		}
		j = strlen(operand);
		if (op_status == 1 && operand[j - 1] == 'X' && operand[j - 2] == ',')
		{
			// printf("CondnforBUFFER,X.");

			operand[j - 2] = '\0';
			rewind(fSymtab);
			fscanf(fSymtab, "%s%X", symbol, &address);
			while (!feof(fSymtab))
			{
				fscanf(fSymtab, "%s%X", symbol, &address);
				if (strcmp(operand, symbol) == 0)
				{
					target = address;
					target += 0X8000;
					break;
				}
			}
			fprintf(fobj, "%2s%04X^", opcode, target);
			count = count + 0X6;
			continue;
		}

		else if (op_status == 1 && strcmp(mnemonic, "RSUB") != 0)
		{
			rewind(fSymtab);
			while (!feof(fSymtab))
			{
				fscanf(fSymtab, "%s%X", symbol, &address);
				if (strcmp(operand, symbol) == 0)
				{
					target = address;
					break;
				}
			}
			printf("\nopcode=%s\ttarget=%X\n", opcode, target);
			fprintf(fobj, "%2s%04X^", op, target);
			count = count + 0X6;
			continue;
		}
		else if (op_status == 1 && strcmp(mnemonic, "RSUB") == 0)
		{
			fprintf(fobj, "%s0000^", opcode);
			count = count + 0X6;
			continue;
		}
		else
		{
			if ((strcmp(mnemonic, "BYTE") == 0) || ((strcmp(mnemonic, "BYTE") == 0)))
			{
				if (operand[0] == 'C')
				{
					for (k = 0; k < strlen(operand) - 3; k++)
					{
						temp1 = 0x0;
						temp1 = temp1 + (int)operand[k + 2];
						ascii = ascii * 0x100 + temp1;
					}
					fprintf(fobj, "%6X^", ascii);
					count = count + strlen(operand) - 0X3;
				}
				else
				{
					for (k = 0; k < strlen(operand) - 3; k++)
					{
						cons[k] = operand[k + 2];
					}
					cons[k] = '\0';
					fprintf(fobj, "%s^", cons);
					count = count + (strlen(cons) + 0X0);
				}
				continue;
			}
			else if ((strcmp(mnemonic, "WORD") == 0) || (strcmp(mnemonic, "word") == 0))
			{
				temp1 = (int)strtol(operand, NULL, 10);
				fprintf(fobj, "%06X^", temp1);
				count = count + 0X6;
				continue;
			}
			else
			{
				continue;
			}
		}
	}
	fprintf(fobj, "\nE^%06X", start);
	printf("\nObject Program written successfully!\n");
	fclose(fIntrm);
	fclose(fSymtab);
	fclose(fOptab);
	fclose(fLength);
	fclose(fobj);
	return 0;
}

void cleanFiles()
{
	remove("SYMTAB.txt");
	remove("Intermediate_File.txt");
	remove("Program Length.txt");
	remove("Object_Program.txt");
}
