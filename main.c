#include "metadata.h"

int move_player(player *p, SDL_Event *key_presed) {
    int next;
    if (CHECK_BEAST_COLLISION(p)) {
        if (!p->can_kill_beast) {
        map[p->pos.y][p->pos.x].flags ^= (1 << 7);
        map[p->pos.y][p->pos.x].dropped_treasure += p->money;
        p->deaths += 1;
        p->pos = game_info.campsite_pos;
        p->money = 0;
        }
        game_info.beast.is_dead = 1;
    }
    switch (key_presed->key.keysym.sym) {
        case SDLK_w:
            next = p->pos.y - 1;
            if (!(map[next][p->pos.x].flags & 0x01)) {
                return 0;
            } else if (map[next][p->pos.x].flags & 0x10) {
                p->money++;
                map[next][p->pos.x].flags &= ~(1 << 4);
            } else if (map[next][p->pos.x].flags & 0x20) {
                p->money += 10;
                map[next][p->pos.x].flags &= ~(1 << 5);
            } else if (map[next][p->pos.x].flags & 0x40) {
                p->money += 50;
                map[next][p->pos.x].flags &= ~(1 << 6);
            } else if (map[next][p->pos.x].flags & 0x80) {
                p->money += map[next][p->pos.x].dropped_treasure;
                map[next][p->pos.x].flags &= ~(1 << 7);
            } else if (map[next][p->pos.x].flags & 0x04) {
                p->bank += p->money;
                p->money = 0;
            }
            p->pos.y--;
            return 1;
        case SDLK_s:
            next = p->pos.y + 1;
            if (!(map[next][p->pos.x].flags & 0x01)) {
                return 0;
            } else if (map[next][p->pos.x].flags & 0x10) {
                p->money++;
                map[next][p->pos.x].flags &= ~(1 << 4);
            } else if (map[next][p->pos.x].flags & 0x20) {
                p->money += 10;
                map[next][p->pos.x].flags &= ~(1 << 5);
            } else if (map[next][p->pos.x].flags & 0x40) {
                p->money += 50;
                map[next][p->pos.x].flags &= ~(1 << 6);
            } else if (map[next][p->pos.x].flags & 0x80) {
                p->money += map[next][p->pos.x].dropped_treasure;
                map[next][p->pos.x].flags &= ~(1 << 7);
            } else if (map[next][p->pos.x].flags & 0x04) {
                p->bank += p->money;
                p->money = 0;
            }
            p->pos.y++;
            return 1;
        case SDLK_a:
            next = p->pos.x - 1;
            if (!(map[p->pos.y][next].flags & 0x01)) {
                return 0;
            } else if (map[p->pos.y][next].flags & 0x10) {
                p->money++;
                map[p->pos.y][next].flags &= ~(1 << 4);
            } else if (map[p->pos.y][next].flags & 0x20) {
                p->money += 10;
                map[p->pos.y][next].flags &= ~(1 << 5);
            } else if (map[p->pos.y][next].flags & 0x40) {
                p->money += 50;
                map[p->pos.y][next].flags &= ~(1 << 6);
            } else if (map[p->pos.y][next].flags & 0x80) {
                p->money += map[p->pos.y][next].dropped_treasure;
                map[p->pos.y][next].flags &= ~(1 << 7);
            } else if (map[p->pos.y][next].flags & 0x04) {
                p->bank += p->money;
                p->money = 0;
            }
            p->pos.x--;
            return 1;
        case SDLK_d:
            next = p->pos.x + 1;
            if (!(map[p->pos.y][next].flags & 0x01)) {
                return 0;
            } else if (map[p->pos.y][next].flags & 0x10) {
                p->money++;
                map[p->pos.y][next].flags &= ~(1 << 4);
            } else if (map[p->pos.y][next].flags & 0x20) {
                p->money += 10;
                map[p->pos.y][next].flags &= ~(1 << 5);
            } else if (map[p->pos.y][next].flags & 0x40) {
                p->money += 50;
                map[p->pos.y][next].flags &= ~(1 << 6);
            } else if (map[p->pos.y][next].flags & 0x80) {
                p->money += map[p->pos.y][next].dropped_treasure;
                map[p->pos.y][next].flags &= ~(1 << 7);
            } else if (map[p->pos.y][next].flags & 0x04) {
                p->bank += p->money;
                p->money = 0;
            }
            p->pos.x++;
            return 1;
        default:
            return 0;
    }
}

