#include "SDL.h"

#pragma region SDL

void write(SDL_Renderer* _renderer, string _text, int _x, int _y) {
    SDL_Color color = { 153, 255, 153, 255 };
    const char* t = _text.c_str();
    TTF_Font* font = TTF_OpenFont("../../../../resources/fonts.ttf", FONT_SIZE);
    if (font != NULL) {
        SDL_Surface* surface = TTF_RenderText_Solid(font, t, color);
        SDL_Texture* texture = SDL_CreateTextureFromSurface(_renderer, surface);
        if (surface != NULL) {
            SDL_Rect score_board;
            score_board.w = surface->w;
            score_board.h = surface->h;
            score_board.x = _x - score_board.w;
            score_board.y = _y - score_board.h;
            SDL_FreeSurface(surface);
            SDL_RenderCopy(_renderer, texture, NULL, &score_board);
            SDL_DestroyTexture(texture);
        }
        else printf("ERROR, no es posible crear una textura del texto renderizado\n");
        TTF_CloseFont(font);
    }

    else printf("ERROR, no se pudo cargar la fuente\n");
}

void gameVisualsController(Match &_match, bool _loaded) {
    Ball ball;
    SDL_Window* newWindow = NULL;
    SDL_Renderer* newRenderer = NULL;
    SDL_Init(SDL_INIT_EVERYTHING);
    bool running = true;
    SDL_Event event;
    SDL_CreateWindowAndRenderer(1000, 600, 0, &newWindow, &newRenderer);
    TTF_Init();

    int width = SDL_GetWindowSurface(newWindow)->w;
    int height = SDL_GetWindowSurface(newWindow)->h;

    //Posición inicial jugador 1
    _match.player1->setPlayerRadius();
    _match.player1->setPlayerPosition(width / 4, height - _match.player1->getR());
    //Posición inicial jugador 2
    _match.player2->setPlayerRadius();
    _match.player2->setPlayerPosition(width - (width / 4), height - _match.player2->getR());

    //Posición inicial pelota
    ball.setBallPosition(newWindow);
    ball.setBallRadius();

    int dX = 2;
    int dY = 2;

    if (_match.player1->getServe() && dX >= 0) {
        dX *= -1;
    }
    else if (!_match.player1->getServe() && dX <= 0) {
        dX *= -1;
    }
    while (running) {
        //Se pone el color de la pantalla en blanco:
        SDL_SetRenderDrawColor(newRenderer, 255, 255, 255, 255);
        SDL_RenderClear(newRenderer);

        //Dibujar un rectángulo relleno:
        SDL_SetRenderDrawColor(newRenderer, 107, 53, 107, 255);
        SDL_Rect rectangle;
        rectangle.w = 100;
        rectangle.h = 300;
        rectangle.y = height - rectangle.h;
        rectangle.x = (width / 2) - (rectangle.w / 2);
        SDL_RenderFillRect(newRenderer, &rectangle);

        //Inputs del juego para mover los jugadores
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
            }
            if (event.type == SDL_KEYDOWN) {
                switch (event.key.keysym.sym)
                {
                    //Mover izquierda y derecha el jugador 1:
                case SDLK_d:
                    if (_match.player1->getX() + _match.player1->getR() <= rectangle.x) {
                        _match.player1->addX(6);
                    }
                    break;
                case SDLK_a:
                    if (_match.player1->getX() - _match.player1->getR() >= 0) {
                        _match.player1->substractX(6);
                    }
                    break;
                    //Salto jugador 1
                case SDLK_w:
                    if (_match.player1->getCanJump() && _match.player1->getY() + _match.player1->getR() == height) {
                        _match.player1->substractY(150);
                        _match.player1->setCanJump(false);
                    }
                    break;
                    //Mover izquierda y derecha el jugador 2: 
                case SDLK_l:
                    if (_match.player2->getX() + _match.player2->getR() <= width) {
                        _match.player2->addX(6);
                    }
                    break;
                case SDLK_j:
                    if (_match.player2->getX() - _match.player2->getR() >= rectangle.x + rectangle.w) {
                        _match.player2->substractX(6);
                    }
                    break;
                    //Salto jugador 2
                case SDLK_i:
                    if (_match.player2->getCanJump() && _match.player2->getY() + _match.player2->getR() == height) {
                        _match.player2->substractY(150);
                        _match.player2->setCanJump(false);
                    }
                    break;
                }
            }
        }
        //Dibujar a los dos jugadores como circulos:
        if (!_match.player1->getCanJump() || _match.player1->getY() + _match.player1->getR() < height) {
            _match.player1->jumpUpdate();
        }
        if (!_match.player2->getCanJump() || _match.player2->getY() + _match.player2->getR() < height) {
            _match.player2->jumpUpdate();
        }
        _match.player1->drawPlayers(newRenderer);
        _match.player2->drawPlayers(newRenderer);

