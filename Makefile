
OBJ=main.o pixel.o queue.o utils.o random.o ambiente.o desenho.o config.o 

boat: $(OBJ)
	gcc -o boat $^  -lallegro -lallegro_primitives -lallegro_image -lallegro_ttf -lallegro_font -lm


pixel.o: pixel.c pixel.h
	gcc -c $^ -g

queue.o: queue.c queue.h utils.h
	gcc -c $^ -g

utils.o: utils.c utils.h
	gcc -c $^ -g

desenho.o: desenho.c desenho.h ambiente.h pixel.h utils.h -lallegro_image -lallegro -lallegro_primitives 
	gcc -c desenho.c -g

random.o: random.c random.h
	gcc -c $^ -g

ambiente.o: ambiente.c ambiente.h config.h random.h pixel.h queue.h utils.h -lm
	gcc -c $^ -g

main.o: ambiente.h config.h pixel.h queue.h utils.h desenho.h main.c
	gcc -c $^ -g

config.o: config.c config.h
	gcc -c $^ -g


clean: 
	rm -f $(OBJ) boat *.h.gch *.txt~  *.c~  *~
