CC= gcc
CFLAGS= -Wall -g -pedantic
EXEC= run
SRC=$(wildcard *.c)
OBJ=$(SRC:.c=.o)
HDRS=-I include
GP=`which gnuplot`
DB=1 #Verbose mode basically 

all : $(EXEC)
	@echo Compiling the executable...
	@if test -f $(EXEC); then echo Success!; else echo Something went wrong...; fi

$(EXEC) : $(OBJ)
	@echo Compiling Object files from Sources...
	@$(CC) $(CFLAGS)  -o $@ $^ -lm
	@mkdir -p ./Data/PlotConfig ./Data/PlotData ./Data/Plots
	@echo Done!

$(OBJ) : $(SRC)
	@echo Compiling Source files...
	@$(CC) -DGNUPLOT='"$(GP)"' -DDB=$(DB) -c $^ $(HDRS) 
	@echo Done!

info :	
	@echo "Sources: $(SRC)"
	@echo "Object : $(OBJ)"
	@echo "Exec   : $(EXEC)"

clean :
	@echo Removing:
	@rm -rvf $(EXEC) $(OBJ) ./Data/PlotConfig ./Data/PlotData ./Data/Plots ./Data/Times/*
	@echo Done.

.PHONY : clean all info install
