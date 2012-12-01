all:
	./make.sh make

clean:
	./make.sh clean

test: all
	./indexer test_index test_index_dir
	./search -m 256MB test_index

.PHONY: all
