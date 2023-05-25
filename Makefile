all:
	gcc ship.c -D USER_INPUT             -D LINUX -o user_ship
	gcc ship.c -D PROGRAM_INPUT          -D LINUX -o program_ship
	gcc ship.c -D MONTE -D PROGRAM_INPUT -D LINUX -o monte_ship
	gcc test.c -D USER_INPUT             -D LINUX -o test_ship
clean:
	rm -f *~~
	rm -f user_ship program_ship monte_ship test_ship
