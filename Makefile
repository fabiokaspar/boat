OBJ=main.o utils.o ambiente.o render.o menu.o

LIBS=-lallegro -lallegro_primitives -lallegro_image -lallegro_ttf -lallegro_font -lm

boat: $(OBJ)
	gcc -g -o $@  $^ $(LIBS)

utils.o: utils.c utils.h render.h
	gcc -c -g $^

render.o: render.c render.h ambiente.h
	gcc -c -g $^

menu.o: menu.c menu.h utils.h
	gcc -c -g $^

ambiente.o: ambiente.c ambiente.h utils.h render.h
	gcc -c -g $^

main.o: main.c utils.h ambiente.h render.h
	gcc -c -g $^

clean: 
	rm -f $(OBJ) boat *.h.gch *.txt~  *.c~  *~
