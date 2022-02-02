//
// Created by oytun on 2.02.2022.
//
#pragma once

#include <map>
#include <set>

#include "GameParameters.h"

using namespace std;

struct GameResultObject
{
    GameStatus status;
    pair<uint32_t,uint16_t> winner;
    pair<uint32_t,uint16_t> loser;
};

class GameSession
{
public:
    GameSession(){}
    GameSession(pair<uint32_t,uint16_t> first_player_id, pair<uint32_t,uint16_t> second_player_id);
    GameResultObject playerChoice(pair<uint32_t,uint16_t> player_id, GameChoices choice);

private:
    GameResultObject gameResult();
    map<GameChoices,set<GameChoices>> m_beat_map;
    pair<uint32_t,uint16_t> m_first_player_id = {0,0};
    pair<uint32_t,uint16_t>  m_second_player_id = {0,0};;
    GameChoices m_first_player_choice = INVALID;
    GameChoices m_second_player_choice = INVALID;
};
