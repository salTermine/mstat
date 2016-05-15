/* DESCRIPTION: 
		AUTHOR: SALVATORE TERMINE
		  DATE:	05.13.16
	   VERSION:	2.0 
*/

#include "hw1.h"

int col_hdr = 0;
char *name = NULL;
char *regNames[] = {"$zero", "$at", "$v0", "$v1", "$a0", "$a1", "$a2", "$a3", 
					  "$t0", "$t1", "$t2", "$t3", "$t4", "$t5", "$t6", "$t7", 
					  "$s0", "$s1", "$s2", "$s3", "$s4", "$s5", "$s6", "$s7", 
					  "$t8", "$t9", "$k0", "$k1", "$gp", "$sp", "$fp", "$ra"};
char line[11];
unsigned int hex = 0;
unsigned int opcode = 0;
char *invalid;

int main(int argc, char *argv[])
{
	
	int opt;
	name = argv[0];

	/* CHECK NUMBER OF ARGS */
	if(argc <= 1 || argc >= 4)
	{
		USAGE(argv[0]);
		exit(EXIT_FAILURE);
	}
	/* CHECK IF ONLY -u FLAG WAS PASSED */
	if((strcmp(argv[1], "-u") == 0) && argv[2] == NULL)
	{
		USAGE(argv[0]);
		exit(EXIT_FAILURE);
	}

	/* PARSE CMDLINE FLAGS */
	while((opt = getopt(argc, argv, "hirou")) != -1)
	{
		switch(opt)
		{
			/* HELP */
			case 'h':
				USAGE(argv[0]);
				exit(EXIT_SUCCESS);
				break;
			/* INSTRUCTIONS */
			case 'i':
				if(argv[2] != NULL) col_hdr = 1;
				instr_func();
				exit(EXIT_SUCCESS);
				break;
			/* REGISTERS */
			case 'r':
				if(argv[2] != NULL) col_hdr = 1;
				registr_func();
				exit(EXIT_SUCCESS);
				break;
			/* OPCODES */
			case 'o':
				if(argv[2] != NULL) col_hdr = 1;
				opcode_func();
				exit(EXIT_SUCCESS);
				break;
			/* BAD FLAG */
			default:
				USAGE(argv[0]);
				exit(EXIT_FAILURE);
				break;
		}/* END SWITCH */
	}/* END WHILE */
}/* END MAIN */

/* ------------------------------------------------------------
   INSTRUCTION FUNCTION 
   PRODUCES SOME SIMPLE STATISTICS IN THE FOLOWING FORMAT.
   EACH LINE WILL CONTAIN THE INSTRUCTION TYPE, THE COUNT
   OF THIS INSTRUCTION TYPE GIVEN IN THE INPUT, AND THE 
   PERCENTAGE OF THIS INSTRUCTION TYPE (BASED ON THE TOTAL
   NUMBER OF INSTRUCTIONS PROVIDED IN THE INPUT). UPON
   SUCCESSFUL COMPLETION THE PROGRAM SHOULD RETURN EXIT_SUCCESS. 
   -------------------------------------------------------------*/
int instr_func(void)
{
	int iType = 0;
	int rType = 0;
	int jType = 0;
	int total_line = 0;
	
	while(scanf("%s", line) != -1) 
	{
		if((strlen(line) < 11))
		{
			hex = strtoull(line, &invalid, 16);
			if(*invalid == '\0')
			{
				hex = hex >> 26;
				if(hex == 0)
				{
					rType++;
					total_line++;
				}
				else if((hex == 2) || (hex == 3))
				{
					jType++;
					total_line++;
				}
				else
				{
					iType++;
					total_line++;
				}
			}
			else
			{
				USAGE(name);
				fprintf(stderr, "Invalid hex value. %x\n", hex);
				exit(EXIT_FAILURE);
			}
		}
		else
		{
			USAGE(name);
			fprintf(stderr, "Invalid hex value. %x\n", hex);
			exit(EXIT_FAILURE);
		}
	}
	if(col_hdr == 1) printf("TYPE\tCOUNT\tPERCENT\n");
    printf("I-Type\t%d\t%f%%\n", iType, (float) iType / total_line * 100);
    printf("J-Type\t%d\t%f%%\n", jType, (float) jType / total_line * 100);
    printf("R-Type\t%d\t%f%%\n", rType, (float) rType / total_line * 100);
    return EXIT_SUCCESS;
}

/* --------------------------------------------------------------------------------
   REGISTER FUNCTION
   PRODUCES A LIST OF THE STATISTICS OF EACH REGISTER. THERE IS A COLUMN FOR
   HOW MANY TIMES IT WAS USED IN TOTAL, HOW MANY R-TYPE INSTRUCTIONS USED IT,
   HOW MANY I-TYPE INSTRUCTIONS USED IT, HOW MANY J-TYPE INSTRUCTIONS USED IT,
   AND THE PERCENTAGE OF TIMES THE REGISTER WAS BEING USED BASED ON ALL THE OTHER
   REGISTERS. UPON SUCCESSFUL COMPLETION THE PROGRAM SHOULD RETURN EXIT_SUCCESSFUL.
   --------------------------------------------------------------------------------*/

