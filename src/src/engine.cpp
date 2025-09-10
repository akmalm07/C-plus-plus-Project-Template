#include "headers.h"
#include "engine.h"



// Impleent a stateless, function based chess design!
// Instead of having to see if the pawn moved, we cna simply check if the pieces is in its starting position
// This will make it easier to implement an AI later on


ChessEngine::ChessEngine(Player p)
	: player(p)
{
	
	reset_board();
}

void ChessEngine::reset_board()
{
	boardSetup = {
		B_ROOK,  B_KNIGHT,  B_BISHOP,  B_QUEEN,  B_KING,  B_BISHOP,  B_KNIGHT,  B_ROOK,

		B_PAWN,  B_PAWN,    B_PAWN,    B_PAWN,   B_PAWN,  B_PAWN,    B_PAWN,    B_PAWN,

		EMPTY,   EMPTY,     EMPTY,     EMPTY,    EMPTY,   EMPTY,     EMPTY,     EMPTY,

		EMPTY,   EMPTY,     EMPTY,     EMPTY,    EMPTY,   EMPTY,     EMPTY,     EMPTY,

		EMPTY,   EMPTY,     EMPTY,     EMPTY,    EMPTY,   EMPTY,     EMPTY,     EMPTY,

		EMPTY,   EMPTY,     EMPTY,     EMPTY,    EMPTY,   EMPTY,     EMPTY,     EMPTY,

		W_PAWN,  W_PAWN,    W_PAWN,    W_PAWN,   W_PAWN,  W_PAWN,    W_PAWN,    W_PAWN,

		W_ROOK,  W_KNIGHT,  W_BISHOP,  W_QUEEN,  W_KING,  W_BISHOP,  W_KNIGHT,  W_ROOK
	};

	if (player == PL_BLACK)
	{
		std::reverse(boardSetup.begin(), boardSetup.end());
	}
}

Pieces ChessEngine::piece_at(int index) const
{
	return boardSetup[index];
}

int ChessEngine::piece_count() const
{
	return piecesLeft;
}

MoveState ChessEngine::move_piece(int from, int to)
{
	if (!valid_piece(from))
		return MOVE_INVALID;

	switch (boardSetup[from])
	{
	case W_PAWN:
	case B_PAWN:
		return handle_pawn_move(from, to);
	case W_ROOK:
	case B_ROOK:
		return handle_rook_move(from, to);
	case W_KNIGHT:
	case B_KNIGHT:
		return handle_knight_move(from, to);
	case W_BISHOP:
	case B_BISHOP:
		return handle_bishop_move(from, to);

	case W_QUEEN:
	case B_QUEEN:
		return handle_queen_move(from, to);
	case W_KING:
	case B_KING:
		return handle_king_move(from, to);
	default:
		return MOVE_INVALID;
	}
}

size_t ChessEngine::get_board_size() const
{
	return 64;
}


bool ChessEngine::piece_exists(int index) const
{
	return boardSetup[index] != EMPTY;
}

bool ChessEngine::valid_piece(int index) const
{
	return piece_exists(index) &&
		(player == PL_WHITE
			? (W_KING <= boardSetup[index] && boardSetup[index] <= W_PAWN)
			: (B_KING <= boardSetup[index] && boardSetup[index] <= B_PAWN));
}

std::array<Pieces, 64> ChessEngine::get_board() const
{
	return boardSetup;
}

ChessEngine::RowCol ChessEngine::get_row_col(int from, int to) const
{
	RowCol rc;
	rc.fromRow = from / 8;
	rc.fromCol = from % 8;
	rc.toRow = to / 8; 
	rc.toCol = to % 8;

	return rc;
}

bool ChessEngine::is_other_player_piece(int index) const
{ 
	return player == PL_WHITE
		? (B_KING <= boardSetup[index] && boardSetup[index] <= B_PAWN)
		: (W_KING <= boardSetup[index] && boardSetup[index] <= W_PAWN);
}



//  0   1   2   3   4   5   6   7 
//  8   9   10  11  12  13  14  15 
//  16  17  18  19  20  21  22  23 
//  24  25  26  27  28  29  30  31 
//  32  33  34  35  36  37  38  39 
//  40  41  42  43  44  45  46  47 
//  48  49  50  51  52  53  54  55 
//  56  57  58  59  60  61  62  63

