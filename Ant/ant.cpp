#include "ant.hpp"

inline void set_cursor_pos_ansi(int x, int y) {
	printf("\033[%d;%dH", x, y);
}

void get_input(int argc, char** argv, int& nAnts, int& width, int& height) {
	if (argc == 4) {
		try {
			nAnts = std::stoi(argv[2]);
			width = std::stoi(argv[3]);
			height = std::stoi(argv[4]);
		}
		catch (const std::exception& e) {
			fmt::print("Provide 3 arguments: (1) number of ants, (2) field width, (3) field height;\n");
			exit(EXIT_FAILURE);
		}
	}
	else {
		int success = 0;
		printf("How many ants?\n >> ");
		success = scanf("%lu", &nAnts);
		printf("width\n >> ");
		success = scanf("%lu", &width);
		printf("height\n >> ");
		success = scanf("%lu", &height);
	}
	set_cursor_pos_ansi(0, 0);
	printf("\033[J");
}

int main(int argc, char** argv) {
	using namespace ant;
	int nAnts = 0, 
		width = 0,
		height = 0;
	get_input(argc, argv, nAnts, width, height);
	
	if (nAnts <= 0 || width <= 0 || height <= 0) {
		fmt::print("All arguments must be positive values!\n");
		exit(EXIT_FAILURE);
	}
	Game g(width, height, nAnts);
	for (;;) {
		set_cursor_pos_ansi(0, 0);
		g.take_turn();
		g.print();
	}
}
