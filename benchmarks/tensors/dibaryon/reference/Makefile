## Set the Tiramisu path
TIRAMISU_PATH=../../../../

# Set to true if you want to run the reference when running Tiramisu code
USE_REFERENCE_WITH_TIRAMISU=0


########################################################################
## General settings
#CC=gcc
#CXX=g++
#CFLAGS=-O3 -mavx512f -m64 
CXXFLAGS=-O3 -std=c++11 -fno-rtti -m64 -mavx512f
CC=mpicc
CXX=mpicxx
CFLAGS=-O2 -m64 -std=c99  -Wno-unused-but-set-variable -I/opt/intel/compilers_and_libraries_2019.3.199/linux/compiler/include/intel64 # -lgfortran -lrt -lpthread -lm
#CFLAGS=-O2 -m64 -std=c99 -Wno-unused-but-set-variable -I/opt/intel/compilers_and_libraries_2019.3.199/linux/compiler/include/intel64 -fPIC

CORES=64
HL_NUM_THREADS=64

#CXXFLAGS=-O2 -std=c++11 -lgomp -fopenmp -lgfortran -fPIC -lrt -lpthread -lm -Wl,-rpath,/global/cscratch1/sd/mlwagman/qblocks/qlua_tiramisu/qlua/gsl/lib -Wl,-rpath,/global/cscratch1/sd/mlwagman/qblocks/qlua_tiramisu/qlua/arpack/lib -Wl,-rpath,L/opt/intel/compilers_and_libraries_2019.3.199/linux/compiler/lib/intel64  -Xlinker  -rpath -Xlinker 

## No need to modify the rest
LIB_DIR=-L${TIRAMISU_PATH}/3rdParty/Halide/bin/ -L${TIRAMISU_PATH}/3rdParty/Halide/lib/ -L/lib64/ 
INCLUDE_DIR=-I${TIRAMISU_PATH}/include/ -I${TIRAMISU_PATH}/3rdParty/Halide/include/ -I${TIRAMISU_PATH}/build/ -I${TIRAMISU_PATH}/benchmarks/ -I${TIRAMISU_PATH}/benchmarks/tensors/dibaryon/tiramisu_make_fused_dibaryon_blocks_correlator  -I${TIRAMISU_PATH}/benchmarks/tensors/dibaryon/tiramisu_make_fused_baryon_blocks_correlator
#TIRAMISU_GENERATED_OBJ=../tiramisu_make_local_single_double_block/generated_tiramisu_make_local_single_double_block_r1.o ../tiramisu_make_local_single_double_block/generated_tiramisu_make_local_single_double_block_r2.o  ../tiramisu_make_dibaryon_correlator/generated_tiramisu_make_dibaryon_correlator.o
TIRAMISU_GENERATED_OBJ=../tiramisu_make_fused_dibaryon_blocks_correlator/generated_tiramisu_make_fused_dibaryon_blocks_correlator.o ../tiramisu_make_fused_baryon_blocks_correlator/generated_tiramisu_make_fused_baryon_blocks_correlator.o



reference:
	$(CC) $(CFLAGS) qblocks_2pt.c qblocks_2pt_test.c -o qblocks_2pt_test

#-ltiramisu

#LIBRARIES=-ltiramisu -lHalide -lisl -lz -lpthread
#LIBRARIES=-ltiramisu -lHalide -lz -lpthread
LIBRARIES=-lHalide -lz -lpthread
INCLUDES="-I${TIRAMISU_PATH}/include/ -I${TIRAMISU_PATH}/3rdParty/Halide/include/"

## ${INCLUDES}

tiramisuwrapper:
	${CXX} ${CXXFLAGS} ${INCLUDE_DIR} -I${TIRAMISU_PATH}/benchmarks/ tiramisu_make_fused_dibaryon_blocks_correlator_wrapper.cpp  ${TIRAMISU_GENERATED_OBJ} ${LIB_DIR} ${LIBRARIES} -o tiramisu_make_fused_dibaryon_blocks_correlator_wrapper

tiramisu:
	$(CXX) $(CXXFLAGS) ${INCLUDE_DIR} -DUSE_REFERENCE=0 -c tiramisu_wrapper.cpp -o tiramisu_wrapper
	# $(CC) $(CFLAGS)  ${INCLUDE_DIR} -c -DUSE_REFERENCE=$(USE_REFERENCE_WITH_TIRAMISU) -DUSE_TIRAMISU=1 qblocks_2pt.c -o qblocks_2pt.o
	$(CC) $(CFLAGS) -c -DUSE_REFERENCE=$(USE_REFERENCE_WITH_TIRAMISU) -DUSE_TIRAMISU=1 qblocks_2pt_test.c -o qblocks_2pt_test.o
	$(CXX) $(CXXFLAGS) ${INCLUDE_DIR} ${LIB_DIR} tiramisu_wrapper qblocks_2pt_test.o ${TIRAMISU_GENERATED_OBJ} ${LIBRARIES} -o qblocks_2pt_test #qblocks_2pt.o 

run:
	HL_NUM_THREADS=${CORES} DYLD_LIBRARY_PATH=${DYLD_LIBRARY_PATH}:${TIRAMISU_PATH}/3rdParty/Halide/bin/:${TIRAMISU_PATH}/build/ LD_LIBRARY_PATH=${LD_LIBRARY_PATH}:${TIRAMISU_PATH}/3rdParty/Halide/bin/:${TIRAMISU_PATH}/3rdParty/isl/build/lib/:${TIRAMISU_PATH}/build/ ./qblocks_2pt_test
slowrun:
	DYLD_LIBRARY_PATH=${DYLD_LIBRARY_PATH}:${TIRAMISU_PATH}/3rdParty/Halide/bin/:${TIRAMISU_PATH}/build/ LD_LIBRARY_PATH=${LD_LIBRARY_PATH}:${TIRAMISU_PATH}/3rdParty/Halide/bin/:${TIRAMISU_PATH}/3rdParty/isl/build/lib/:${TIRAMISU_PATH}/build/ ./qblocks_2pt_test_slow
fastrun:
	DYLD_LIBRARY_PATH=${DYLD_LIBRARY_PATH}:${TIRAMISU_PATH}/3rdParty/Halide/bin/:${TIRAMISU_PATH}/build/ LD_LIBRARY_PATH=${LD_LIBRARY_PATH}:${TIRAMISU_PATH}/3rdParty/Halide/bin/:${TIRAMISU_PATH}/3rdParty/isl/build/lib/:${TIRAMISU_PATH}/build/ ./qblocks_2pt_test_fast

clean:
	rm -rf *.o qblocks_2pt_test tiramisu_wrapper tiramisu