int registr_func(void)
{
	unsigned int rs = 0;
	unsigned int rt = 0;
	unsigned int rd = 0;
	int regCountR[32];
	int regCountI[32];
	int regCountJ[32];
	int totals = 0;
	int instruction_total = 0;

	// INITIALIZE REGCOUNTS TO 0
    for (int i = 0; i < 32; i++) {
        regCountI[i] = 0;
        regCountR[i] = 0;
        regCountJ[i] = 0;
    }

	while(scanf("%s", line) != -1) {
        if(strlen(line) < 11)
        {
        	hex = strtoull(line, &invalid, 16);
        	if(*invalid == '\0')
        	{
        		opcode = hex >> 26;
        		/* R-TYPE */
        		if (opcode == 0) 
        		{
                    rs = hex << 6;
                    rs = rs >> 27;
                    rt = hex << 11;
                    rt = rt >> 27;
                    rd = hex << 16;
                    rd = rd >> 27;
                    regCountR[rs]++;
                    regCountR[rd]++;
                    regCountR[rt]++;
                    totals += 3;
                    instruction_total++;
                }
                /* I-TYPE */
                else if (opcode != 0 && opcode != 2 && opcode != 3) 
                {
                    rs = hex << 6;
                    rs = rs >> 27;
                    rt = hex << 11;
                    rt = rt >> 27;
                    regCountI[rs]++;
                    regCountI[rt]++;
                    totals += 2;
                    instruction_total++;
                }
                /* J-TYPE */
                else 
                {
                    continue;
                }
        	}
        	/* BAD INPUT FROM FILE */
        	else
			{
				USAGE(name);
				fprintf(stderr, "Invalid hex value.%x\n", hex);
				exit(EXIT_FAILURE);
			}
        }
        /* BAD INPUT FROM FILE */
        else
		{
			USAGE(name);
			fprintf(stderr, "Invalid hex value.%x\n", hex);
			exit(EXIT_FAILURE);
		}
    }
    /* IF THE -u FLAG WAS GIVEN THEN PRINT HEADERS */
    if(col_hdr == 1) printf("REG\tUSE\tR-TYPE\tI-TYPE\tJ-TYPE\tPERCENT\n");
    /* PRINT REGISTER INFORMATION */
	for (int i = 0; i < 32; i++) {
	    printf("%s\t%d\t%d\t%d\t%d\t%f%%\n", regNames[i], (regCountR[i] 
	    	+ regCountI[i] + regCountJ[i]), regCountR[i], regCountI[i], 
	    	  regCountJ[i], ((float) (regCountR[i] + regCountI[i] 
	    	+ regCountJ[i]) / instruction_total)*100);
	}
	return EXIT_SUCCESS;
}

/*  ----------------------------------------------------------------------------------
	OPCODE FUNCTION
	LIST OUT THE STATISTICS OF EACH OPCODE USED. THERE ARE 127 POSSIBLE OPCODES
	(2^(6+1) - 1). TRACK THE NUMBER OF TIMES AN OPCODE IS USED AND THE PERCENTAGE
	USED IN THE ENTIRE PROGRAM. FOR THE INSTRUCTIONS WITH OPCODE 0x00, ALSO KNOWN
	AS R-TYPE INSTRUCTIONS, YOU SHOULD KEEP TRACK OF THE FUNCTION FIELD AND ITS USAGE
	PERCENTAGE WITH RESPECT TO THE TOTAL NUMBER OF INSTRUCTIONS WITH OPCODE 0x00. IF
	DONE CORRECTLY THE PERCENTAGE OF INSTRUCTIONS WITH TOTAL NUMBER OF INSTRUCTIONS
	WITH OPCODE 0x00 SHOULD EQUAL THE PERCENTAGE OF R-TYPE INSTRUCTIONS YOU SEE WHEN
	YOUR RUN YOUR PROGRAM WITH -i. NOTE THE SINGLE LINE BREAK BETWEEN OPCODE AND 
	FUNCTION INFORMATION.
    ---------------------------------------------------------------------------------*/
int opcode_func(void)
{
	unsigned int function = 0;
	unsigned int mask = 0x0000003F;
	unsigned int func = mask & function;
	int function_totals = 0;
	int totals = 0;
	int opCodes[64];
	int functions[64];
	
	/* INITIALIZE OPCODES[] AND FUNCTIONS[] TO 0 */
	for (int i = 0; i < 64; i++) 
	{
        opCodes[i] = 0;
        functions[i] = 0;
    }

	while(scanf("%s", line) != -1)
	{
		if(strlen(line) < 11)
		{
			hex = strtoull(line, &invalid, 16);
	        opcode = hex >> 26;
	        function = hex;
	        mask = 0x0000003F;
	        func = mask & function;
	        opCodes[opcode]++;

	       	if (opcode == 0)
	       	{
	       		functions[func]++;
	       		function_totals++;
	       	}
	        totals++;
	    }
	    /* BAD INPUT FROM FILE */
        else
		{
			USAGE(name);
			fprintf(stderr, "Invalid hex value.%x\n", hex);
			exit(EXIT_FAILURE);
		}
    }
	/* PRINT COLUMN HEADERS IF -u FLAG WAS GIVEN */
	if(col_hdr == 1) printf("OPCODE\tCOUNT\tPERCENTAGE\n");
	/* PRINT OPCODE STATS */
	for (int i = 0; i < 64; i++) {
		printf("0x%x\t%d\t%.1f%%\n", i, opCodes[i], ((float) opCodes[i] / totals) * 100);
    }
    /* PRINT COLUMN HEADERS IF -u FLAG WAS GIVEN */
	if(col_hdr == 1) {
		printf("\nFUNC\tCOUNT\tPERCENTAGE\n");
	}
	/* PRINT FUNCTION STATS */
	for (int i = 0; i < 64; i++) {
		printf("0x%x\t%d\t%.1f%%\n", i, functions[i],((float)functions[i]/function_totals) * 100);
    }
    return EXIT_SUCCESS;
}