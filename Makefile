CC = gcc
FILES = Makefile simpsh.c README test.sh LAB1CReport.pdf
default: simpsh.c
	$(CC) -o simpsh -g simpsh.c
dist: $(FILES)
	tar -czvf lab1-304925920.tar.gz $(FILES)
clean:
	rm -f lab1-304925920.tar.gz simpsh out.txt out2.txt test.txt comp.txt verb.txt
check: default test.sh
	./test.sh
