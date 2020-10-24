# ipp_getScannerAttributes

  

Output :

    >$ make
    >chmod 640 get_scanner_attributes_v2.c
    >gcc -o get_scanner_attributes_v2 `cups-config --cflags` get_scanner_attributes_v2.c `cups-config --libs`
    >./get_scanner_attributes_v2
    ><---Printing Printers Attributes--->
    >attributes-charset => utf-8
    >attributes-natural-language => en-gb
    >status-message => The printer or class does not exist.