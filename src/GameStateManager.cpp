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
    while (!GameStates.empty()) {
        delete GameStates.back();
        GameStates.pop_back();
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
    State->SetGameStateManager(this);
    if (!GameStates.empty()) {
        GameStates.back()->Pause();
    }
    GameStates.push_back(State);
}

void GameStateManager::PopState() {
    if (!GameStates.empty()) {
        GameState* back = GameStates.back();
        GameStates.pop_back();
        delete back;
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
