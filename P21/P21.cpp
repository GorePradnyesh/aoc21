/*
**
*/

#include "P21.h"
#include "Die.h"

#include <iostream>
#include <iomanip>
#include <array>

#include <chrono>
#include <thread>

namespace
{
	void RollTest()
	{
		DiePtr die = std::make_shared<DeterministicDie>(100);
		std::uint32_t rollCount = 50;
		for(std::uint32_t rollNum = 0; rollNum < rollCount; rollNum++)
		{
			auto rolled = die->GenerateNext();
			std::cout << "\r" << "After roll#: " << std::setw(3) << die->GetRollCount() << " Generated roll: " << std::setw(3) << rolled << std::flush;
			std::this_thread::sleep_for (std::chrono::milliseconds(40));
		}
	}
	
	struct Player
	{
		Player(std::uint32_t inStartingPos)
		:mCurrentPos(inStartingPos)
		{}
		
		std::uint32_t mScore { 0 };
		std::uint32_t mCurrentPos { 0 };
		
	};
	using PlayerPtr = std::shared_ptr<Player>;
	
	/**/
	std::uint32_t GetNextPos(
		std::uint32_t inRollCount,
		std::uint32_t inCurrentPos,
		std::uint32_t inSpaceCount)
	{
		// 10, 20, 30 -> position 10
		if(!((inRollCount + inCurrentPos) % inSpaceCount))
		{
			return inSpaceCount;
		}
		return (inRollCount + inCurrentPos) % inSpaceCount;
	}
	
	/**/
	void RunGame()
	{
		constexpr std::uint32_t playerCount = 2;
		constexpr std::uint32_t dieSides = 100;
		DiePtr gameDie = std::make_shared<DeterministicDie>(dieSides);
		constexpr std::uint32_t spaceCount = 10;	// circular board
		
		std::array<PlayerPtr, playerCount> players { std::make_shared<Player>(9), std::make_shared<Player>(4) };
		
		std::uint32_t turnCount { 0 };
		std::uint32_t winningPlayer = 0;
		while(1)
		{
			constexpr std::uint32_t perPlayerRollCount = 3;
			std::uint32_t playerNumber = turnCount % playerCount;
			auto currentPlayer = players[playerNumber];
			
			std::uint32_t rollCount =  { 0 };
			
			for(std::uint32_t playerRoll = 0; playerRoll < perPlayerRollCount; playerRoll++)
			{
				rollCount += gameDie->GenerateNext();
			}
			
			currentPlayer->mCurrentPos = GetNextPos(rollCount, currentPlayer->mCurrentPos, spaceCount);
			currentPlayer->mScore += currentPlayer->mCurrentPos;
			
			std::cout << "Player #" << playerNumber
				<< " rolled: " << rollCount
				<< ", moves to pos: " << currentPlayer->mCurrentPos
				<< ", total score: " << currentPlayer->mScore
				<< "\n";
			turnCount++;
			
			if(currentPlayer->mScore >= 1000)
			{
				winningPlayer = playerNumber;
				break;
			}
			
		}
		std::uint32_t losingPlayerNumber = (winningPlayer + 1) % playerCount;
		std::cout << "Losing player score " << players[losingPlayerNumber]->mScore << "\n";
		std::cout << "Die Roll:  " << gameDie->GetRollCount() << "\n";
		std::cout << "Final Value :  " << players[losingPlayerNumber]->mScore * gameDie->GetRollCount() << "\n";
		
		
	}

} // unnamed

namespace P21
{
/**/
void Process()
{
	// RollTest();
	RunGame();
	std::cout << std::endl;
}

} // namespace P21
