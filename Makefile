#This is a Makefile for get_printers

final:
	chmod 640 get_scanner_attributes_v2.c
	gcc -o get_scanner_attributes_v2 get_scanner_attributes_v2.c -lcups
	./get_scanner_attributes_v2