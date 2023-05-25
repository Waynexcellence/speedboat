all:
	gcc ship.c -D USER_INPUT             -o user_ship
	gcc ship.c -D PROGRAM_INPUT          -o program_ship
	gcc ship.c -D MONTE -D PROGRAM_INPUT -o monte_ship
	gcc test.c -D USER_INPUT             -o test_ship
clean:
	rm -f *~~
	rm -f user_ship program_ship monte_ship test_ship
