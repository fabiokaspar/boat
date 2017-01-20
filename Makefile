OBJ=main.o utils.o

boat: $(OBJ)
	gcc -o boat $^ -lallegro -lallegro_primitives -lallegro_image -lallegro_ttf -lallegro_font -lm

#queue.o: queue.c queue.h utils.h
#	gcc -c $^ -g

utils.o: utils.c utils.h
	gcc -c $^ -g


#ambiente.o: ambiente.c ambiente.h queue.h utils.h -lm
#	gcc -c $^ -g

main.o: utils.h main.c
	gcc -c $^ -g

clean: 
	rm -f $(OBJ) boat *.h.gch *.txt~  *.c~  *~
