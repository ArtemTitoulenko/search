all:
	./make.sh make

clean:
	./make.sh clean

test: all
	./indexer test_index test_index_dir
	./search test_index

.PHONY: all
