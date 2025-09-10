#pragma once


#include <raylib.h>
#include <array>
#include "pieces.h"

constexpr const char* SPRITE_MAP_PATH = "../textures/chess-pieces-spritesheet.png";

enum Player
{
	PL_WHITE = 0,
	PL_BLACK
};

enum ClickState
{
	NO_CLICK = 0,
	FIRST_CLICK,
	SECOND_CLICK
};

class Game
{
public:
	Game(int screenWidth, int screenHeight, const char* title);

	void run();

	void render_board();

	void load_assets();

	~Game();

private:


	int screenWidth;
	int screenHeight;

	const char* title;

	bool isGameOver;


	std::array<Pieces, 64> boardSetup;

	struct EnPassentOppertunity
	{
		std::pair<int, int> possiblePositions;
		bool whenImplemented = false;
	};

	std::vector<EnPassentOppertunity> _enPassantOppertunities;

	struct PiecesTexturePos
	{
		Rectangle rect;
		Pieces piece;
	};

	struct
	{
		int width;
		int height;
		Texture2D tex;
	} texInfo;
	

	std::vector<PiecesTexturePos> piecesRects;

	unsigned int piecesLeft = 12;

	struct 
	{ 
		ClickState state = NO_CLICK;
		int firstPos = -1;
		int secondPos = -1;
		int hoverPos = -1;

		void reset()
		{
			state = NO_CLICK;
			firstPos = -1;
			secondPos = -1;
		}
	} click;

	Player player = PL_WHITE;	
	
	void process_input();

	int get_x_pos(int index) const;
	int get_y_pos(int index) const;

	bool valid_piece(int index) const;

	bool piece_exists(int index) const;

	int get_index(int x, int y) const;

	int get_index_from_mouse_pos() const;


	bool can_move_here(int posOfPiece, int movePos);

	// Handles
	void handle_resize();
	void handle_clicks();
};