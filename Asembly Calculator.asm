;partners: rudrapp2, gduncan2
;
;
; The Purpose of this program is to be a calculator for postfix expressions. I do this by storing the inputed values in a stack. The first value in will be the 
; last value out. For each arithmatic operation I will pop out the top value, store that value in R3, then move the pointer down, pop that value into R4 and move 
; the pointer down again. If there is an underflow error I will branch to an error message. Otherwise I will preform the operation and then PUSH that value back 
; into the stack and continue reading the characters as they are presented. If an equal sign is presented, I check to see if the stack has only one value (the 
; answer) then I loop to a print function. Otherwise, the error message will print again.
;
;
;
;
;
;
;

.ORIG x3000
	
	AND R5, R5, #0		; Initialize R5

READCH	
	ADD R5, R5, #0
	BRnp INVALID
	GETC			; Gets character INPUT (R0)
	OUT			; Prints to screen
	ST R0, SAVER0		; SAVES R0
	LD R1, NEGEQU		; Loads R1 with negative equals ASCII
	ADD R1, R0, R1		; ADDS Negative ASCII of EQUAL to R1
	BRz PRINT_HEX		; Branch to Evaluate if 0
	LD R1, NEGSPA		; Loads R1 with negative space ACII
	ADD R1, R0, R1		; ADDS Negative ACII of SPACE to R1
	BRz READCH		; If 0 branch back to read character
	LD R1, NEGZER		; Loads R1 with negative zero ASCII
	ADD R1, R1, R0		; ADDS Negavite ASCII of ZERO to R1
	BRn EVALUATE		; IF ASCII less than x30 then branch to evaluate
	LD R1, NEGNIN		; Loads R1 with negative NINE to R1
	ADD R1, R1, R0		; ADD Negative ASCII of NINE to R1
	BRp EVALUATE 		; If Positive branch to EVALUATE
	LD R0, SAVER0		; Loads saved R0 back into R0
	LD R1, NEGZER		; Loads Inverse of x30 into R1
	ADD R0, R0, R1		; Converts hex to decimal value
	JSR PUSH		; JUMPS to PUSH
	BRnzp READCH		; Branch back to READCHARACTER




;R3- value to print in hexadecimal
PRINT_HEX
	LD R3, STACK_START	; Loads Memory location of STACK_START into R3
	LD R4, STACK_TOP	; Loads memory location of STACK_TOP into R4
	NOT R3, R3		; Negates R3 
	ADD R3, R3, #2		; Negates R3
	ADD R3, R3, R4		; Adds R3 and R4 into R3
	BRnp INVALID		; If equal to 0 then branch to INVALID (Meaning there is 1 item in stack)
	JSR POP
	ADD R1, R0, #0		; Puts R0 into R1
	ADD R5, R0, #0		; Sets R5 to output
	ADD R3, R5, #0
PRINT_FROM_REGISTER
	AND R6, R6, #0 		; sets R6 to 0
	ADD R6, R6, #3 		; sets R6 to 3 (outer loop counter)
OUTER_LOOP
	AND R5, R5, #0 		; sets R5 to 0
	ADD R5, R5, #3 		; sets R5 to 3 (inner loop counter)
	AND R4, R4, #0 		; sets R4 to 0
INNER_LOOP
	ADD R4, R4, R4 		; left-shifts R4
	ADD R3, R3, #0 		; sets the CC based on R3
	BRzp SHIFT_R3		; if R3 is negative, then add 1 to R. Else, do not add 1 to R4
	ADD R4, R4, #1 		; add 1 to R4 if R3 is negative
SHIFT_R3
	ADD R3, R3, R3 		; left-shifts R3
	ADD R5, R5, #-1 	; decrements the inner loop counter
	BRzp INNER_LOOP 	; jumps back up to inner loop so that the inner loop repeats 4 times

	ADD R4, R4, #-9 	; checks whether R4 holds a digit (0-9) or a letter (A-F)
	BRnz ADD_48 		; if R4 is a digit, then we only need to add 48 to it to convert it to its ASCII character
	ADD R4, R4, #7 		; if R4 is a letter, then add an additional 7 to convert it to its ASCII character
