#ifndef INTERACTIONS_H
#define INTERACTIONS_H

#include "ball.h"
#include "brick.h"
#include "paddle.h"
#include "powerup.h"

//Determine if two entities overlap
bool is_interacting(const entity& e1, const entity& e2);

//Resolve potential collision between the ball and the paddle
void handle_collision(ball& b, const paddle& p);

//Resolve potential collision between the ball and a brick
void handle_collision(ball& b, brick& p);

//Resolve potential powerups affecting the paddle or ball
void handle_powerups(powerup& u, paddle& p, ball& b);

#endif