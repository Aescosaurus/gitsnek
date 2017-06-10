/****************************************************************************************** 
 *	Chili DirectX Framework Version 16.07.20											  *	
 *	Game.cpp																			  *
 *	Copyright 2016 PlanetChili.net <http://www.planetchili.net>							  *
 *																						  *
 *	This file is part of The Chili DirectX Framework.									  *
 *																						  *
 *	The Chili DirectX Framework is free software: you can redistribute it and/or modify	  *
 *	it under the terms of the GNU General Public License as published by				  *
 *	the Free Software Foundation, either version 3 of the License, or					  *
 *	(at your option) any later version.													  *
 *																						  *
 *	The Chili DirectX Framework is distributed in the hope that it will be useful,		  *
 *	but WITHOUT ANY WARRANTY; without even the implied warranty of						  *
 *	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the						  *
 *	GNU General Public License for more details.										  *
 *																						  *
 *	You should have received a copy of the GNU General Public License					  *
 *	along with The Chili DirectX Framework.  If not, see <http://www.gnu.org/licenses/>.  *
 ******************************************************************************************/
#include "MainWindow.h"
#include "Game.h"
#include "SpriteCodex.h"

Game::Game( MainWindow& wnd )
	:
	wnd( wnd ),
	gfx( wnd ),
	brd( gfx ),
	rng( std::random_device()() ),
	snek( { 2,2 } ),
	goal( rng,brd,snek )
	// asdfasdfasdf asdasd asdasd
{
}

void Game::Go()
{
	gfx.BeginFrame();	
	UpdateModel();
	ComposeFrame();
	gfx.EndFrame();
}

void Game::UpdateModel()
{
	if( gameIsStarted )
	{
		if( !gameIsOver && gameIsStarted )
		{
			if( wnd.kbd.KeyIsPressed( VK_UP ) )
			{
				delta_loc = { 0,-1 };
			}
			if( wnd.kbd.KeyIsPressed( VK_DOWN ) )
			{
				delta_loc = { 0,1 };
			}
			if( wnd.kbd.KeyIsPressed( VK_LEFT ) )
			{
				delta_loc = { -1,0 };
			}
			if( wnd.kbd.KeyIsPressed( VK_RIGHT ) )
			{
				delta_loc = { 1,0 };
			}

			++snekMoveCounter;
			if( snekMoveCounter >= snekMovePeriod )
			{
				if( growInitialSnake < 2 )
				{
					snek.Grow();
					growInitialSnake++;
				}
				snekMoveCounter = 0;
				const Location next = snek.GetNextHeadLocation( delta_loc );
				if( !brd.IsInsideBoard( next ) ||
					snek.IsInTileExceptEnd( next ) )
				{
					gameIsOver = true;
				}
				else
				{
					const bool eating = (next == goal.GetLocation());
					if( eating )
					{
						snek.Grow();
						rocks[rockNum].Respawn( rng,brd,snek );
						rocks[rockNum].SetSpawned( true );
						++rockNum;
					}
					bool facePalming = false;
					for( int i = 0; i < rockNum; i++ )
					{
						facePalming = (next == rocks[i].GetLocation());
					}
					if( facePalming )
					{
						gameIsOver = true;
					}
					snek.MoveBy( delta_loc );
					if( eating )
					{
						goal.Respawn( rng,brd,snek );
						if( snekMovePeriod > 0 )
							--snekMovePeriod;
					}
				}
			}
			++snekSpeedupCounter;
			if( snekMoveCounter > snekSpeedupPeriod )
			{
				snekSpeedupCounter = 0;
				snekMovePeriod--;
			}
		}
	}
	else
	{
		gameIsStarted = wnd.kbd.KeyIsPressed( VK_RETURN );
	}
}

void Game::ComposeFrame()
{
	if( gameIsStarted )
	{
		snek.Draw( brd );
		goal.Draw( brd );
		if( gameIsOver )
		{
			SpriteCodex::DrawGameOver( 350,256,gfx );
		}
		brd.DrawBorder();
		/*
		for( int i = 0; i < totalRocks; i++ )
		{
			// rocks[i].Respawn( rng,brd,snek );
			rocks[i].Draw( brd );
		}
		*/
	}
	else
	{
		SpriteCodex::DrawTitle( 290,255,gfx );
	}
}
