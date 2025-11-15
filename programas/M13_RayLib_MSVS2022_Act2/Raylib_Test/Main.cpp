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

#include <vector>
#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include "main.h"

using namespace std;

float curVersion = 0.6;
float minVersion = 0.3;

float fileVersion = -1.0;
string levelTitle = "NULL";

int minSize = 2;
int sizeW = -1;
int sizeH = -1;

int playerX = -1;
int playerY = -1;

int minTexture = 1;
int fileTextures = -1;

char textChar;
string textName;
string level = "Third_level.erro";

map<char, string> textureFile;
map<char, Texture2D> textures;
map<char, Model> models;

char** level_floor;
char** level_stage;
char** level_collisions;
char** level_objects;

string musicFile = "";
Music gameMusic;
bool musicLoaded = false;

int score = 3;

bool canMoveTo(int x, int y, bool isPlayer) {
    int maxY = sizeH;
    int maxX = sizeW;
    if (maxY == 0 || maxX == 0) return false;

    if (x < 0 || y < 0 || y >= maxY || x >= maxX)
        return false;

    if (isPlayer) {
        return level_collisions[y][x] == '0';
    }
    else if (!isPlayer){
        return level_collisions[y][x] != 'X';
    }
}

bool tryMoveCoffin(int playerX, int playerY, int dx, int dy) {
    int targetX = playerX + dx;
    int targetY = playerY + dy;
    int pushX = targetX + dx;
    int pushY = targetY + dy;

    if (targetY < 0 || targetY >= sizeH || targetX < 0 || targetX >= sizeW)
        return false;

    if (level_objects[targetY][targetX] == 'C') {
        if (canMoveTo(pushX, pushY, false) && level_objects[pushY][pushX] == '0') {
            level_objects[pushY][pushX] = 'C';
            level_objects[targetY][targetX] = '0';

            if (level_collisions[pushY][pushX] == 'S') {
                level_floor[pushY][pushX] = 'C';
                level_objects[pushY][pushX] = '0';
                score--;
            }
            return true;
        }
        return false; 
    }

    return true; 
}


