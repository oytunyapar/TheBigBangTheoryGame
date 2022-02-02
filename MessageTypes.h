//
// Created by oytun on 2.02.2022.
//
#pragma once

enum MessageTypes
{
    JOIN,
    GAME_STARTED,
    CHOICE,
    RESULT,
    LEAVE
};

enum Result{
    WIN,
    LOST,
    TIE
};