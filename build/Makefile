all: test test2
INC=/usr/local/include/clp

FILES = $(INC)/Arg.hpp $(INC)/Switch.hpp \
	$(INC)/Option.hpp $(INC)/CommandLineParser.hpp


test: $(FILES)
	@echo "BUILDING ---> UNIT TEST 1"
	@g++ -o ../bin/test ../test/test.cpp
 
test2: $(FILES)
	@echo "BUILDING ---> UNIT TEST 2"
	@g++ -o ../bin/test2 ../test/test2.cpp 

clean:
	@echo "CLEANING ---> FILES"
	@rm -f ../include/*~
	@rm -f ../bin/test
	@rm -f ../bin/test2
	@rm -f ../test/*~
	
