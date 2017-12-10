all:
	gcc -o control control.c
	gcc -o client client.c

clean:
	rm control
	rm client
	rm story.txt
