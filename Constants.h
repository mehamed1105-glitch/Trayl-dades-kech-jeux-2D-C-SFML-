#pragma once

const int WINDOW_WIDTH = 1600;
const int WINDOW_HEIGHT = 800;

const float GRAVITY = 1800.f;
const float JUMP_FORCE = -850.f;
const float SLIDE_DURATION = 0.4f;

const float PLAYER_START_X = 200.f;
const float PLAYER_W = 160.f;
const float PLAYER_H = 400.f;
const float PLAYER_GROUND_Y = (float)WINDOW_HEIGHT - 120.f - PLAYER_H;
const float PLAYER_SLIDE_H = 200.f;

const float BASE_SPEED = 400.f;
const float SPEED_INCREMENT = 15.f;
const float MAX_SPEED = 900.f;

const float GAME_DURATION = 30.f;
const float LVL2_DURATION = 40.f;

const int MAX_LIVES = 3;