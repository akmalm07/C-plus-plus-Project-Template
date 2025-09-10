#pragma once


#include <raylib.h>
#include <array>
#include "engine.h"

constexpr const char* SPRITE_MAP_PATH = "../textures/chess-pieces-spritesheet.png";

enum ClickState
{
	NO_CLICK = 0,
	FIRST_CLICK,
	SECOND_CLICK
};

class Game
{
public:
	Game(int screenWidth, int screenHeight, const char* title, Player player);

	void run();

	void render_board();

	void load_assets();

	~Game();

private:


	int screenWidth;
	int screenHeight;

	const char* title;

	bool isGameOver;


	struct PiecesTexturePos
	{
		Rectangle rect;
		Pieces piece;
	};

	std::vector<PiecesTexturePos> piecesRects;
	
	struct
	{
		int width;
		int height;
		Texture2D tex;
	} texInfo;
	
	ChessEngine chessEngine;


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

	int get_index(int x, int y) const;
	int get_index_from_mouse_pos() const;


	// Handles
	void handle_resize();
	void handle_clicks();
};