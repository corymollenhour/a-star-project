all : a-star random_board

a-star : a-star.cpp
	g++ a-star.cpp -o a-star

random_board : random_board.cpp
	g++ random_board.cpp -o random_board