int load_map(char *filename) {
    FILE *fp = fopen(filename, "rb");
    if (!fp) {
        return -1;
    }
    return fread(map, sizeof(pixel), LAB_HEIGHT * LAB_WIDTH, fp);
}

int field_of_view() {
    ///Lookin down
    for (int i = 0; i < LAB_HEIGHT - game_info.beast.pos.y; ++i) {
        if (!(map[game_info.beast.pos.y + i][game_info.beast.pos.x].flags & 0x01) || !CHECK_PLAYER_COORDINATE(x)) {
            break;
        }
        SDL_Point observed = game_info.beast.pos;
        observed.y += i;
        if (CHECK_PLAYER_SEEN_COORDINATE(observed, y)) {
            game_info.beast.dest = observed;
            game_info.beast.preferred_direction = DOWN;
            return 1;
        }
    }
    ///Lookin up
    for (int i = 0; i < game_info.beast.pos.y; ++i) {
        if (!(map[game_info.beast.pos.y - i][game_info.beast.pos.x].flags & 0x01) || !CHECK_PLAYER_COORDINATE(x)) {
            break;
        }
        SDL_Point observed = game_info.beast.pos;
        observed.y -= i;
        if (CHECK_PLAYER_SEEN_COORDINATE(observed, y)) {
            game_info.beast.dest = observed;
            game_info.beast.preferred_direction = UP;
            return 1;
        }
    }
    ///Lookin right
    for (int i = 0; i < LAB_WIDTH - game_info.beast.pos.x; ++i) {
        if (!(map[game_info.beast.pos.y][game_info.beast.pos.x + i].flags & 0x01) || !CHECK_PLAYER_COORDINATE(y)) {
            break;
        }
        SDL_Point observed = game_info.beast.pos;
        observed.x += i;
        if (CHECK_PLAYER_SEEN_COORDINATE(observed, x)) {
            game_info.beast.dest = observed;
            game_info.beast.preferred_direction = RIGHT;
            return 1;
        }
    }
    ///Lookin left
    for (int i = 0; i < game_info.beast.pos.x; ++i) {
        if (!(map[game_info.beast.pos.y][game_info.beast.pos.x - i].flags & 0x01) || !CHECK_PLAYER_COORDINATE(y)) {
            break;
        }
        SDL_Point observed = game_info.beast.pos;
        observed.x -= i;
        if (CHECK_PLAYER_SEEN_COORDINATE(observed, x)) {
            game_info.beast.dest = observed;
            game_info.beast.preferred_direction = LEFT;
            return 1;
        }
    }
    return 0;
}

void move_beast() {
    if (game_info.beast.wandering) {

    } else {
        if (!field_of_view()) {
            return;
        }
        game_info.beast.wandering = 1;
    }
    switch (game_info.beast.preferred_direction) {
        case UP:
            game_info.beast.pos.y--;
            break;
        case DOWN:
            game_info.beast.pos.y++;
            break;
        case RIGHT:
            game_info.beast.pos.x++;
            break;
        case LEFT:
            game_info.beast.pos.x--;
            break;
        default:
            return;
    }
    if (game_info.beast.pos.x == game_info.beast.dest.x && game_info.beast.pos.y == game_info.beast.dest.y) {
        game_info.beast.wandering = 0;
        game_info.beast.preferred_direction = NONE;
    }
}


