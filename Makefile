main: main.c
	gcc -o $@ $<

clean:
	-@rm ./main
