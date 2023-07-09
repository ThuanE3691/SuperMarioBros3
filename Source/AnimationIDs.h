#pragma once

// -------------------- MARIO --------------------------

#pragma region MARIO

#define ID_ANI_MARIO_IDLE_RIGHT 400
#define ID_ANI_MARIO_IDLE_LEFT 401

#define ID_ANI_MARIO_WALKING_RIGHT 500
#define ID_ANI_MARIO_WALKING_LEFT 501

#define ID_ANI_MARIO_RUNNING_RIGHT 600
#define ID_ANI_MARIO_RUNNING_LEFT 601

#define ID_ANI_MARIO_JUMP_WALK_RIGHT 700
#define ID_ANI_MARIO_JUMP_WALK_LEFT 701

#define ID_ANI_MARIO_JUMP_RUN_RIGHT 800
#define ID_ANI_MARIO_JUMP_RUN_LEFT 801

#define ID_ANI_MARIO_SIT_RIGHT 900
#define ID_ANI_MARIO_SIT_LEFT 901

#define ID_ANI_MARIO_BRACE_RIGHT 1000
#define ID_ANI_MARIO_BRACE_LEFT 1001

#define ID_ANI_MARIO_BIG_TRANSFORM_TO_SMALL_LEFT	1011
#define ID_ANI_MARIO_BIG_TRANSFORM_TO_SMALL_RIGHT	1021

#define ID_ANI_MARIO_BIG_KICK_LEFT	1031
#define ID_ANI_MARIO_BIG_KICK_RIGHT	1041

#define ID_ANI_MARIO_BIG_HOLDING_LEFT_IDLE	1051
#define ID_ANI_MARIO_BIG_HOLDING_RIGHT_IDLE	1052
#define ID_ANI_MARIO_BIG_HOLDING_LEFT_WALK	1061
#define ID_ANI_MARIO_BIG_HOLDING_RIGHT_WALK	1062
#define ID_ANI_MARIO_BIG_HOLDING_LEFT_RUN	1071
#define ID_ANI_MARIO_BIG_HOLDING_RIGHT_RUN	1072

#define ID_ANI_MARIO_DIE 999

// SMALL MARIO
#define ID_ANI_MARIO_SMALL_IDLE_RIGHT 1100
#define ID_ANI_MARIO_SMALL_IDLE_LEFT 1102

#define ID_ANI_MARIO_SMALL_WALKING_RIGHT 1200
#define ID_ANI_MARIO_SMALL_WALKING_LEFT 1201

#define ID_ANI_MARIO_SMALL_RUNNING_RIGHT 1300
#define ID_ANI_MARIO_SMALL_RUNNING_LEFT 1301

#define ID_ANI_MARIO_SMALL_BRACE_RIGHT 1400
#define ID_ANI_MARIO_SMALL_BRACE_LEFT 1401

#define ID_ANI_MARIO_SMALL_JUMP_WALK_RIGHT 1500
#define ID_ANI_MARIO_SMALL_JUMP_WALK_LEFT 1501

#define ID_ANI_MARIO_SMALL_JUMP_RUN_RIGHT 1600
#define ID_ANI_MARIO_SMALL_JUMP_RUN_LEFT 1601

#define ID_ANI_MARIO_SMALL_TRANSFORM_TO_BIG_LEFT	1700
#define ID_ANI_MARIO_SMALL_TRANSFORM_TO_BIG_RIGHT	1701

#define ID_ANI_MARIO_SMALL_KICK_LEFT	1711
#define ID_ANI_MARIO_SMALL_KICK_RIGHT	1721

#define ID_ANI_MARIO_SMALL_HOLDING_LEFT_IDLE	1731
#define ID_ANI_MARIO_SMALL_HOLDING_RIGHT_IDLE	1741
#define ID_ANI_MARIO_SMALL_HOLDING_LEFT_WALK	1751
#define ID_ANI_MARIO_SMALL_HOLDING_RIGHT_WALK	1761
#define ID_ANI_MARIO_SMALL_HOLDING_LEFT_RUN	1771
#define ID_ANI_MARIO_SMALL_HOLDING_RIGHT_RUN	1781


#pragma endregion


// -------------------- ENEMIES ------------------------

#pragma region ENEMIES

// -------------------- GOOMBA -------------------------

#define ID_ANI_GOOMBA_WALKING 5000
#define ID_ANI_GOOMBA_DIE 5001
#define ID_ANI_GOOMBA_DIE_BY_ATTACK	5002

#define ID_ANI_GOOMBA_RED_WING_WALK	5011
#define ID_ANI_GOOMBA_RED_WING_JUMP	5012

