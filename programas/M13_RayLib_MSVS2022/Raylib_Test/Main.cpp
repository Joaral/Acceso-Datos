/*******************************************************************************************
*
*   raylib [core] example - 3d camera mode
*
*   Example complexity rating: [★☆☆☆] 1/4
*
*   Example originally created with raylib 1.0, last time updated with raylib 1.0
*
*   Example licensed under an unmodified zlib/libpng license, which is an OSI-certified,
*   BSD-like license that allows static linking with closed source software
*
*   Copyright (c) 2014-2025 Ramon Santamaria (@raysan5)
*
********************************************************************************************/

#include "raylib.h"

#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include "main.h"

using namespace std;

float curVersion = 0.5;
float minVersion = 0.3;

float fileVersion = -1.0;
string levelTitle = "NULL";

int minSize = 2;
int sizeW = -1;
int sizeH = -1;

int minTexture = 1;
int fileTextures = -1;

char textChar;
string textName;

map<char, string> textureFile;
map<char, Texture2D> textures;

char** level_floor;
char** level_stage;
char** level_collisions;
char** level_objects;





int entierro(char** floor)
{
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 800;
    const int screenHeight = 450;

    InitWindow(screenWidth, screenHeight, "raylib [core] example - 3d camera mode");

    // Define the camera to look into our 3d world
    Camera3D camera = { 0 };
    camera.position = Vector3{ 0.0f, 10.0f, 10.0f };  // Camera position
    camera.target = Vector3{ 0.0f, 0.0f, 0.0f };      // Camera looking at point
    camera.up = Vector3{ 0.0f, 1.0f, 0.0f };          // Camera up vector (rotation towards target)
    camera.fovy = 45.0f;                                // Camera field-of-view Y
    camera.projection = CAMERA_PERSPECTIVE;             // Camera mode type

    float cubeSize = 1.0f;

    float cubeOffsetX = 0.0f;
    if (sizeW % 2 == 0) {
        cubeOffsetX = 0.5f;
    }
    float cubeOffsetZ = 0.0f;
    if (sizeW % 2 == 0) {
        cubeOffsetZ = 0.5f;
    }

    float cubeInitX = -(sizeW / 2) + cubeOffsetX;
    float cubeInitZ = -(sizeH / 2) + cubeOffsetZ;

    Vector3 cubePosition = { cubeInitX, 0.0f, cubeInitZ};

    //CARGAR TEXTURAS
    for (map<char, string>::iterator it = textureFile.begin(); it != textureFile.end(); ++it) {
        Texture2D texture = LoadTexture(it->second.c_str());

        textures.insert({ it->first, texture });
    }
    

    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        // TODO: Update your variables here
        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

        ClearBackground(RAYWHITE);

        BeginMode3D(camera);

        cubePosition = { cubeInitX, 0.0f, cubeInitZ };

        DrawGrid(10, 1.0f);

        for (int h = 0; h < sizeH; h++) {
            for (int w = 0; w < sizeW; w++) {
                if (floor[h][w] != '0') {
                    DrawCube(cubePosition, cubeSize, cubeSize, cubeSize, RED);
                    DrawCubeWires(cubePosition, cubeSize, cubeSize, cubeSize, GREEN);
                }
                cubePosition.x += cubeSize;
            }
            cubePosition.z += cubeSize;
        }

        EndMode3D();

        DrawText("Welcome to the third dimension!", 10, 40, 20, DARKGRAY);

        DrawFPS(10, 10);

        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}


void  print_map(char** map) {
    for (int i = 0; i < sizeH; i++) {
        for (int j = 0; j < sizeW; j++) {
            cout << map[i][j];
        }
        cout << endl;
    }
}

