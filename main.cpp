#include <SDL2/SDL.h>
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_hints.h>
#include <SDL2/SDL_keyboard.h>
#include <SDL2/SDL_keycode.h>
#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_scancode.h>
#include <SDL2/SDL_surface.h>
#include <SDL2/SDL_timer.h>
#include <algorithm>
#include <iostream>
#include <vector>

#define FPS 60
int main() {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        std::cerr << "SDL_Init Error: " << SDL_GetError() << "\n";
        return 1;
    }

    SDL_Window* window = SDL_CreateWindow("Simple SDL Window",
                                          SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                          800, 600, SDL_WINDOW_SHOWN);
    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "2");
    if (!window) {
        std::cerr << "SDL_CreateWindow Error: " << SDL_GetError() << "\n";
        SDL_Quit();
        return 1;
    }

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        std::cerr << "SDL_CreateRenderer Error: " << SDL_GetError() << "\n";
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    // THIS IS FOR THE PLAYER
    SDL_Surface* image = SDL_LoadBMP("../assets/1.bmp");
    if (!image) {
        std::cerr << "SDL_LoadBMP Error: " << SDL_GetError() << '\n';
        SDL_DestroyRenderer(renderer); SDL_DestroyWindow(window); SDL_Quit(); return 1;
    }
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer,image);
    SDL_FreeSurface(image);
    SDL_Rect rect = {800/2 - 50,520,50,50};


    // THIS IS FOR THE ENEMIES
    SDL_Surface* enemy_sprite = SDL_LoadBMP("../assets/2.bmp");
    if (!enemy_sprite) {
        std::cerr << "SDL_LoadBMP Error: " << SDL_GetError() << '\n';
        SDL_DestroyRenderer(renderer); SDL_DestroyWindow(window); SDL_Quit(); return 1;
    }
    SDL_Texture* enemy_texture = SDL_CreateTextureFromSurface(renderer,enemy_sprite);
    SDL_FreeSurface(enemy_sprite);
    struct Enemy{
        SDL_Rect rect;
        bool isAlive = true;
    };

    std::vector<SDL_Rect> enemies;
    for(int i=0;i<5;i++){
        for(int j=0;j<10;j++){
            SDL_Rect enemy_rect = {50,50,50,50};
            enemy_rect.x += 0 + j * (60);
            enemy_rect.y += 0 + i * (60);
            enemies.push_back(enemy_rect);
        }
    }

    //THIS IS FOR THE BULLET
    SDL_Surface* bullet_sprite = SDL_LoadBMP("../assets/3.bmp");
    if (!bullet_sprite) {
        std::cerr << "SDL_LoadBMP Error: " << SDL_GetError() << '\n';
        SDL_DestroyRenderer(renderer); SDL_DestroyWindow(window); SDL_Quit(); return 1;
    }
    SDL_Texture* bullet_texture = SDL_CreateTextureFromSurface(renderer,enemy_sprite);
    SDL_FreeSurface(bullet_sprite);

    bool running = true;
    int direction = 1;
    int elapsed = 0;
    std::vector<SDL_Rect> bullets;
    bool bullet_created = false;
    SDL_Rect bullet_rect;

    SDL_Event event;
    while (running) {
        elapsed = SDL_GetTicks();
        unsigned int frame_time = 0;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
            }
            const auto key_state = SDL_GetKeyboardState(NULL);
            if(key_state[SDL_SCANCODE_D] && rect.x < 750){
                rect.x += 10;
            }
            if(key_state[SDL_SCANCODE_A] && rect.x > 0){
                rect.x -= 10;
            }
            if(key_state[SDL_SCANCODE_SPACE] &&  rect.x > 0){
                bullet_rect =  {rect.x + 25,rect.y - 30,10,20};
                bullets.push_back(bullet_rect);
            }
        }
        SDL_SetRenderDrawColor(renderer, 0,0,0, 255);
        SDL_RenderCopy(renderer, texture,NULL, &rect);
        bool change_dir = false;
        for(auto& en : enemies){
            if((en.x + 50 >= 800 && direction == 1) || (en.x <= 0 && direction == -1)){
                change_dir = true;
                direction *= -1;
                break;
            }
        }

        for(auto& en : enemies){
            en.x += 5 * direction;
        }
        for(auto& x: enemies){
            SDL_RenderCopy(renderer, enemy_texture,NULL, &x);
        }
        for(auto& bullet: bullets){
            SDL_RenderCopy(renderer, bullet_texture,NULL, &bullet);
        }
        for(auto& bullet: bullets){
            for(auto& enemy : enemies){
                if(SDL_HasIntersection(&enemy, &bullet)){
                }
            }
            bullet.y -= 10;
        }

        SDL_RenderPresent(renderer);
        SDL_RenderClear(renderer);
        frame_time = SDL_GetTicks() - elapsed;
        if(1000/FPS > frame_time){
            SDL_Delay((1000/FPS) - frame_time);
        }
    }
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}

