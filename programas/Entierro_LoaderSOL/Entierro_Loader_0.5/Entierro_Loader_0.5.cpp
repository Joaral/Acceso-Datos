#include <iostream>
#include <fstream>
#include <string>
#include <map>

using namespace std;

float curVersion = 0.5;
float minVersion = 0.3;

float fileVersion = -1.0;
string levelTitle = "NULL";

int minSize = 2;
int sizeW = -1;
int sizeH = -1;

int minTexture = 1;
int textures = -1;

char textChar;
string textName;

map<char, string> textures;

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

    getline(levelFile, tmp,';');

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
    
    if (stoi(tmp) < minTexture) {
        cout << "ERROR 8.2: El archivo no supera el minimo de texturas." << endl;
        return 8;
    }

    getline(levelFile, tmp, '\n');

    textures = stoi(tmp);

    for (int i = 0; i <= textures; i++) {

        getline(levelFile, tmp, ';');

        textChar = tmp[0];

        getline(levelFile, tmp, ';');

        textName = tmp;

        cout << textChar << " -> " << textName << endl;

        textures.insert({ textChar, textName });

        getline(levelFile, tmp, '\n');
        
    }

    

    levelFile.close();
    return 0;
}


