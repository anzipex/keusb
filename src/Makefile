
#
# Makefile -- сценарий автоматической сборки проекта
#
# Разработал: Муковников М. Ю.
# Версия:     1.0.0
# Изменён:    16.04.2010
#

CC     = gcc
CFLAGS = -Wall -O2 -fomit-frame-pointer
BINS   = /usr/bin
OBJS1  = driver.o device.o
OBJS2  = daemon.o config.o
SRCS   = device.c driver.c daemon.c config.c keusb.c keusbd.c

VPATH  = posix

all: keusb keusbd

keusb: keusb.o ${OBJS1}
	${CC} ${CFLAGS} -o $@ keusb.o ${OBJS1}

keusbd: keusbd.o ${OBJS2}
	${CC} ${CFLAGS} -o $@ keusbd.o ${OBJS2}

.c.o:
	${CC} ${CFLAGS} -I ${VPATH} -c $<

clean:
	rm -f *~ *.o keusb keusbd

install:
	cp -f keusb keusbd ${BINS}
	cp -f conf.d/keusb.example /etc/conf.d
	cp -f init.d/keusbd /etc/init.d

deinstall:
	rm -f /etc/init.d/keusbd
	rm -f ${BINS}/keusb
	rm -f ${BINS}/keusbd

