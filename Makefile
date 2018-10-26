OBJ=main.o utils.o ambiente.o render.o menu.o barco.o eventos.o

LIBS=-lallegro -lallegro_primitives -lallegro_image -lallegro_ttf -lallegro_font -lallegro_audio -lallegro_acodec  -lm 

boat: $(OBJ)
	gcc -g -o $@  $^ $(LIBS)

utils.o: utils.c utils.h
	gcc -c -g $^

render.o: render.c render.h ambiente.h utils.h barco.h eventos.h
	gcc -c -g $^

barco.o: barco.c barco.h render.h ambiente.h
	gcc -c -g $^

eventos.o: eventos.c eventos.h render.h barco.h
	gcc -c -g $^

menu.o: menu.c menu.h utils.h render.h eventos.h
	gcc -c -g $^

ambiente.o: ambiente.c ambiente.h utils.h eventos.h
	gcc -c -g $^

main.o: main.c utils.h ambiente.h render.h eventos.h barco.h menu.h
	gcc -c -g $^

clean: 
	rm -f $(OBJ) boat *.h.gch *.txt~  *.c~  *~
