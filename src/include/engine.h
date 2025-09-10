#pragma once

#include <array>
#include <functional>

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

enum Player
{
	PL_WHITE = 0,
	PL_BLACK
};

enum MoveState
{
	MOVE_SUCCESS = 0,
	MOVE_INVALID,
	MOVE_CAPTURE,

};


class ChessEngine
{
public:

	ChessEngine(Player player);

	void reset_board();

	Pieces piece_at(int index) const;

	int piece_count() const;

	MoveState move_piece(int from, int to);

	size_t get_board_size() const;

	bool piece_exists(int index) const;

	bool valid_piece(int index) const;
	
	std::array<Pieces, 64> get_board() const;


private:
	
	struct EnPassentOppertunity
	{
		int underPosition;
		bool whenImplemented = false;
	};

	Player player;

	std::array<Pieces, 64> boardSetup;

	std::vector<EnPassentOppertunity> enPassantOppertunities;

	unsigned int piecesLeft = 12;

	size_t gameMovesCount = 0;

	struct RowCol
	{
		int fromRow;
		int fromCol;
		int toRow;
		int toCol;
	};

	bool kingMoved = false;

	RowCol get_row_col(int from, int to) const;

	bool is_other_player_piece(int index) const;

	MoveState handle_pawn_move(int from, int to);
	MoveState handle_rook_move(int from, int to);
	MoveState handle_knight_move(int from, int to);
	MoveState handle_bishop_move(int from, int to);
	MoveState handle_queen_move(int from, int to);
	MoveState handle_king_move(int from, int to);

	bool is_mate() const;

	MoveState handle_castling(int from, int to);

	int row_col_to_index(int row, int col) const;

	void check_pawn_promotion(int index);
	bool en_passent_avalible(int to) const;
};