.SUFFIXES : .x .o .c .s

#ROOT = /usr/local/arm_linux_4.2
INC = /home/dhivakar/tmp/tmp/include 

LIB = /home/dhivakar/tmp/tmp/lib
CC = arm-linux-gcc -O2 -I$(INC) -static
WEC_LDFLAGS = -L$(LIB)
STRIP = arm-linux-strip

TARGET = server
SRCS = cmd.c server.c xml_parser.c
#LIBS = -lgcc
LIBS = -lxml -lm -lz

nu:
	$(CC) $(WEC_LDFLAGS) $(SRCS) -o $(TARGET) $(LIBS) -static
	$(STRIP) $(TARGET)
pc:
	gcc $(SRCS) -I/usr/include/libxml2 -lxml2 -o test
xml:
	gcc xml_test.c xml_parser.c -I/usr/include/libxml2 -lxml2 -o test_xml
clean:
	rm -f *.o
	rm -f *.x
	rm -f *.flat
	rm -f *.map
	rm -f temp
	rm -f *.img
	rm -f $(TARGET)
	rm -f *.gdb
	rm -f *.bak
	rm -f test
