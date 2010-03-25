
CFLAGS = `allegro-config --cflags` -m32 -g
LDFLAGS = `allegro-config --libs` -m32

OBJS = dog2.o draw.o game.o grphload.o initend.o optnspge.o paint.o random.o titlepge.o

all: dog2

dog2: $(OBJS)

clean:
	$(RM) dog2 $(OBJS)