MoveState ChessEngine::handle_pawn_move(int from, int to)
{
	// The + 1 are values to represent direction. Direction is always one
	
	std::function<bool(const RowCol&)> move_to_right_left = [](const RowCol& rc)
	{
			return rc.toCol == rc.fromCol - 1 || rc.toCol == rc.fromCol + 1;
	};

	RowCol rc = get_row_col(from, to);
	// Standard move
	if (rc.toCol == rc.fromCol && rc.toRow == rc.fromRow - 1 && boardSetup[to] == EMPTY)
	{
		// Promotion without capture
		if (rc.toRow != 0)
		{
			boardSetup[to] = boardSetup[from];
			boardSetup[from] = EMPTY;
		}
		else
		{
			boardSetup[to] = boardSetup[from];
			boardSetup[from] = EMPTY;
			check_pawn_promotion(to);
		}
		return MOVE_SUCCESS;
	}
	// Double move from starting position
	if (rc.toCol == rc.fromCol && rc.fromRow == 6 /* 6 is the start row */ && rc.toRow == rc.fromRow - 2 &&
		boardSetup[row_col_to_index(rc.toRow + 1, rc.toCol)] == EMPTY && boardSetup[to] == EMPTY)
	{
		boardSetup[to] = boardSetup[from];
		boardSetup[from] = EMPTY;
		return MOVE_SUCCESS;
	}
	// Capture move
	if ((move_to_right_left(rc)) && rc.toRow == rc.fromRow - 1 &&
		is_other_player_piece(to))
	{
		// Promotion with capture
		if (rc.toRow != 0)
		{
			boardSetup[to] = boardSetup[from];
			boardSetup[from] = EMPTY;
			piecesLeft--;
		}
		else
		{
			boardSetup[to] = boardSetup[from];
			boardSetup[from] = EMPTY;
			piecesLeft--;
			check_pawn_promotion(to);
		}

		return MOVE_CAPTURE;
	}

	if ((move_to_right_left(rc)) && rc.toRow == rc.fromRow - 1 && rc.toRow == 3 &&
		en_passent_avalible(to))
	{
		boardSetup[to] = boardSetup[from];
		boardSetup[from] = EMPTY;
		boardSetup[row_col_to_index(rc.toRow + 1, rc.toCol)] = EMPTY;
		piecesLeft--;
		return MOVE_CAPTURE;
	}


	return MOVE_INVALID;
}


MoveState ChessEngine::handle_rook_move(int from, int to)
{
	RowCol rc = get_row_col(from, to);


	std::function<MoveState(int)> handle_straight_line_move = [this, to, from](int direction)
	{
		int current = from;
		while (true)
		{
			current += direction;
			if (current < 0 || current >= 64)
				return MOVE_INVALID;
			if (current == to)
			{
				if (is_other_player_piece(current))
				{
					boardSetup[to] = boardSetup[from];
					boardSetup[from] = EMPTY;
					piecesLeft--;
					return MOVE_CAPTURE;
				}
				else if (boardSetup[current] == EMPTY)
				{
					boardSetup[to] = boardSetup[from];
					boardSetup[from] = EMPTY;
					return MOVE_SUCCESS;
				}
				else
				{
					return MOVE_INVALID;
				}
			}
			else if (boardSetup[current] != EMPTY)
			{
				return MOVE_INVALID;
			}
			if (direction == 1 && current % 8 == 0) 
				return MOVE_INVALID;
			if (direction == -1 && current % 8 == 7) 
				return MOVE_INVALID;
		}
		};

	if (rc.fromRow == rc.toRow)
	{
		if (rc.toCol > rc.fromCol)
		{
			return handle_straight_line_move(1); // Move right
		}
		else
		{
			return handle_straight_line_move(-1); // Move left
		}
	}
	else if (rc.fromCol == rc.toCol)
	{
		if (rc.toRow > rc.fromRow)
		{
			return handle_straight_line_move(8); // Move down
		}
		else
		{
			return handle_straight_line_move(-8); // Move up
		}
	}
	return MOVE_INVALID;
}

MoveState ChessEngine::handle_knight_move(int from, int to)
{
	RowCol rc = get_row_col(from, to);
	if ((std::abs(rc.toRow - rc.fromRow) == 2 && std::abs(rc.toCol - rc.fromCol) == 1) ||
		(std::abs(rc.toRow - rc.fromRow) == 1 && std::abs(rc.toCol - rc.fromCol) == 2))
	{
		if (is_other_player_piece(to))
		{
			boardSetup[to] = boardSetup[from];
			boardSetup[from] = EMPTY;
			piecesLeft--;
			return MOVE_CAPTURE;
		}
		else if (boardSetup[to] == EMPTY)
		{
			boardSetup[to] = boardSetup[from];
			boardSetup[from] = EMPTY;
			return MOVE_SUCCESS;
		}
	}
	return MOVE_INVALID;

}

