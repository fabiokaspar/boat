OBJ=main.o utils.o ambiente.o render.o

CC_ALLEGRO=-lallegro -lallegro_primitives -lallegro_image -lallegro_ttf -lallegro_font
#CC_OPT=-std=c99

boat: $(OBJ)
	gcc -g -o $@  $^  $(CC_ALLEGRO) -lm

utils.o: utils.c utils.h
	gcc -c -g $^

render.o: render.c render.h 
	gcc -c -g $^

ambiente.o: ambiente.c ambiente.h utils.h -lm
	gcc -c -g $^

main.o: main.c utils.h ambiente.h render.h -lm
	gcc -c -g $^

clean: 
	rm -f $(OBJ) boat *.h.gch *.txt~  *.c~  *~
