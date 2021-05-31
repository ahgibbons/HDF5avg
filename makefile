HDF5BIN=/usr/local/hdf5/bin/
HDFC=h5avg
TESTH5=test.h5
TESTRES=test_result.txt
TDSET=myset
TESTOUT=testout.h5

all: $(HDFC).out

$(HDFC).out: $(HDFC).c
	$(HDF5BIN)h5cc -o $(HDFC).out $(HDFC).c
	rm $(HDFC).o

clean:
	rm $(HDFC).out	

test: $(HDFC).out
	./$(HDFC).out $(TESTH5) $(TDSET) $(TESTOUT)
	h5dump $(TESTOUT)
	cat $(TESTRES)
	echo "Test results should match the h5dump results"

cleantest:
	rm $(TESTOUT)