#pragma region Movimiento de la pelota
        //Dibujar y mover la pelota:
        if (ball.getY() - ball.getR() <= 0) {
            dY *= -1;
        }
        if (ball.getX() + ball.getR() > width || ball.getX() - ball.getR() <= 0) {
            dX *= -1;
        }
        if (ball.getY() + ball.getR() > rectangle.y && ball.getX() + ball.getR() >= rectangle.x && ball.getX() - ball.getR() <= rectangle.x + rectangle.w) {
            dX *= -1;
            dY *= -1;
        }
        //Colisión de la pelota con los jugadores
        if (ball.getY() + ball.getR() >= _match.player1->getY() - _match.player1->getR() && ball.getX() + ball.getR() >= _match.player1->getX() - _match.player1->getR() && ball.getX() - ball.getR() <= _match.player1->getX() + _match.player1->getR()) {
            dY *= -1;
            dX *= -1;
        }
        if (ball.getY() + ball.getR() >= _match.player2->getY() - _match.player2->getR() && ball.getX() + ball.getR() >= _match.player2->getX() - _match.player2->getR() && ball.getX() - ball.getR() <= _match.player2->getX() + _match.player2->getR()) {
            dY *= -1;
            dX *= -1;
        }
        //Asignación de puntuación a los jugadores
        if (ball.getY() + ball.getR() >= height && ball.getX() <= rectangle.x) {
            _match.points[1]++;
            //Se marca el bool serve del jugador 2 como true y se establece el del jugador 1 como false
            _match.player1->setServe(false);
            _match.player2->setServe(true);
            //Se reestablece la posición de la pelota
            ball.setBallPosition(newWindow);
            if (dX <= 0) {
                dX *= -1;
            }
        }
        else if (ball.getY() + ball.getR() >= height && ball.getX() >= rectangle.x) {
            _match.points[0]++;
            //Se marca el bool serve del jugador 1 como true y se establece el del jugador 2 como false
            _match.player1->setServe(true);
            _match.player2->setServe(false);
            //Se reestablece la posición de la pelota
            ball.setBallPosition(newWindow);
            if (dX >= 0) {
                dX *= -1;
            }
        }
        ball.addBallX(dX, dY);
        ball.drawBall(newRenderer);
#pragma endregion

        if (_match.points[0] == 5 || _match.points[1] == 5) {
            running = false;
        }
        //Se muestran las puntuaciones por pantalla
        write(newRenderer, to_string(_match.points[0]), width / 4 + FONT_SIZE, FONT_SIZE * 2);
        write(newRenderer, to_string(_match.points[1]), width - (width / 4) + FONT_SIZE, FONT_SIZE * 2);

        _match.matchTime++;

        SDL_RenderPresent(newRenderer);
    }    
    _match.setWinner(_loaded);
    //Se limpia el SDL
    SDL_DestroyRenderer(newRenderer);
    SDL_DestroyWindow(newWindow);
    TTF_Quit();
}
#pragma endregion
