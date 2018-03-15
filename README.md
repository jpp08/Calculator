# Calculator with Binary Tree

## Implementation

	I had to use the polish notation for the project.
	Example: 
		(5 − 6) × 7  => × − 5 6 7
		5 − (6 × 7)  => − 5 × 6 7

## Usage
In shell,
	
	make

	./pc
	or 
	printf '+ 1 2\n- * 3 4 5\n' | ./pc
	or
	echo '- * + 1 2 3 4' | ./pc