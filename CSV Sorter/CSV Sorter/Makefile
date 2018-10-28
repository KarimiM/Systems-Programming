all: sorter mergesort simpleCSVsorter

sorter:
	gcc sorter.c mergesort.c simpleCSVsorter.c -o sorter.out 2> errorlog

mergesort: 
	gcc -c mergesort.c -o mergesort.out

simpleCSVsorter:
	gcc simpleCSVsorter.c -o simpleCSVsorter.out

clean:
	rm -rf sorter.out mergesort.out simpleCSVsorter.out
