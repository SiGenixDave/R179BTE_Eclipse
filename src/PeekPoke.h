/*
 * PeekPoke.h
 *
 *  Created on: Nov 30, 2018
 *      Author: David Smail
 */

#ifndef PEEKPOKE_H_
#define PEEKPOKE_H_

void PeekSingleService(const char *str);
BOOLEAN PeekSingle (char cmdPtr[][MAX_PARAM_LENGTH]);
void PokeSingleService(const char *str);
BOOLEAN PokeSingle (char cmdPtr[][MAX_PARAM_LENGTH]);

#endif /* PEEKPOKE_H_ */
