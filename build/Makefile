all: test
INC=/usr/local/include/clp

FILES = $(INC)/Arg.hpp $(INC)/Switch.hpp \
	$(INC)/Option.hpp $(INC)/CommandLineParser.hpp


test: $(FILES)
	@echo "BUILDING ---> UNIT TEST 1"
	@g++ -o ../bin/test ../test/test.cpp 

clean:
	@echo "CLEANING ---> FILES"
	@rm -f ../include/*~
	@rm -f ../bin/test
	@rm -f ../test/*~
	
