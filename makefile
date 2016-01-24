GPP=g++
OUTFILE="Bin/AntiVehicleSpawn.so"
COMPILE_FLAGS=-c -m32 -O3 -fpack-struct=1 -w -DLINUX -ISDK/amx/

all:
	$(GPP) $(COMPILE_FLAGS) SDK/*.cpp	
	$(GPP) $(COMPILE_FLAGS) *.cpp	
	$(GPP) -m32 -O2 -fshort-wchar -shared -o $(OUTFILE) *.o
clean:
	rm *.o