//
// Created by oytun on 2.02.2022.
//
#pragma once

enum GameChoices
{
    INVALID,
    SCISSORS,
    PAPER,
    ROCK,
    LIZARD,
    SPOCK,
    NUMBER_OF_CHOICES
};

enum GameStatus
{
    FINISHED_WINNER,
    FINISHED_TIE,
    WAITING,
};
