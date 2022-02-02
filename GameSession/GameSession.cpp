//
// Created by oytun on 2.02.2022.
//
#include "GameSession.h"

GameSession::GameSession(pair<uint32_t,uint16_t> first_player_id, pair<uint32_t,uint16_t> second_player_id):
m_first_player_id(first_player_id),
m_second_player_id(second_player_id)
{
    m_beat_map[SCISSORS] = set<GameChoices>{PAPER,LIZARD};
    m_beat_map[PAPER] = set<GameChoices>{ROCK,SPOCK};
    m_beat_map[ROCK] = set<GameChoices>{LIZARD,SCISSORS};
    m_beat_map[LIZARD] = set<GameChoices>{PAPER,SPOCK};
    m_beat_map[SPOCK] = set<GameChoices>{ROCK,SCISSORS};
}

GameResultObject GameSession::playerChoice(pair<uint32_t,uint16_t> player_id, GameChoices choice)
{
    if(player_id == m_first_player_id && m_first_player_choice == INVALID)
    {
        m_first_player_choice = choice;
    }

    if(player_id == m_second_player_id && m_second_player_choice == INVALID)
    {
        m_second_player_choice = choice;
    }

    return gameResult();
}

GameResultObject GameSession::gameResult()
{
    GameResultObject result;
    result.status = WAITING;

    if(m_first_player_choice != INVALID && m_second_player_choice != INVALID)
    {
        auto first_player_beats = m_beat_map[m_first_player_choice];
        auto second_player_beats = m_beat_map[m_second_player_choice];

        if(first_player_beats.find(m_second_player_choice) != first_player_beats.end())
        {
            result.status = FINISHED_WINNER;
            result.winner = m_first_player_id;
            result.loser = m_second_player_id;
        }
        else if(second_player_beats.find(m_first_player_choice) != second_player_beats.end())
        {
            result.status = FINISHED_WINNER;
            result.winner = m_second_player_id;
            result.loser = m_first_player_id;
        }
        else
        {
            result.status = FINISHED_TIE;
            result.winner = m_second_player_id;
            result.loser = m_first_player_id;
        }

        m_first_player_choice = INVALID;
        m_second_player_choice = INVALID;
    }

    return result;
}