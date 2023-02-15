#Compilation сommand
CC=gcc -Wall -Wextra -o

#The name of the executable file
EXE_NAME=s21_grep

#The name of the file to compile
C_NAME=s21_grep.c

#Launch сommand
START=./$(EXE_NAME)

s21_grep:
	$(CC) $(EXE_NAME) $(C_NAME)
	# $(START) dab test.txt

rebuild:
	rm -rf $(EXE_NAME)
	$(CC) $(EXE_NAME) $(C_NAME)

clean:
	rm -rf $(EXE_NAME)
