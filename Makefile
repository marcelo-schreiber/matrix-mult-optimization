PROJ_NAME=matmult
 
# .c files
C_SOURCE=$(wildcard *.c)
 
# .h files
H_SOURCE=$(wildcard *.h)
 
# Object files
OBJ=$(C_SOURCE:.c=.o)
 
# Compiler
CC=gcc
 
# Flags for compiler
CC_FLAGS=-c         \
				 -O3				\
				 -mavx			\
				 -march=native	\
         -Wall      \

 LIKWID_FLAGS=-DLIKWID_PERFMON \
 						 -I${LIKWID_INCLUDE} \
 						 -L${LIKWID_LIB} \


#
# Compilation and linking
#
all: $(PROJ_NAME)
 
$(PROJ_NAME): $(OBJ)
		$(CC) $(LIKWID_FLAGS) $^ -o $@ -lm -llikwid
 
%.o: %.c %.h
		$(CC) -o $@ $< $(CC_FLAGS) $(LIKWID_C) -lm -llikwid
 
matmult.o: matmult.c $(H_SOURCE)
		$(CC) $< $(CC_FLAGS) $(LIKWID_FLAGS) -o $@ -lm -llikwid
 
softclean:
		@rm -f *~ *.bak

clean:  softclean
		@rm -f $(PROG) *.o core a.out $(PROJ_NAME) output.out output2.out
		@rm -rf $(DISTDIR) $(DISTDIR).tar


		
.PHONY: all
