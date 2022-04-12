CFLAGS = -Wall

OBJECTS = xbox_test.o usbxbox.o

TARFILES = Makefile xbox_test.c \
	usbxbox.h usbxbox.c

xbox : $(OBJECTS)
	cc $(CFLAGS) -o xbox_test $(OBJECTS) -lusb-1.0 -pthread

xbox_test.o : xbox_test.c usbxbox.h
usbxbox.o : usbxbox.c usbxbox.h

.PHONY : clean
clean :
	rm -rf *.o 
