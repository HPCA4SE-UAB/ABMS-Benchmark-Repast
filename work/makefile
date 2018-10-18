#************************************************************************************************************
#
# Repast HPC Tutorial Makefile
#
#************************************************************************************************************

include ./env

.PHONY: all
all : 


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
RepastHPC_Demo_03: clean_compiled_files
	$(MPICXX) $(REPAST_HPC_DEFINES) $(BOOST_INCLUDE) $(REPAST_HPC_INCLUDE) -I./include -c ./src/Main.cpp -o ./objects/Main.o
	$(MPICXX) $(REPAST_HPC_DEFINES) $(BOOST_INCLUDE) $(REPAST_HPC_INCLUDE) -I./include -c ./src/Model.cpp -o ./objects/Model.o
	$(MPICXX) $(REPAST_HPC_DEFINES) $(BOOST_INCLUDE) $(REPAST_HPC_INCLUDE) -I./include -c ./src/Agent.cpp -o ./objects/Agent.o
	$(MPICXX) $(BOOST_LIB_DIR) $(REPAST_HPC_LIB_DIR) -o ./bin/Model.exe  ./objects/Main.o ./objects/Model.o ./objects/Agent.o $(REPAST_HPC_LIB) $(BOOST_LIBS) -lfftw3



.PHONY: RepastHPC_Model
RepastHPC_Model : copy_RepastHPC_Model RepastHPC_Model_Compilation


.PHONY: copy_RepastHPC_Model
copy_RepastHPC_Model : clean
	cp -r ../RepastHPC/* ./


.PHONY: compare_RepastHPC_Model
compare_RepastHPC_Model:
	@echo "Comparing Agent.h:"
	-@diff ../RepastHPC/include/Agent.h ./include/Agent.h
	@echo "Comparing _Model.h:"
	-@diff ../RepastHPC/include/Model.h ./include/Model.h
	@echo "Comparing Agent.cpp:"
	-@diff ../RepastHPC/src/Agent.cpp ./src/Agent.cpp
	@echo "Comparing Model.cpp:"
	-@diff ../RepastHPC/src/Model.cpp ./src/Model.cpp
	@echo "Comparing main.cpp:"
	-@diff ../RepastHPC/src/main.cpp ./src/main.cpp