player *init_game(int choice) {
    game_info.treasures_remaining = 0;

    for (int i = 0; i < LAB_HEIGHT; ++i) {
        for (int j = 0; j < LAB_WIDTH; ++j) {
            if (map[i][j].flags & 0x10 || map[i][j].flags & 0x20 || map[i][j].flags & 0x40) {
                game_info.treasures_remaining++;
            }
            if (map[i][j].flags & 0x04) {
                game_info.campsite_pos.y = i;
                game_info.campsite_pos.x = j;
            }
            if (map[i][j].flags & 0x08) {
                game_info.beast.pos.y = i;
                game_info.beast.pos.x = j;
            }
        }
    }

    game_info.player1.is_active = 0;
    game_info.player2.is_active = 0;
    game_info.player3.is_active = 0;
    game_info.player4.is_active = 0;
    switch (choice) {
        case P1:
            game_info.player1.is_active = 1;
            game_info.player1.bank = 0;
            game_info.player1.deaths = 0;
            game_info.player1.money = 0;
            game_info.player1.can_kill_beast = 0;
            game_info.player1.pos = game_info.campsite_pos;
            return &game_info.player1;
        case P2:
            game_info.player2.is_active = 1;
            game_info.player2.bank = 0;
            game_info.player2.deaths = 0;
            game_info.player2.money = 0;
            game_info.player2.can_kill_beast = 0;
            game_info.player2.pos = game_info.campsite_pos;
            return &game_info.player2;
        case P3:
            game_info.player3.is_active = 1;
            game_info.player3.bank = 0;
            game_info.player3.deaths = 0;
            game_info.player3.money = 0;
            game_info.player3.can_kill_beast = 0;
            game_info.player3.pos = game_info.campsite_pos;
            return &game_info.player3;
        case P4:
            game_info.player4.is_active = 1;
            game_info.player4.bank = 0;
            game_info.player4.deaths = 0;
            game_info.player4.money = 0;
            game_info.player4.can_kill_beast = 0;
            game_info.player4.pos = game_info.campsite_pos;
            return &game_info.player4;
        default:
            return 0;

    }
}

///Layer of abstraction to enable future player input from client process
int listen_player_input(player* current) {
    if (event.type == SDL_TEXTINPUT) move_player(current, &event);
}

///Function to extend duration of a turn to 0.2 seconds
int frame_rate(void* data) {
   usleep(MS_IN_SECOND * 0.1);
}

