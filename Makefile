all:
	gcc ship.c -D USER_INPUT -o user_ship
	gcc ship.c -D PROGRAM_INPUT -o program_ship
clean:
	rm -f *~~
	rm -f user_ship program_ship
