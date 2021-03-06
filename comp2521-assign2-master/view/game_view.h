////////////////////////////////////////////////////////////////////////
// COMP2521 19t0 ... the Fury of Dracula
// game_view.h: the Game View ADT
//
// 2014-07-01	v1.0	Team Dracula <cs2521@cse.unsw.edu.au>
// 2017-11-30	v1.1	Team Dracula <cs2521@cse.unsw.edu.au>
// 2018-12-31	v2.0	Team Dracula <cs2521@cse.unsw.edu.au>

#ifndef FOD__GAME_VIEW_H_
#define FOD__GAME_VIEW_H_

#include <stdbool.h>

#include "game.h"
#include "places.h"

typedef struct game_view *GameView;

/**
 * Creates a new view to summarise the current state of the game.
 *
 * @param past_plays is a string of all plays made in the game so far by
 *    all players (including Dracula) from earliest to most recent.
 *
 * @param messages is an array containing a `player_message` for each
 *    play in the game so far.  It will have exactly the same number of
 *    elements as there are plays in `past_plays`.  The message from the
 *    first play will be at index 0, and so on.  The contents of each
 *    `player_message` will be exactly as provided by the player.
 *
 * The "player_message" type is defined in game.h.
 * You are free to ignore messages if you wish.
 */
GameView gv_new (char *past_plays, player_message messages[]);

/**
 * Frees all resources allocated for `gv`.
 * After this has been called, `gv` should not be accessed.
 */
void gv_drop (GameView gv);

/**
 * Get the current round
 */
round_t gv_get_round (GameView gv);

/**
 * Get the current player; effectively, whose turn is it?
 */
enum player gv_get_player (GameView gv);

/**
 * Get the current score, a positive integer between 0 and 366.
 */
int gv_get_score (GameView gv);

/**
 * Get the current health points for a given player.
 * @param player specifies which players's life/blood points to return;
 * @returns a value between 0..9 for Hunters, or >0 for Dracula
 */
int gv_get_health (GameView gv, enum player player);

/**
 * Get the current location of a given player.
 *
 * May be `UNKNOWN_LOCATION` if the player has not had a turn yet
 * (i.e., at the beginning of the game when the round is 0)
 *
 * Possible values include:
 * - in the interval 0..70, if the player was (known to be) in a
 *   particular city or on a particular sea;
 * - `CITY_UNKNOWN`, if Dracula was known to be in a city;
 * - `SEA_UNKNOWN`, if Dracula was known to be at sea;
 * - `HIDE`, if Dracula was known to have made a hide move;
 * - `DOUBLE_BACK_n`, where n is [1...5], if Dracula was known to have
 *   made a double back move _n_ positions back in the trail; e.g.,
 *   `DOUBLE_BACK_1` is the last place place he visited; or
 * - `TELEPORT`, if Dracula apparated back to Castle Dracula.
 */
location_t gv_get_location (GameView gv, enum player player);

/**
 * Fills the trail array with the locations of the last 6 turns for the
 * given player.
 *
 * If the move does not exist yet (i.e., the start of the game),
 * the value should be UNKNOWN_LOCATION (-1).
 *
 * For example after 2 turns the array may have the contents
 *
 *     {29, 12, -1, -1, -1, -1}
 *
 * This would mean in the first move the player started on location 12
 * then moved to the current location of 29.
 */
void gv_get_history (
	GameView gv, enum player player, location_t trail[TRAIL_SIZE]);

/**
 * Return an array of `location_t`s giving all of the locations that the
 * given `player` could reach from their current location, assuming it's
 * currently `round`.
 *
 * The array can be in any order but must contain unique entries.
 * The array size is stored at the variable referenced by `n_locations`.
 * The player's current location should be included in the array.
 *
 * `road`, `rail`, and `sea` connections should only be considered
 * if the `road`, `rail`, `sea` parameters are true, respectively.
 *
 * The function must take into account the current round and player for
 * determining how far `player` can travel by rail.
 *
 * When `player` is `PLAYER_DRACULA`, the function must take into
 * account (many of) the rules around Dracula's movements, such as that
 * Dracula may not go to the hospital, and may not travel by rail.
 * It need not take into account the trail restriction.
 */
location_t *gv_get_connections (
	GameView gv, size_t *n_locations,
	location_t from,
	enum player player, round_t round,
	bool road, bool rail, bool sea);

#endif // !defined (FOD__GAME_VIEW_H_)
