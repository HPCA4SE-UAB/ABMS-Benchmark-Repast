#************************************************************************************************************
#
# Makefile targets
#
#************************************************************************************************************

include ./make.configure

default:all

.PHONY: clean_output_files
clean_output_files:
	rm -f *.csv
	rm -f *.txt
	rm -f ./output/*.csv
	rm -f ./output/*.txt
	rm -f ./logs/*.*

.PHONY: clean_compiled_files
clean_compiled_files:
	rm -f *.exe
	rm -f ./bin/*.exe
	rm -f *.o
	rm -f ./object/*.o

.PHONY: remove_subdirectories
remove_subdirectories:
	rm -fr objects
	rm -fr output
	rm -fr bin
	rm -fr include
	rm -fr src
	rm -fr props
	mkdir objects
	mkdir output
	mkdir bin

	
.PHONY: clean
clean: clean_compiled_files clean_output_files remove_subdirectories
	rm -f *.cpp
	rm -f ./src/*.cpp
	rm -f *.props
	rm -f ./props/*.props
	rm -f ./include/*.h


.PHONY: RepastHPC_Model_Compilation
RepastHPC_Model_Compilation: clean_compiled_files
	$(MPICXX) $(REPAST_HPC_DEFINES) $(CXXFLAGS) -I./include -c ./src/Main.cpp -o ./objects/Main.o
	$(MPICXX) $(REPAST_HPC_DEFINES) $(CXXFLAGS) -I./include -c ./src/Model.cpp -o ./objects/Model.o
	$(MPICXX) $(REPAST_HPC_DEFINES) $(CXXFLAGS) -I./include -c ./src/Agent.cpp -o ./objects/Agent.o
	$(MPICXX) $(LDFLAGS) -o ./bin/Model.exe  ./objects/Main.o ./objects/Model.o ./objects/Agent.o $(REPAST_LIB) $(BOOST_LIBS) $(FFTW3_LIB)



.PHONY: all
all : copy_RepastHPC_Model RepastHPC_Model_Compilation


.PHONY: copy_RepastHPC_Model
copy_RepastHPC_Model : clean
	cp -r ../RepastHPC/* ./

