#include "headers.h"
#include "game.h"


Game::Game(int screenWidth, int screenHeight, const char* title)
	: canPawnMove(boardSetup)
{
	this->screenWidth = screenWidth;
	this->screenHeight = screenHeight;
	this->title = title;
	this->isGameOver = false;

	boardSetup = {
		B_ROOK, B_KNIGHT, B_BISHOP, B_QUEEN, B_KING, B_BISHOP, B_KNIGHT, B_ROOK,
		B_PAWN, B_PAWN, B_PAWN, B_PAWN, B_PAWN, B_PAWN, B_PAWN, B_PAWN,
		EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
		EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
		EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
		EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
		W_PAWN, W_PAWN, W_PAWN, W_PAWN, W_PAWN, W_PAWN, W_PAWN, W_PAWN,
		W_ROOK, W_KNIGHT, W_BISHOP, W_QUEEN, W_KING, W_BISHOP, W_KNIGHT, W_ROOK
	};

	SetConfigFlags(FLAG_WINDOW_RESIZABLE);

	InitWindow(screenWidth, screenHeight, title);
	SetTargetFPS(300);
}

void Game::run()
{

	load_assets();

	while (!WindowShouldClose() && !isGameOver)
	{
		process_input();
		
		BeginDrawing();
		ClearBackground(BLACK);

		render_board();
		
		EndDrawing();
	}
	CloseWindow();
}



void Game::render_board()
{

	// Draw chess board
	for (int i = 0; i < 64; i++)
	{
		int x = (i % 8) * (screenWidth / 8);
		int y = int(i / 8) * (screenHeight / 8);

		if (click.firstPos == i && click.state == FIRST_CLICK)
			DrawRectangle(x, y, screenWidth / 8, screenHeight / 8, RED);
		else if (click.hoverPos == i && click.state == FIRST_CLICK)
			DrawRectangle(x, y, screenWidth / 8, screenHeight / 8, GREEN);
		else if ((i + (i / 8)) % 2 == 0)
			DrawRectangle(x, y, screenWidth / 8, screenHeight / 8, LIGHTGRAY);
		else
			DrawRectangle(x, y, screenWidth / 8, screenHeight / 8, DARKGRAY);
	}


	int squareSize = screenWidth / 8;

	// Draw pieces

	for (int i = 0; i < piecesRects.size(); i++)
	{
		for (int j = 0; j < boardSetup.size(); j++)
		{
			if (boardSetup[j] == EMPTY)
				continue;

			if (piecesRects[i].piece == boardSetup[j])
			{
				int x = (j % 8) * squareSize;
				int y = (j / 8) * squareSize;
				Vector2 pos = { (float)x, (float)y };
				DrawTextureRec(texInfo.tex, piecesRects[i].rect, pos, WHITE);
			}
		}
	}

	// Decide move
	if (click.state == SECOND_CLICK)
	{
		if (can_move_here(click.firstPos, click.secondPos))
		{
			boardSetup[click.secondPos] = boardSetup[click.firstPos];
			boardSetup[click.firstPos] = EMPTY;
		}
	}


}

void Game::load_assets()
{

	texInfo.tex = LoadTexture(SPRITE_MAP_PATH);

	texInfo.width = texInfo.tex.width / 6;
	texInfo.height = texInfo.tex.height / 2;

	piecesRects.reserve(piecesLeft);

	for (int i = 0; i < piecesLeft; i++)
	{
		Rectangle rec{ (i % 6) * texInfo.width, (i / 6) * texInfo.height, texInfo.width, texInfo.height };

		Pieces piece = static_cast<Pieces>(i + 1);

		piecesRects.push_back({ rec, piece });
	}

}


void Game::process_input()
{
	handle_resize();

	handle_clicks();
}

void Game::handle_resize()
{
	if (IsWindowResized()) 
	{
		screenWidth = GetScreenWidth();
		screenHeight = GetScreenHeight();

		if (screenWidth != screenHeight) 
		{
			int newSize = std::min(screenWidth, screenHeight);
			screenWidth = screenHeight = newSize;
			SetWindowSize(newSize, newSize);
		}
		click.reset();
	}
}

void Game::handle_clicks()
{
	if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) 
	{
		int index = get_index_from_mouse_pos();

		switch (click.state) 
		{
		case NO_CLICK:
			if (valid_piece(index)) 
			{
				click.firstPos = index;
				click.state = FIRST_CLICK;
			}
			break;

		case FIRST_CLICK:
			click.secondPos = index;
			if (click.firstPos != click.secondPos) 
			{
				boardSetup[click.secondPos] = boardSetup[click.firstPos];
				boardSetup[click.firstPos] = EMPTY;
			}
			click.reset();
			break;

		default:
			click.reset();
			break;
		}
	}

	if (click.state == FIRST_CLICK) 
	{
		click.hoverPos = get_index_from_mouse_pos();

		if (IsKeyPressed(KEY_BACKSPACE))
		{
			click.reset();
		}
	}
}
int Game::get_x_pos(int index) const
{
	return (index % 8) * (screenWidth / 8);
}

int Game::get_index(int x, int y) const
{
	return (x % 8) + (y / 8);
}

int Game::get_index_from_mouse_pos() const
{
	Vector2 mouse = GetMousePosition();
	int col = mouse.x / (screenWidth / 8);
	int row = mouse.y / (screenHeight / 8);

	std::cout << "Mouse Position: (" << mouse.x << ", " << mouse.y << ") -> Index: (" << row << ", " << col << ")\n";

	return row * 8 + col;
}

bool Game::can_move_here(int posOfPiece, int movePos)
{
	//switch (boardSetup[posOfPiece])
	//{
	//case W_PAWN:
	//case B_PAWN:
	//	return can_pawn_move(posOfPiece, movePos);
	//case W_ROOK:
	//case B_ROOK:
	//	return can_rook_move(posOfPiece, movePos);
	//case W_KNIGHT:
	//case B_KNIGHT:
	//	return can_knight_move(posOfPiece, movePos);
	//case W_BISHOP:
	//case B_BISHOP:
	//	return can_bishop_move(posOfPiece, movePos);

	//case W_QUEEN:
	//case B_QUEEN:
	//	return can_queen_move(posOfPiece, movePos);
	//case W_KING:
	//case B_KING:
	//	return can_king_move(posOfPiece, movePos);
	//default:
	//	return false;
	//}
	return true;

}

int Game::get_y_pos(int index) const
{
	return (index / 8) * (screenHeight / 8);
}

bool Game::piece_exists(int index) const
{
	return boardSetup[index] != EMPTY;
}

bool Game::valid_piece(int index) const 
{
	return piece_exists(index) &&
		(player == PL_WHITE
			? (W_KING <= boardSetup[index] && boardSetup[index] <= W_PAWN)
			: (B_KING <= boardSetup[index] && boardSetup[index] <= B_PAWN));
}


Game::~Game() = default;