int entierro(char** floor)
{
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 800;
    const int screenHeight = 450;

    InitWindow(screenWidth, screenHeight, "raylib [core] example - 3d camera mode");

	// Logica para añadir la musica
    InitAudioDevice();

    if (fileVersion >= 0.6f && musicFile != "")
    {
        gameMusic = LoadMusicStream(musicFile.c_str());
        PlayMusicStream(gameMusic);
        musicLoaded = true;
    }

    // Define the camera to look into our 3d world
    Camera3D camera = { 0 };
    camera.position = Vector3{ 0.0f, 20.0f, 1.0f };  // Camera position
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

    for (map<char, string>::iterator it = textureFile.begin(); it != textureFile.end(); ++it) {
        Texture2D t = LoadTexture(it->second.c_str());

        Mesh mesh = GenMeshCube(cubeSize, cubeSize, cubeSize);
        Model m = LoadModelFromMesh(mesh);
        m.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = t;

        models.insert({ it->first, m });
    }
    
    // Find player position in level_objects
    for (int h = 0; h < sizeH; h++) {
        for (int w = 0; w < sizeW; w++) {
            if (level_objects[h][w] == '@') {
                playerX = w;
                playerY = h;
            }
        }
    }

    SetTargetFPS(60); // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------
    float timer;
    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // Update
        // 
		// Musica si se ha cargado
        if (musicLoaded) UpdateMusicStream(gameMusic);

        // Movimiento del jugador (WASD y flechas)
        if(score != 0) 
            timer = GetTime();

        if (IsKeyPressed(KEY_W) || IsKeyPressed(KEY_UP)) {
            int dx = 0, dy = -1;
            if (tryMoveCoffin(playerX, playerY, dx, dy) && canMoveTo(playerX + dx, playerY + dy, true))
                playerY += dy;
        }
        if (IsKeyPressed(KEY_S) || IsKeyPressed(KEY_DOWN)) {
            int dx = 0, dy = 1;
            if (tryMoveCoffin(playerX, playerY, dx, dy) && canMoveTo(playerX + dx, playerY + dy, true))
                playerY += dy;
        }
        
        if (IsKeyPressed(KEY_A) || IsKeyPressed(KEY_LEFT)) {
            int dx = -1, dy = 0;
            if (tryMoveCoffin(playerX, playerY, dx, dy) && canMoveTo(playerX + dx, playerY + dy, true))
                playerX += dx;
        }
        
        if (IsKeyPressed(KEY_D) || IsKeyPressed(KEY_RIGHT)) {
            int dx = 1, dy = 0;
            if (tryMoveCoffin(playerX, playerY, dx, dy) && canMoveTo(playerX + dx, playerY + dy, true))
                playerX += dx;
        }
        
        
        
        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

        ClearBackground(RAYWHITE);

        BeginMode3D(camera);

        cubePosition = { cubeInitX, 0.0f, cubeInitZ };

        DrawGrid(10, 1.0f);


        // Draw Floor
        cubePosition = { cubeInitX, 0.0f, cubeInitZ };
        for (int h = 0; h < sizeH; h++) {
            cubePosition.x = cubeInitX;
            for (int w = 0; w < sizeW; w++) {
                char floorKey = floor[h][w];

                if (floorKey != '0') {
                        DrawModel(models[floorKey], cubePosition, 1.0f, WHITE);
                    
                    DrawCubeWires(cubePosition, cubeSize, cubeSize, cubeSize, BLACK);
                }

                cubePosition.x += cubeSize;
            }
            cubePosition.z += cubeSize;
        }

        // Draw Stage (y+1)
        cubePosition = { cubeInitX, 1.0f, cubeInitZ };
        for (int h = 0; h < sizeH; h++) {
            cubePosition.x = cubeInitX;
            for (int w = 0; w < sizeW; w++) {
                if (level_stage[h][w] != '0') {
                    char texKey = level_stage[h][w];
                    
                    DrawModel(models[texKey], cubePosition, 1.0f, WHITE);
                    DrawCubeWires(cubePosition, cubeSize, cubeSize, cubeSize, BLACK);
                }
                cubePosition.x += cubeSize;
            }
            cubePosition.z += cubeSize;
        }

        
        // Draw Player (y+1)
        if (playerX != -1 && playerY != -1) {
            float playerRadius = 0.5f;
            Vector3 playerPos = {
                cubeInitX + playerX * cubeSize,
                1.0f,
                cubeInitZ + playerY * cubeSize
            };

            DrawSphere(playerPos, playerRadius, RED);
            DrawModel(models['@'], playerPos, 1.0f, WHITE);
        }

        // Draw Coffins(y+1)
		int numCoffins = 0;
        cubePosition = { cubeInitX, 1.f, cubeInitZ };
        for (int h = 0; h < sizeH; h++) {
            cubePosition.x = cubeInitX;
            for (int w = 0; w < sizeW; w++) {
                char objKey = level_objects[h][w];
                if (objKey != '0' && objKey != '@') {
                    DrawModel(models[objKey], cubePosition, 1.0f, WHITE);
                    DrawCubeWires(cubePosition, cubeSize, cubeSize, cubeSize, BLACK);
					numCoffins++;
                }
                cubePosition.x += cubeSize;
            }
            cubePosition.z += cubeSize;
        }
		score = numCoffins;

        EndMode3D();

        if (score == 0) {
            DrawText("Enterradas! YOU WIN", 10, 40, 20, DARKGRAY);
            DrawText(TextFormat("%.2f", timer), 10, 60, 20, DARKGRAY);
        }
        else {
            DrawText("Entierramelas Todas!", 10, 40, 20, DARKGRAY);
            DrawText(TextFormat("%.2f",timer), 10, 60, 20, DARKGRAY);
        }

        DrawFPS(10, 10);

        EndDrawing();
        //----------------------------------------------------------------------------------
    }
	//liberar modelos y musica
    for (auto& m : models) {
        UnloadModel(m.second);
    }

    if (musicLoaded) UnloadMusicStream(gameMusic);
    CloseAudioDevice();

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
    ifstream levelFile(level);

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

    

	// Musica para 0.6 o superior
    if (fileVersion >= 0.6f)
    {
        getline(levelFile, tmp, ';');
        if (tmp != "MUSIC")
        {
            cout << "ERROR 9: Falta la línea MUSIC en versiones 0.6 o superiores." << endl;
            return 9;
        }

        getline(levelFile, tmp, ';');
        musicFile = tmp;

        if (musicFile == "" || musicFile == "NULL") {
            cout << "WARNING: Archivo de música inválido, no se cargará música." << endl;
        }

        getline(levelFile, tmp, '\n');
    }
    else
    {
        // Si el archivo es 0.5 o anterior, seguimos normalmente
        cout << "Archivo sin música (versión antigua)." << endl;
    }

	getline(levelFile, tmp, ';');

	// Tamaño

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