MoveState ChessEngine::handle_bishop_move(int from, int to)
{
	RowCol rc = get_row_col(from, to);
	std::function<MoveState(int)> handle_diagonal_move = [this, to, from](int direction)
	{
		int current = from;
		while (true)
		{
			current += direction;
			if (current < 0 || current >= 64)
				return MOVE_INVALID;
			if (current == to)
			{
				if (is_other_player_piece(current))
				{
					boardSetup[to] = boardSetup[from];
					boardSetup[from] = EMPTY;
					piecesLeft--;
					return MOVE_CAPTURE;
				}
				else if (boardSetup[current] == EMPTY)
				{
					boardSetup[to] = boardSetup[from];
					boardSetup[from] = EMPTY;
					return MOVE_SUCCESS;
				}
				else
				{
					return MOVE_INVALID;
				}
			}
			else if (boardSetup[current] != EMPTY)
			{
				return MOVE_INVALID;
			}
			if (direction == 7 && current % 8 == 0) // Moving down-left
				return MOVE_INVALID;
			if (direction == -7 && current % 8 == 7) // Moving up-right
				return MOVE_INVALID;
			if (direction == 9 && current % 8 == 7) // Moving down-right
				return MOVE_INVALID;
			if (direction == -9 && current % 8 == 0) // Moving up-left
				return MOVE_INVALID;
		}
	};
	if (std::abs(rc.toRow - rc.fromRow) == std::abs(rc.toCol - rc.fromCol))
	{
		if (rc.toRow > rc.fromRow && rc.toCol > rc.fromCol)
		{
			return handle_diagonal_move(9); // Move down-right
		}
		else if (rc.toRow > rc.fromRow && rc.toCol < rc.fromCol)
		{
			return handle_diagonal_move(7); // Move down-left
		}
		else if (rc.toRow < rc.fromRow && rc.toCol > rc.fromCol)
		{
			return handle_diagonal_move(-7); // Move up-right
		}
		else if (rc.toRow < rc.fromRow && rc.toCol < rc.fromCol)
		{
			return handle_diagonal_move(-9); // Move up-left
		}
	}

	return MOVE_INVALID;
}

MoveState ChessEngine::handle_queen_move(int from, int to)
{
	RowCol rc = get_row_col(from, to);
	if (rc.fromRow == rc.toRow || rc.fromCol == rc.toCol)
	{
		return handle_rook_move(from, to);
	}
	else if (std::abs(rc.toRow - rc.fromRow) == std::abs(rc.toCol - rc.fromCol))
	{
		return handle_bishop_move(from, to);
	}
	return MOVE_INVALID;
}

MoveState ChessEngine::handle_king_move(int from, int to)
{
	RowCol rc = get_row_col(from, to);
	if (std::abs(rc.toRow - rc.fromRow) <= 1 && std::abs(rc.toCol - rc.fromCol) <= 1)
	{
		kingMoved = true;
		if (is_other_player_piece(to))
		{
			boardSetup[to] = boardSetup[from];
			boardSetup[from] = EMPTY;
			piecesLeft--;
			return MOVE_CAPTURE;
		}
		else if (boardSetup[to] == EMPTY)
		{
			boardSetup[to] = boardSetup[from];
			boardSetup[from] = EMPTY;
			return MOVE_SUCCESS;
		}
	}

	// Moving piece from 59 to 57
	// Moving piece from 59 to 61

	// Moving piece from 60 to 58
	// Moving piece from 60 to 62

	// Castling

	return handle_castling(from, to);
}

MoveState ChessEngine::handle_castling(int from, int to)
{
	if (kingMoved)
		return MOVE_INVALID;

	if (player == PL_BLACK)
	{
		if (to == 57 && boardSetup[56] == B_ROOK && boardSetup[57] == EMPTY && boardSetup[58] == EMPTY)
		{
			boardSetup[56] = EMPTY;
			boardSetup[57] = B_KING;
			boardSetup[58] = B_ROOK;
			boardSetup[59] = EMPTY;
			kingMoved = true;
			return MOVE_SUCCESS;
		}
		else if (to == 61 && boardSetup[63] == B_ROOK && boardSetup[62] == EMPTY && boardSetup[61] == EMPTY && boardSetup[60] == EMPTY)
		{
			boardSetup[59] = EMPTY;
			boardSetup[60] = B_ROOK;
			boardSetup[61] = B_KING;
			boardSetup[62] = EMPTY;
			boardSetup[63] = EMPTY;
			kingMoved = true;
			return MOVE_SUCCESS;
		}
	}
	else
	{
		if (to == 58 && boardSetup[56] == W_ROOK && boardSetup[57] == EMPTY && boardSetup[58] == EMPTY && boardSetup[59] == EMPTY)
		{
			boardSetup[56] = EMPTY;
			boardSetup[57] = EMPTY;
			boardSetup[58] = W_KING;
			boardSetup[57] = W_ROOK;
			boardSetup[60] = EMPTY;
			kingMoved = true;
			return MOVE_SUCCESS;
		}
		else if (to == 62 && boardSetup[63] == W_ROOK && boardSetup[62] == EMPTY && boardSetup[61] == EMPTY)
		{
			boardSetup[60] = EMPTY;
			boardSetup[61] = W_KING;
			boardSetup[62] = W_ROOK;
			boardSetup[63] = EMPTY;
			kingMoved = true;
			return MOVE_SUCCESS;
		}
	}

	return MOVE_INVALID;
}

int ChessEngine::row_col_to_index(int row, int col) const
{
	return (row * 8) + col;
}

void ChessEngine::check_pawn_promotion(int index)
{
	if (index / 8 == 0)
	{
		boardSetup[index] = player == PL_WHITE ? W_QUEEN : B_QUEEN;
	}
}

bool ChessEngine::en_passent_avalible(int to) const
{
	for (const auto& ep : enPassantOppertunities)
	{
		if (ep.whenImplemented + 1 == gameMovesCount &&
			(to == ep.underPosition))
		{
			return true;
		}
	}
	return false;
}
