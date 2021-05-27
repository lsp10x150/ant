#pragma once
#include <fmt/core.h>
#include <array>
#include <chrono>
#include <map>
#include <functional>
#include <random>
#include <fmt/color.h>

namespace ant {

	std::random_device rd;
	std::default_random_engine e(rd());

	enum class CellColor { White, Black };
	enum class Direction { West, East, North, South };

	struct Coordinates {
		int x, y;
	};

	class Board {
	public:
		Board(uint32_t x, uint32_t y)
			: width(x), height(y)
		{
			board = new CellColor * [height];
			for (int i = 0; i < height; i++) {
				board[i] = new CellColor[width];
				for (int j = 0; j < width; j++) {
					board[i][j] = CellColor::Black;
				}
			}
		}
		void chage_color(Coordinates c) {
			board[c.y][c.x] = board[c.y][c.x] == CellColor::White ? CellColor::Black : CellColor::White;
		}
		CellColor get_color(Coordinates c) {
			return board[c.y][c.x];
		}

		void print() {
			for (uint32_t i = 0; i < height; i++) {
				for (uint32_t j = 0; j < width; j++) {
					if (board[i][j] == CellColor::White)
						putchar(static_cast<char>(219));
					else
						fmt::print(" "); // static_cast<char>(176);
				}
				fmt::print("\n");
			}

		}

		friend std::ostream& operator<< (std::ostream& os, const Board& b) {
			for (uint32_t i = 0; i < b.height; i++) {
				for (uint32_t j = 0; j < b.width; j++) {
					if (b.board[i][j] == CellColor::White)
						os << static_cast<char>(219);
					else
						os << ' '; // static_cast<char>(176);
				}
				os << "\n";
			}
			return os;
		}
		uint32_t get_width() { return width; }
		uint32_t get_height() { return height; }
	private:
		const uint32_t height, width;
		CellColor** board = nullptr;
	};

	class Ant {
	protected:
		void safe_inc_x(int& x, Direction& d, Board& b) {
			if (x == b.get_width() - 1) {
				--x;
				d = Direction::West;
			}
			else {
				++x;
				d = Direction::East;
			}
		};
		void safe_dec_x(int& x, Direction& d, Board& b) {
			if (x == 0) {
				++x;
				d = Direction::East;
			}
			else {
				--x;
				d = Direction::West;
			}
		};

		void safe_inc_y(int& y, Direction& d, Board& b) {
			if (y == b.get_height() - 1) {
				--y;
				d = Direction::North;
			}
			else {
				++y;
				d = Direction::South;
			}
		};
		void safe_dec_y(int& y, Direction& d, Board& b) {
			if (y == 0) {
				++y;
				d = Direction::South;
			}
			else {
				--y;
				d = Direction::North;
			}
		};
	public:
		Ant(int x, int y)
			: c{ x, y }
		{
			static Direction dirs[4] = { Direction::West, Direction::East, Direction::North, Direction::South };
			static std::uniform_int_distribution u(0, 3);
			d = dirs[u(e)];
		}
		void move(Board& b) {
			static std::map<Direction,
				std::function<void(Ant& ant, Board& b)>>
				navigate_funcs
			{
				{Direction::East,
				[](Ant& ant, Board& b) {
					auto& coord = ant.coords();
					auto& y = coord.y;
					auto& d = ant.dir();
					switch (b.get_color(coord)) {
					case CellColor::White:
						ant.safe_dec_y(y, d, b);
						break;
					case CellColor::Black:
						ant.safe_inc_y(y, d, b);
						break;
					default:
						break;
					}
				}
				},
				{Direction::West,
					[](Ant& ant, Board& b) {
					auto& coord = ant.coords();
					auto& y = coord.y;
					auto& d = ant.dir();
					switch (b.get_color(coord)) {
					case CellColor::White:
						ant.safe_inc_y(y, d, b);
						break;
					case CellColor::Black:
						ant.safe_dec_y(y, d, b);
						break;
					default:
						break;
					}
				}
				},
				{Direction::North,
					[](Ant& ant, Board& b) {
					auto& coord = ant.coords();
					auto& x = coord.x;
					auto& d = ant.dir();
					switch (b.get_color(coord)) {
					case CellColor::White:
						ant.safe_dec_x(x, d, b);
						break;
					case CellColor::Black:
						ant.safe_inc_x(x, d, b);
						break;
					default:
						break;
					}
				}
				},
				{Direction::South,
					[](Ant& ant, Board& b) {
					auto& coord = ant.coords();
					auto& x = coord.x;
					auto& d = ant.dir();
					switch (b.get_color(coord)) {
					case CellColor::White:
						ant.safe_inc_x(x, d, b);
						break;
					case CellColor::Black:
						ant.safe_dec_x(x, d, b);
						break;
					default:
						break;
					}
				}}
			};
			navigate_funcs[d](*this, b);
		}
	protected:
		friend class Game;
		Coordinates& coords() { return c; }
		Direction& dir() { return d; }

	private:
		Coordinates c;
		Direction d;
	};

	class Game {
	public:
		Game(int xb, int yb, int nAnts)
			: board{ xb, yb }
		{
			std::uniform_int_distribution ux(0, xb - 1);
			std::uniform_int_distribution uy(0, yb - 1);
			for (int i = 0; i < nAnts; ++i) {
				ants.emplace_back(ux(e), uy(e));
			}
		}
		void take_turn() {
			for (auto& ant : ants) {
				auto ant_coords = ant.coords();
				ant.move(board);
				board.chage_color(ant_coords);
			}
		}
		void print() {
			board.print();
		}
	private:
		Board board;
		std::vector<Ant> ants;
	};
}