ADD_48
	ADD R4, R4, #15 	; adding 48 to R4
	ADD R4, R4, #15 	; adding 48 to R4
	ADD R4, R4, #15		; adding 48 to R4
	ADD R4, R4, #3		; adding 48 to R4
	ADD R4, R4, #9 		; adding back that original 9 that we subtracted from R4
	ADD R0, R4, #0 		; sets R0 to the ASCII character that is stored in R4
	TRAP x21 		; prints out the ASCII character

	ADD R6, R6, #-1 	; decrements outer loop counter
	BRzp OUTER_LOOP 	; outer loop will run 4 times
	BRnzp DONE


;R0 - character input from keyboard
;R6 - current numerical output
;
;
EVALUATE
	JSR POP		; Pops value into R0
	ADD R5, R5, #0	; Sets CC to R5
	BRp INVALID	; Checks R5 for error
	ADD R4, R0, #0	; ADDS R0 into R4
	JSR POP		; Pops value into R0
	ADD R5, R5, #0	; Sets CC to R5
	BRp INVALID	; Checks R5 for error
	ADD R3, R0, #0 	; ADDS R0 into R3
	LD R0, SAVER0	; Loads saved value back into R0
	LD R1, NEGADD	; Loads Negated "+" into R1
	ADD R1, R1, R0	; Adds R1 and R0
	BRz PLUS	; Branch to PLUS
	LD R1, NEGMIN	; Loads Negated "+" into R1
	ADD R1, R1, R0	; Adds R1 and R0
	BRz MIN		; Branch to MIN
	LD R1, NEGMUL	; Loads Negated "+" into R1
	ADD R1, R1, R0	; Adds R1 and R0
	BRz MUL		; Branch to MUL
	LD R1, NEGDIV	; Loads Negated "+" into R1
	ADD R1, R1, R0	; Adds R1 and R0
	BRz DIV		; Branch to DIV
	LD R1, NEGPOW	; Loads Negated "+" into R1
	ADD R1, R1, R0	; Adds R1 and R0
	BRz EXP		; Branch to EXP
	BRnzp INVALID	; Branch to INVALID





;input R3, R4
;out R0
PLUS	

	ADD R0, R3, R4	; Adds values of R3 and R4 into R0
	JSR PUSH	; Pushes value to top of stack
	BRnzp READCH	; Loops back to read character

;input R3, R4
;out R0
MIN	

	NOT R4, R4	; Negates R4
	ADD R4, R4, #1	; Negates R4
	ADD R0, R3, R4	; Adds R3 and R4 into R0
	JSR PUSH	; Pushes new value to top of stack
	BRnzp READCH	; Loops back to read character
	
;input R3, R4
;out R0
MUL	
	AND R0, R0, #0	; Clears R0
	ADD R4, R4, #0	; Sets cc to R4
	BRn NEMULL	; Branches to Negative Multiply if Negative
MULLP	ADD R0, R0, R3	; Puts R3 into R0
	ADD R4, R4, #-1 ; Decrements R4
	BRp MULLP	; Branch to MULTIPLYLOOP is positive
	JSR PUSH	; Pushes value to top of stack
	BRnzp READCH	; Loops back to read character

NEMULL	ADD R0, R0, R3	; Puts R3 into R0
	ADD R4, R4, #1	; Increments R4
	BRn NEMULL	; Branch to Negative Multiply Loop is positive
	NOT R0, R0
	ADD R0, R0, #1
	JSR PUSH	; Else Push to top of stack
	BRnzp READCH	; Loops back to read Character

;input R3, R4
;out R0
DIV	
	AND R0, R0, #0	; Clears R0
	NOT R4, R4	; Negates R4
	ADD R4, R4, #1	; Negates R4
DIVLP	ADD R3, R3, R4	; Adds R3 and R4 into R3
	ADD R0, R0, #1	; Increments counter
	ADD R3, R3, #0 	; Sets CC to R3
	BRzp DIVLP	; Branch to Divide Loop
	ADD R0, R0, #-1	; Decrements Counter
	JSR PUSH	; Pushes new value to top of stack
	BRnzp READCH	; Loops back to read character


;input R3, R4
;out R0


