#This is a Makefile for get_printers

final:
	chmod 640 get_scanner_attributes_v2.c
	gcc -o get_scanner_attributes_v2 `cups-config --cflags` get_scanner_attributes_v2.c `cups-config --libs`
	./get_scanner_attributes_v2