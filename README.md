# BrushPhone

## Overview

BrushPhone is a multiplayer LAN game inspired by [GarticPhone](https://garticphone.com/pt), developed in C++. Players take turns drawing and guessing.

## Features

- **LAN Multiplayer Support**
- **Lobby System** 
  - **Host Creation**: A player can create a lobby, generating a unique code for others to join.
  - **Join by Code**: Players can join existing lobbies using the provided code.
- **Ready System**: The game starts automatically once all players in the lobby are marked as "ready."
- **In-Lobby Chat**
- **Drawing & Guessing**: Take turns drawing and guessing words.
- **Screen Navigation System**
- **Pure C++ Implementation**: The entire project is written in C++, without using Qt’s specific UI language (QML).

## Components

- **Language**: C++
- **Framework**: Qt
- **Networking**: LAN-based multiplayer

## Game

![Game](Assets/game.gif)

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.
