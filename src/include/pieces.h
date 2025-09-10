#pragma once

#include <array>

enum Pieces
{
	EMPTY = 0,
	W_KING,
	W_QUEEN,
	W_BISHOP,
	W_KNIGHT,
	W_ROOK,
	W_PAWN,
	B_KING,
	B_QUEEN,
	B_BISHOP,
	B_KNIGHT,
	B_ROOK,
	B_PAWN,
};


class Piece
{
public:
	Piece(std::array<int, 64>& boardSetup) : boardSetup(boardSetup) {};
	Piece(const Piece&) = default;

	virtual bool operator()(int posOfPiece, int movePos) const = 0;

	
	bool piece_exists(int index) const;

protected:
	

	std::array<int, 64>& boardSetup;
};