EXP
	AND R0, R0, #0	; R0 gets 0
	ADD R1, R4, #-1	; Counter for how many times to multiply (4)
	ADD R2, R3, #0	; Multiply Counter (2)
	ADD R3, R3, #0	; For my brain  (2)
	ADD R4, R3, #0	; For my brain (2)

	ST R3, EXPSAVE	; Stores R3 value (STORES 2)

EXPLOOP	
	JSR MULEX
	ADD R3, R0, #0	; Updates R3
	LD R2, EXPSAVE	; R4 gets reloaded
	AND R0, R0, #0	; R0 gets 0
	ADD R1, R1, #-1	; Decrements R2
	BRp EXPLOOP	; Branch if positive
	ADD R0, R3, #0	; Sets R0 to R3
	JSR PUSH	; Push
	BRnzp READCH	; Loops back to read character




MULEX	ADD R0, R0, R3	; R0=R0+R3
	ADD R2, R2, #-1	; Decrements R4
	BRp MULEX	; Branch back to MULEX if positive
	RET		; Return




	
;IN:R0, OUT:R5 (0-success, 1-fail/overflow)
;R3: STACK_END R4: STACK_TOP
;
PUSH	
	ST R3, PUSH_SaveR3	;save R3
	ST R4, PUSH_SaveR4	;save R4
	AND R5, R5, #0		;
	LD R3, STACK_END	;
	LD R4, STACK_TOP	;
	ADD R3, R3, #-1		;
	NOT R3, R3		;
	ADD R3, R3, #1		;
	ADD R3, R3, R4		;
	BRz OVERFLOW		;stack is full
	STR R0, R4, #0		;no overflow, store value in the stack
	ADD R4, R4, #-1		;move top of the stack
	ST R4, STACK_TOP	;store top of stack pointer
	BRnzp DONE_PUSH		;
OVERFLOW
	ADD R5, R5, #1		;
DONE_PUSH
	LD R3, PUSH_SaveR3	;
	LD R4, PUSH_SaveR4	;
	RET


PUSH_SaveR3	.BLKW #1	;
PUSH_SaveR4	.BLKW #1	;


;OUT: R0, OUT R5 (0-success, 1-fail/underflow)
;R3 STACK_START R4 STACK_TOP
;
POP	
	ST R3, POP_SaveR3	;save R3
	ST R4, POP_SaveR4	;save R3
	AND R5, R5, #0		;clear R5
	LD R3, STACK_START	;
	LD R4, STACK_TOP	;
	NOT R3, R3		;
	ADD R3, R3, #1		;
	ADD R3, R3, R4		;
	BRz UNDERFLOW		;
	ADD R4, R4, #1		;
	LDR R0, R4, #0		;
	ST R4, STACK_TOP	;
	BRnzp DONE_POP		;
UNDERFLOW
	ADD R5, R5, #1		;
DONE_POP
	LD R3, POP_SaveR3	;
	LD R4, POP_SaveR4	;
	RET
INVALID
	LEA R0, INV		; Loads Invalid expression string into R0
	PUTS			; PRINTS TO SCREEN


DONE	
	ADD R5, R1, #0		; Puts answer into R5
	HALT


POP_SaveR3	.BLKW #1	;
POP_SaveR4	.BLKW #1	;
STACK_END	.FILL x3FF0	;
STACK_START	.FILL x4000	;
STACK_TOP	.FILL x4000	;

NEGADD		.FILL xFFD5	; Negative ASCII for '+'
NEGMIN		.FILL xFFD3	; Negative ASCII for '-'
NEGMUL		.FILL xFFD6	; Negative ASCII for '*'
NEGDIV		.FILL xFFD1 	; Negative ASCII for '/'
NEGPOW		.FILL xFFA2 	; Negative ASCII for '^'
NEGEQU		.FILL xFFC3	; Negative ASCII for '='
NEGSPA		.FILL xFFE0	; Negative ASCII for ' '
NEGZER		.FILL xFFD0	; Negative ASCII for '0'
NEGNIN		.FILL xFFC7	; Negative ASCII for '9'
INV		.STRINGZ "Invalid Expression"
SAVER0		.BLKW #1	; spot to save R0
SAVER4		.BLKW #1	; Spot to save R4
EXPSAVE		.BLKW #1	; Spot to Save R4 in EXP
R0SAVE		.BLKW #1	; Spot to save R0

.END
