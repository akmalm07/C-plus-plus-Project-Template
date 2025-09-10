#include "headers.h"
#include "game.h"


Game::Game(int screenWidth, int screenHeight, const char* title, Player player)
	: chessEngine(player)
{
	this->screenWidth = screenWidth;
	this->screenHeight = screenHeight;
	this->title = title;
	this->isGameOver = false;


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
		for (int j = 0; j < chessEngine.get_board_size(); j++)
		{
			if (chessEngine.piece_at(j) == EMPTY)
				continue;

			if (piecesRects[i].piece == chessEngine.piece_at(j))
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

		std::cout << "Moving piece from " << click.firstPos << " to " << click.secondPos << "\n";

		if (chessEngine.move_piece(click.firstPos, click.secondPos) != MOVE_INVALID)
		{
			int i = 0;

			click.reset();

			//boardSetup[click.secondPos] = boardSetup[click.firstPos];
			//boardSetup[click.firstPos] = EMPTY;
		}
		else
		{
			click.state = FIRST_CLICK;
		}
	}


}

void Game::load_assets()
{

	texInfo.tex = LoadTexture(SPRITE_MAP_PATH);

	texInfo.width = texInfo.tex.width / 6;
	texInfo.height = texInfo.tex.height / 2;

	piecesRects.reserve(chessEngine.piece_count());

	for (int i = 0; i < chessEngine.piece_count(); i++)
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
			if (chessEngine.valid_piece(index)) 
			{
				click.firstPos = index;
				click.state = FIRST_CLICK;
			}
			break;

		case FIRST_CLICK:
			click.secondPos = index;
			if (click.firstPos != click.secondPos) 
			{
				click.state = SECOND_CLICK;
				//boardSetup[click.secondPos] = boardSetup[click.firstPos];
				//boardSetup[click.firstPos] = EMPTY;
			}
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

	//std::cout << "Mouse Position: (" << mouse.x << ", " << mouse.y << ") -> Index: (" << row << ", " << col << ")\n";

	return row * 8 + col;
}

int Game::get_y_pos(int index) const
{
	return (index / 8) * (screenHeight / 8);
}


Game::~Game() = default;