int main(int argc, char *argv[]) {
    ///Boilerplate
    SDL_Window *window = SDL_CreateWindow("Runner", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                          WIDTH + STAT_WIDTH, HEIGHT, (SDL_WINDOW_OPENGL + SDL_WINDOW_FULLSCREEN * 0));
    SDL_Renderer *r = SDL_CreateRenderer(window, -1, (SDL_RENDERER_ACCELERATED));
    TTF_Init();
    SDL_Thread* frame_rate_thread;

    ///RESOURCE LOADING
    load_map("Resource/map.bin");
    SDL_Texture *bricks = SDL_CreateTextureFromSurface(r, SDL_LoadBMP("Resource/wall.bmp"));
    SDL_Texture *bricks_background = SDL_CreateTextureFromSurface(r, SDL_LoadBMP("Resource/background.bmp"));
    SDL_Texture *treasure = SDL_CreateTextureFromSurface(r, SDL_LoadBMP("Resource/coin.bmp"));
    SDL_Texture *drop = SDL_CreateTextureFromSurface(r, SDL_LoadBMP("Resource/drop.bmp"));
    SDL_Texture *campsite = SDL_CreateTextureFromSurface(r, SDL_LoadBMP("Resource/campsite.bmp"));
    SDL_Texture *beast = SDL_CreateTextureFromSurface(r, SDL_LoadBMP("Resource/beast.bmp"));
    SDL_Texture *bush = SDL_CreateTextureFromSurface(r, SDL_LoadBMP("Resource/bbush.bmp"));
    SDL_Texture *p1texture = SDL_CreateTextureFromSurface(r, SDL_LoadBMP("Resource/p1texture.bmp"));
    SDL_Texture *p2texture = SDL_CreateTextureFromSurface(r, SDL_LoadBMP("Resource/p2texture.bmp"));
    SDL_Texture *p3texture = SDL_CreateTextureFromSurface(r, SDL_LoadBMP("Resource/p3texture.bmp"));
    SDL_Texture *p4texture = SDL_CreateTextureFromSurface(r, SDL_LoadBMP("Resource/p4texture.bmp"));
    TTF_Font *font = TTF_OpenFont("Resource/sans.ttf", 15);
    if (!font) {
        printf("TTF_OpenFont: %s\n", TTF_GetError());
        return 228;
    }

    ///Stat screen
    SDL_Color text_color = {255, 255, 255};
    SDL_Surface *beast_surface = TTF_RenderText_Solid(font, "Wild beast", text_color);
    SDL_Texture *beast_message = SDL_CreateTextureFromSurface(r, beast_surface);
    SDL_Rect beast_message_container;
    beast_message_container.x = WIDTH + STAT_WIDTH/4;
    beast_message_container.y = 0;
    beast_message_container.w = 3 * STAT_WIDTH/4;
    beast_message_container.h = TEXT_HEIGHT;
    SDL_Rect beast_descriptor;
    beast_descriptor.x = WIDTH + STAT_WIDTH/20;
    beast_descriptor.y = 0;
    beast_descriptor.w = PIXEL_SIZE*4;
    beast_descriptor.h = PIXEL_SIZE*4;

    SDL_Surface *treasure_surface = TTF_RenderText_Solid(font, "Treasure", text_color);
    SDL_Texture *treasure_message = SDL_CreateTextureFromSurface(r, treasure_surface);
    SDL_Rect treasure_message_container;
    treasure_message_container.x = WIDTH + STAT_WIDTH/4;
    treasure_message_container.y = beast_message_container.y + TEXT_HEIGHT* 4/3;
    treasure_message_container.w = 3 * STAT_WIDTH/4;
    treasure_message_container.h = TEXT_HEIGHT;
    SDL_Rect treasure_descriptor;
    treasure_descriptor.x = WIDTH + STAT_WIDTH/20;
    treasure_descriptor.y = treasure_message_container.y;
    treasure_descriptor.w = PIXEL_SIZE*4;
    treasure_descriptor.h = PIXEL_SIZE*4;

    SDL_Surface *drop_surface = TTF_RenderText_Solid(font, "Dropped treasure", text_color);
    SDL_Texture *drop_message = SDL_CreateTextureFromSurface(r, drop_surface);
    SDL_Rect drop_message_container;
    drop_message_container.x = WIDTH + STAT_WIDTH/4;
    drop_message_container.y = treasure_message_container.y + TEXT_HEIGHT* 4/3;
    drop_message_container.w = 3 * STAT_WIDTH/4;
    drop_message_container.h = TEXT_HEIGHT;
    SDL_Rect drop_descriptor;
    drop_descriptor.x = WIDTH + STAT_WIDTH/20;
    drop_descriptor.y = drop_message_container.y;
    drop_descriptor.w = PIXEL_SIZE*4;
    drop_descriptor.h = PIXEL_SIZE*4;


    SDL_Rect stats1_message_container;
    stats1_message_container.x = WIDTH;
    stats1_message_container.y = drop_message_container.y + TEXT_HEIGHT* 3;
    stats1_message_container.w = 3 * STAT_WIDTH/4;
    stats1_message_container.h = TEXT_HEIGHT;

    SDL_Rect stats2_message_container;
    stats2_message_container.x = WIDTH;
    stats2_message_container.y = stats1_message_container.y + TEXT_HEIGHT;
    stats2_message_container.w = 3 * STAT_WIDTH/4;
    stats2_message_container.h = TEXT_HEIGHT;


    ///Init
    int choice = 0;

    player *current = init_game(choice);
    game_info.player1.texture = p1texture;
    game_info.player2.texture = p2texture;
    game_info.player3.texture = p3texture;
    game_info.player4.texture = p4texture;

    while (current->bank < 500) {
        frame_rate_thread = SDL_CreateThread(frame_rate, "fr", 0);
        SDL_RenderClear(r);

        ///Static text&stat screen image rendering
        SDL_RenderCopy(r, beast_message, 0, &beast_message_container);
        SDL_RenderCopy(r, beast, 0, &beast_descriptor);
        SDL_RenderCopy(r, treasure_message, 0, &treasure_message_container);
        SDL_RenderCopy(r, treasure, 0, &treasure_descriptor);
        SDL_RenderCopy(r, drop_message, 0, &drop_message_container);
        SDL_RenderCopy(r, drop, 0, &drop_descriptor);

        sprintf(stats, "Collected treasure: %zu", current->money);

        SDL_Surface *stat_surface = TTF_RenderText_Solid(font, stats, text_color);
        SDL_Texture *stat_message = SDL_CreateTextureFromSurface(r, stat_surface);
        SDL_RenderCopy(r, stat_message, 0, &stats1_message_container);
        SDL_DestroyTexture(stat_message);
        SDL_FreeSurface(stat_surface);

        sprintf(stats, "Treasure in bank: %zu", current->bank);
        stat_surface = TTF_RenderText_Solid(font, stats, text_color);
        stat_message = SDL_CreateTextureFromSurface(r, stat_surface);
        SDL_RenderCopy(r, stat_message, 0, &stats2_message_container);
        SDL_DestroyTexture(stat_message);
        SDL_FreeSurface(stat_surface);

        for (int i = 0; i < LAB_HEIGHT; ++i) {
            for (int j = 0; j < LAB_WIDTH; ++j) {
                SDL_Rect boundaries = {0, 0, 0, 0};
                boundaries.y = j * PIXEL_SIZE;
                boundaries.x = i * PIXEL_SIZE;
                boundaries.h = PIXEL_SIZE;
                boundaries.w = PIXEL_SIZE;
                if (map[j][i].flags & 0x01) {
                    SDL_RenderCopy(r, bricks_background, 0, &boundaries);
                    if (map[j][i].flags & 0x10) {
                        boundaries.h = PIXEL_SIZE / 2;
                        boundaries.w = PIXEL_SIZE / 2;
                        boundaries.y = boundaries.y + PIXEL_SIZE / 4;
                        boundaries.x = boundaries.x + PIXEL_SIZE / 4;
                        SDL_RenderCopy(r, treasure, 0, &boundaries);
                    } else if (map[j][i].flags & 0x20) {
                        boundaries.h = PIXEL_SIZE / 1.5;
                        boundaries.w = PIXEL_SIZE / 1.5;
                        boundaries.y = boundaries.y + PIXEL_SIZE / 3;
                        boundaries.x = boundaries.x + PIXEL_SIZE / 3;
                        SDL_RenderCopy(r, treasure, 0, &boundaries);
                    } else if (map[j][i].flags & 0x40) {
                        SDL_RenderCopy(r, treasure, 0, &boundaries);
                    } else if (map[j][i].flags & 0x80) {
                        SDL_RenderCopy(r, drop, 0, &boundaries);
                    } else if (map[j][i].flags & 0x04) {
                        SDL_RenderCopy(r, campsite, 0, &boundaries);
                    } else if (map[j][i].flags & 0x02) {
                        SDL_RenderCopy(r, bush, 0, &boundaries);
                    }
                } else {
                    SDL_RenderCopy(r, bricks, 0, &boundaries);
                }
            }
        }

        SDL_Rect boundaries = {0, 0, 0, 0};
        ///Beast rendering
        boundaries.y = game_info.beast.pos.y * PIXEL_SIZE;
        boundaries.x = game_info.beast.pos.x * PIXEL_SIZE;
        boundaries.h = PIXEL_SIZE;
        boundaries.w = PIXEL_SIZE;
        SDL_RenderCopy(r, beast, 0, &boundaries);

        ///Player rendering
        if (game_info.player1.is_active) {
            boundaries.y = game_info.player1.pos.y * PIXEL_SIZE;
            boundaries.x = game_info.player1.pos.x * PIXEL_SIZE;
            boundaries.h = PIXEL_SIZE;
            boundaries.w = PIXEL_SIZE;
            SDL_RenderCopy(r, game_info.player1.texture, 0, &boundaries);
        }


        if (game_info.player2.is_active) {
            boundaries.y = game_info.player2.pos.y * PIXEL_SIZE;
            boundaries.x = game_info.player2.pos.x * PIXEL_SIZE;
            boundaries.h = PIXEL_SIZE;
            boundaries.w = PIXEL_SIZE;
            SDL_RenderCopy(r, game_info.player2.texture, 0, &boundaries);
        }


        if (game_info.player3.is_active) {
            boundaries.y = game_info.player3.pos.y * PIXEL_SIZE;
            boundaries.x = game_info.player3.pos.x * PIXEL_SIZE;
            boundaries.h = PIXEL_SIZE;
            boundaries.w = PIXEL_SIZE;
            SDL_RenderCopy(r, game_info.player3.texture, 0, &boundaries);
        }


        if (game_info.player4.is_active) {
            boundaries.y = game_info.player4.pos.y * PIXEL_SIZE;
            boundaries.x = game_info.player4.pos.x * PIXEL_SIZE;
            boundaries.h = PIXEL_SIZE;
            boundaries.w = PIXEL_SIZE;
            SDL_RenderCopy(r, game_info.player4.texture, 0, &boundaries);
        }

        SDL_RenderPresent(r);

        SDL_PollEvent(&event);
        move_beast();
        listen_player_input(current);

        if (event.type == SDL_QUIT || event.key.keysym.sym == SDLK_ESCAPE) {
            return 0;
        }
        SDL_WaitThread(frame_rate_thread, 0);
    }

    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(r);
    return 0;
}
