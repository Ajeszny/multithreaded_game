#ifndef RUNNER_METADATA_H
#define RUNNER_METADATA_H
#include <stdio.h>
#include <SDL.h>
#include <SDL_ttf.h>
#include <pthread.h>
#include <unistd.h>
#include <SDL2/SDL_net.h>
#define LAB_WIDTH 50
#define LAB_HEIGHT 50
#define PIXEL_SIZE 16
#define WIDTH LAB_WIDTH*PIXEL_SIZE
#define HEIGHT LAB_HEIGHT*PIXEL_SIZE
#define STAT_WIDTH (WIDTH/2)
#define TEXT_HEIGHT 60
#define MS_IN_SECOND 1000 * 1000
#define STAT_HEIGHT HEIGHT

char stats[99] = {0};
SDL_Event event;

enum players {
    P1, P2, P3, P4
};

typedef struct pixel_t {
    ///0x01 - not wall, 0x02 - bush, 0x04 - campsite, 0x08 - beast spawn
    /// 0x10 - booty, 0x20 - large booty, 0x40 - giant booty, 0x80 - drop
    ///Size is 32 cuz padding
    uint32_t flags;
    ///Is zero except when (flags & 0x80)
    size_t dropped_treasure;

} pixel;

typedef struct player_t {
    SDL_Point pos;
    size_t money;
    size_t bank;
    size_t deaths;
    SDL_bool is_active;
    SDL_Texture* texture;
    SDL_bool can_kill_beast;
} player;

enum direction {
    UP,//y--
    DOWN,//y++
    RIGHT,//x++
    LEFT,//x--
    NONE,//No movement
};

typedef struct beast_t {
    SDL_Point pos;
    SDL_Point dest;
    _Bool wandering;//Continue to move even when no player is in fov
    enum direction preferred_direction;
    SDL_bool is_dead;
} beast;

struct gameinfo_t {
    beast beast;
    player player1;
    player player2;
    player player3;
    player player4;
    int treasures_remaining;
    SDL_Point campsite_pos;

} game_info;\

struct player_name_t {
    char name[99];
    SDL_Color c;
};

#define CHECK_PLAYER_COORDINATE(N) \
(game_info.beast.pos.N ==      \
game_info.player1.pos.N||      \
game_info.beast.pos.N ==      \
game_info.player2.pos.N||        \
game_info.beast.pos.N ==      \
game_info.player3.pos.N||        \
game_info.beast.pos.N ==      \
game_info.player4.pos.N)

#define CHECK_PLAYER_SEEN_COORDINATE(observed, N) \
(observed.N ==      \
game_info.player1.pos.N||      \
observed.N ==      \
game_info.player2.pos.N||        \
observed.N ==      \
game_info.player3.pos.N||        \
observed.N ==      \
game_info.player4.pos.N)

#define CHECK_BEAST_COLLISION(player) \
(player->pos.y == game_info.beast.pos.y&&player->pos.x == game_info.beast.pos.x)

pixel map[LAB_HEIGHT][LAB_WIDTH];
#endif