int main()
{
    ifstream levelFile("First_level.erro");

    if (!levelFile.is_open()) {
        cout << "ERROR 1: El archivo no existe" << endl;
        return 1;
    }

    string tmp = "";

    getline(levelFile, tmp, ';');

    if (tmp != "ERRO") {
        cout << "ERROR 2: El archivo no es de formato 'Entierro'" << endl;
        return 2;
    }

    getline(levelFile, tmp, ';');

    fileVersion = stof(tmp);

    if (fileVersion > curVersion || fileVersion < minVersion) {
        cout << "ERROR 3: El archivo tiene una version incompatible con la actual" << endl;
        return 3;
    }

    //Situarnos en la siguiente linea
    getline(levelFile, tmp, '\n');


    getline(levelFile, tmp, ';');


    if (tmp != "TITLE") {
        cout << "ERROR 4: la cabecera del titulo es erronea." << endl;
        return 4;
    }

    getline(levelFile, tmp, ';');

    if (tmp == "NULL") {
        cout << "ERROR 5: No existe nombre de nivel." << endl;
        return 5;
    }

    levelTitle = tmp;

    getline(levelFile, tmp, '\n');

    getline(levelFile, tmp, ';');

    if (tmp != "SIZE") {
        cout << "ERROR 6: la cabecera del tamaño es erronea." << endl;
        return 6;
    }

    getline(levelFile, tmp, ';');

    sizeW = stoi(tmp);

    getline(levelFile, tmp, ';');

    sizeH = stoi(tmp);

    if (sizeW < minSize && sizeH < minSize) {
        cout << "ERROR 7: los tamaños del nivel no supera el minimo." << endl;
        return 7;
    }

    getline(levelFile, tmp, '\n');

    getline(levelFile, tmp, ';');

    if (tmp != "TEXTURES") {
        cout << "ERROR 8: la cabecera de las texturas es erronea." << endl;
        return 8;
    }

    getline(levelFile, tmp, ';');
    fileTextures = stoi(tmp);


    if (stoi(tmp) < minTexture) {
        cout << "ERROR 8.2: El archivo no supera el minimo de texturas." << endl;
        return 8;
    }

    getline(levelFile, tmp, '\n');


    for (int i = 0; i < fileTextures; i++) {

        getline(levelFile, tmp, ';');

        textChar = tmp[0];

        getline(levelFile, tmp, ';');

        textName = tmp;

        cout << textChar << " -> " << textName << endl;

        textureFile.insert({ textChar, textName });

        getline(levelFile, tmp, '\n');

    }

    //FLOOR
    getline(levelFile, tmp, ';');

    if (tmp != "FLOOR") {
        cout << "ERROR 10: Suelo mal formado." << endl;
        return 8;
    }
    cout << tmp << endl;
    getline(levelFile, tmp, '\n');

    level_floor = new char* [sizeH];
    for (int i = 0; i < sizeH; i++) {
        level_floor[i] = new char[sizeW];
    }

    for (int i = 0; i < sizeH; i++) {
        for (int j = 0; j < sizeW; j++) {
            getline(levelFile, tmp, ';');
            textChar = tmp[0];
            level_floor[i][j] = textChar;
        }
        getline(levelFile, tmp, '\n');
    }

    print_map(level_floor);

    //STAGE
    getline(levelFile, tmp, ';');

    if (tmp != "STAGE") {
        cout << "ERROR 10: nivel mal formado." << endl;
        return 8;
    }
    cout << tmp << endl;
    getline(levelFile, tmp, '\n');

    level_stage = new char* [sizeH];
    for (int i = 0; i < sizeH; i++) {
        level_stage[i] = new char[sizeW];
    }

    for (int i = 0; i < sizeH; i++) {
        for (int j = 0; j < sizeW; j++) {
            getline(levelFile, tmp, ';');
            textChar = tmp[0];
            level_stage[i][j] = textChar;
        }
        getline(levelFile, tmp, '\n');
    }

    print_map(level_stage);
    //COLLISIONS
    getline(levelFile, tmp, ';');

    if (tmp != "COLLISIONS") {
        cout << "ERROR 10: colisiones mal formadas." << endl;
        return 8;
    }
    cout << tmp << endl;
    getline(levelFile, tmp, '\n');

    level_collisions = new char* [sizeH];
    for (int i = 0; i < sizeH; i++) {
        level_collisions[i] = new char[sizeW];
    }

    for (int i = 0; i < sizeH; i++) {
        for (int j = 0; j < sizeW; j++) {
            getline(levelFile, tmp, ';');
            textChar = tmp[0];
            level_collisions[i][j] = textChar;
        }
        getline(levelFile, tmp, '\n');
    }

    print_map(level_collisions);
    //OBJECTS
    getline(levelFile, tmp, ';');

    if (tmp != "OBJECTS") {
        cout << "ERROR 10: objetos mal formados." << endl;
        return 8;
    }
    cout << tmp << endl;
    getline(levelFile, tmp, '\n');

    level_objects = new char* [sizeH];
    for (int i = 0; i < sizeH; i++) {
        level_objects[i] = new char[sizeW];
    }

    for (int i = 0; i < sizeH; i++) {
        for (int j = 0; j < sizeW; j++) {
            getline(levelFile, tmp, ';');
            textChar = tmp[0];
            level_objects[i][j] = textChar;
        }
        getline(levelFile, tmp, '\n');
    }

    print_map(level_objects);


    levelFile.close();

    entierro(level_floor);

    return 0;
}