#define ID_ANI_GOOMBA_RED_WING_FLY	5013

#define ID_ANI_GOOMBA_RED_WALK	5021
#define ID_ANI_GOOMBA_RED_DIE	5031
#define ID_ANI_GOOMBA_RED_WING_DIE_BY_ATTACK	5032
#define ID_ANI_GOOMBA_RED_DIE_BY_ATTACK	5033

// ------------------- KOOPA ---------------------------

// RED KOOPA

#define ID_ANI_KOOPA_WALKING_LEFT 6000
#define ID_ANI_KOOPA_WALKING_RIGHT 6001
#define ID_ANI_KOOPA_SHELL_IDLE 6002
#define ID_ANI_KOOPA_SHELL_ROTATE 6003
#define ID_ANI_KOOPA_SHELL_TRANSFORM_WALKING 6004
#define ID_ANI_KOOPA_DIE_REVERSE_SHELL	6005

// GREEN KOOPA

#define ID_ANI_KOOPA_GREEN_WALKING_LEFT 6100
#define ID_ANI_KOOPA_GREEN_WALKING_RIGHT 6101
#define ID_ANI_KOOPA_GREEN_SHELL_IDLE 6102
#define ID_ANI_KOOPA_GREEN_SHELL_ROTATE 6103
#define ID_ANI_KOOPA_GREEN_SHELL_TRANSFORM_WALKING 6104
#define ID_ANI_KOOPA_GREEN_DIE_REVERSE_SHELL	6105

// GREEN KOOPA WITH WING
#define ID_ANI_KOOPA_GREEN_FLY_LEFT	6200
#define ID_ANI_KOOPA_GREEN_FLY_RIGHT	6201

// ------------------- PIRANHA --------------------------

// PIRANHA

// RED

#define ID_ANI_PIRANHA_SHOOT_FIRE_TOP_LEFT	7001
#define ID_ANI_PIRANHA_SHOOT_FIRE_TOP_RIGHT	7002
#define ID_ANI_PIRANHA_SHOOT_FIRE_BOTTOM_LEFT	7003
#define ID_ANI_PIRANHA_SHOOT_FIRE_BOTTOM_RIGHT	7004

#define ID_ANI_PIRANHA_MOVE_HEAD_TOP_LEFT	7005
#define ID_ANI_PIRANHA_MOVE_HEAD_TOP_RIGHT	7006
#define ID_ANI_PIRANHA_MOVE_HEAD_BOTTOM_LEFT	7007
#define ID_ANI_PIRANHA_MOVE_HEAD_BOTTOM_RIGHT	7008

#define ID_ANI_PIRANHA_DIE_BY_ATTACK	7010

// GREEN

#define ID_ANI_PIRANHA_GREEN_SHOOT_FIRE_TOP_LEFT	7101
#define ID_ANI_PIRANHA_GREEN_SHOOT_FIRE_TOP_RIGHT	7102
#define ID_ANI_PIRANHA_GREEN_SHOOT_FIRE_BOTTOM_LEFT	7103
#define ID_ANI_PIRANHA_GREEN_SHOOT_FIRE_BOTTOM_RIGHT	7104

#define ID_ANI_PIRANHA_GREEN_MOVE_HEAD_TOP_LEFT	7105
#define ID_ANI_PIRANHA_GREEN_MOVE_HEAD_TOP_RIGHT	7106
#define ID_ANI_PIRANHA_GREEN_MOVE_HEAD_BOTTOM_LEFT	7107
#define ID_ANI_PIRANHA_GREEN_MOVE_HEAD_BOTTOM_RIGHT	7108


// VENUS

#define ID_ANI_VENUS_MOVE	7200

// FIRE BULLET

#define ID_ANI_FIRE_BULLET_LEFT	7020
#define ID_ANI_FIRE_BULLET_RIGHT	7021


#pragma endregion

// -------------------- BRICK --------------------------

#define ID_ANI_BRICK 10000

// -------------------- COIN ---------------------------

#define ID_ANI_COIN 11000
#define ID_ANI_COIN_DISAPPEAR 11100
#define ID_ANI_COIN_STATIC	11200

// -------------------- QUESTION BLOCK -----------------

#define ID_ANI_QUESTION_BLOCK 12000
#define ID_ANI_EMPTY_BLOCK 13000


// -------------------- POWER UP -----------------

#define ID_ANI_MUSHROOM 14000

// -------------------- BUTTON -------------------

#define ID_ANI_BUTTON_APPEAR 15000
#define ID_ANI_BUTTON_NORMAL 15100
#define ID_ANI_BUTTON_JUMP_ON	15200
