HDF5BIN=/usr/local/hdf5/bin/
HDFC=h5avg

all: $(HDFC).out

$(HDFC).out: $(HDFC).c
	$(HDF5BIN)h5cc -o $(HDFC).out $(HDFC).c
	rm $(HDFC).o

clean:
	rm $(HDFC).out	