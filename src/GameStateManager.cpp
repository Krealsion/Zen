#include "GameStateManager.h"

#include "GameState.h"
#include "Timer.h"

GameStateManager::GameStateManager(GameState* InitialState) {
    PushState(InitialState);
    GameRunning = true;
    UpdateThread = std::thread([&]() {
        while (IsRunning())
            Update();
    });
    DrawThread = std::thread([&]() {
        while (IsRunning())
            Draw();
    });
    UpdateThread.join();
    DrawThread.join();
}

GameStateManager::~GameStateManager() {
    for (int i = 0; i < GameStates.size(); i++) {
        delete[] GameStates[i];
    }
}

void GameStateManager::Update() {
    Timer::UpdateTime();
    if (!GameStates.empty()) {
        GameStates.back()->Update();
    }
}

void GameStateManager::Draw() {
    if (!GameStates.empty()) {
        GameStates.back()->Draw(Graphics);
    }
}

void GameStateManager::PushState(GameState* State) {
    if (!GameStates.empty()) {
        GameStates.back()->Pause();
    }
    GameStates.push_back(State);
}

void GameStateManager::PopState() {
    if (!GameStates.empty()) {
        GameStates.pop_back();
        if (!GameStates.empty()) {
            GameStates.back()->Resume();
        }
    }
}

bool GameStateManager::IsRunning() {
    return GameRunning;
}

void GameStateManager::Exit() {
    GameRunning = false;
}
