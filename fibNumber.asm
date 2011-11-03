# ###############################################
# Author: Nolan Hager
# Descripton: A program dealing with Fibonacci 
#	Numbers I did for a class
# ###############################################

		
		.data

prompt1:	.asciiz	"Enter a valid Fibonacci number: "
prompt2:	.asciiz "	No more Fibnoacci number"
tab:		.asciiz "	"
space:		.asciiz	" "
newline:	.asciiz "\n"
error_message:	.asciiz	" is not a valid Fibonacci number."

		.text
		.globl main

main:	li		$t3, 1			#to start the fib numbers
		li		$t2, 1			#to start the fib numbers
		li		$t5, 40			#checking to see if the number is in bound
		li		$t6, 5			#the counter for the number of outputs

Input:	li		$v0,4			#Getting the First Number
		la		$a0, prompt1
		syscall

		li		$v0, 5			#Keyboard input
		syscall
		addi		$t1,$v0,0		#copying the value to $t1


		blt		$t1,$t5,out_bounds

		jal		fib_number

		j end_of_program
	
out_bounds:	li		$v0,4
		la		$a0, error_message
		syscall
		la		$a0,newline
		syscall
		j		Input


end_of_program:	jr $31		#end of program

fib_number:	addi 		$sp,$sp,-16
		sw		$ra,0($sp)
		sw		$t2,4($sp)
		sw		$t3,8($sp)
		sw		$t4,12($sp)

		blt		$t3, $t1, recur
		beq		$t3, $t1, exitRecur
		j		greater

recur:		add		$t4,$t3,$t2
		addi		$t2,$t3,0
		addi		$t3,$t4,0
		jal		fib_number

		j		exitRecur

greater:	j		out_bounds	

display:	beq		$t6,$zero,exitRecur
		li		$v0,4			
		la		$a0, space
		syscall
		li		$v0,1
		add		$a0,$t4,$zero
		syscall
		addi		$t6,$t6,-1
		j		exitRecur
		
		
exitRecur:	lw		$ra,0($sp)
		lw		$t2,4($sp)
		lw		$t3,8($sp)
		lw		$t4,12($sp)
		addi		$sp,$sp,16
		jr		$ra


# end of program