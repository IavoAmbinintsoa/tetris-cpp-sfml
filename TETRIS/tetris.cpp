#include <SFML/Graphics.hpp>
#include <queue>
#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <random>
#include <fstream>

#define WINDOW_SIZE_HEIGTH 480
#define WINDOW_SIZE_WIDTH 480
#define SPACE 22
#define EMPTY 0
#define MATRIX_END -1
#define MATRIX_WIDTH 10
#define MATRIX_HEIGTH 22
#define CELL_SIZE 20
#define CELL 4

using namespace std;
using namespace sf;


typedef struct cellTetris
{
    Vector2i position;
    Color couleur;
    Sprite sprite;
}cellTetris;

using OneFigure = vector<cellTetris>;
static int score = 0;
static int bestScore;

void blockage()
{
 string g;
 cin >> g;
 cin.clear();
 cin.ignore();
}

vector<vector<int>> tetrisBoard(MATRIX_HEIGTH, vector<int>(MATRIX_WIDTH, EMPTY));
vector<char> figureList{'I','I','o','T','T','T','T','S','S','Z','Z','L','L','L','L','J','J','J','J'};
void fillListCell(vector<OneFigure>& figureList) {
    vector<Vector2i> CoordinateList {
        // I - Ligne droite (4 rotations)
        {0,0},{0,1},{0,2},{0,3},    // Rotation 0
        {0,0},{1,0},{2,0},{3,0},    // Rotation 1

        // O - Carre (1 rotation)
        {0,0},{0,1},{1,0},{1,1},    // Rotation 0 (seulement une rotation)

        // T - T (4 rotations)
        {0,0},{0,1},{0,2},{1,1},    // Rotation 0
        {0,1},{1,0},{1,1},{2,1},    // Rotation 1
        {1,0},{0,1},{1,1},{2,1},    // Rotation 2
        {0,1},{1,0},{1,1},{1,2},    // Rotation 3

        // S - S (2 rotations)
        {0,1},{1,1},{1,0},{2,0},    // Rotation 0
        {1,0},{0,1},{1,1},{0,2},    // Rotation 1

        // Z - Z (2 rotations)
        {0,0},{1,0},{1,1},{2,1},    // Rotation 0
        {1,0},{0,1},{1,1},{0,2},    // Rotation 1

        // L - L (4 rotations)
        {0,0},{1,0},{2,0},{2,1},    // Rotation 0
        {0,0},{0,1},{0,2},{1,0},    // Rotation 1
        {0,0},{1,0},{2,0},{0,1},    // Rotation 2
        {0,0},{1,0},{1,1},{1,2},    // Rotation 3

        // J - J (4 rotations)
        {0,1},{1,1},{2,1},{2,0},    // Rotation 0
        {0,0},{0,1},{0,2},{1,2},    // Rotation 1
        {0,0},{1,0},{2,0},{0,1},    // Rotation 2
        {0,0},{1,0},{1,1},{1,2}     // Rotation 3
    };

    for (int i = 0; i < CoordinateList.size(); i += 4) {
        OneFigure figure;
        for (int j = i; j < i + 4; ++j) {
            cellTetris cell;
            cell.position = CoordinateList[j];
            figure.push_back(cell);
        }
        figureList.push_back(figure);
    }
}

/*
[][][][] x2 (0,0),(0,1),(0,2)(0,3) ; (0,0),(1,0),(2,0)(3,0)

[][] (0,0)(0,1)(1,0)(1,1)
[][]

[][][]  x4 (0,0),(0,1),(0,2)(1,1) ; (0,0),(0,1),(0,2)(-1,-1)
  []        (0,0),(1,0),(2,0)(1,-1);(0,0),(0,1),(0,2)(0,1)

[][][]  x4 (0,0),(0,1),(0,2)(1,3) ; (0,0),(0,1),(0,2)(-1,0)
    []      (0,0),(1,0),(2,0)(3,1) ; (0,0),(1,0),(2,0)(0,-1)

*/
/* OneFigure figureRotation(OneFigure &fig,vector<OneFigure> figList)
{
    OneFigure figRet;
    vector<OneFigure>::iterator it = find(figList.begin(),figList.end(),fig);
    figRet = figList[(it+1)%4];
    return figRet;

} */
int radomInteger(int start, int end)
{
    return rand() % (end - start + 1) + start;  
}
OneFigure randomcell(vector<OneFigure>& forList, const Texture& tex)
{
    int n = radomInteger(0, forList.size() - 1);
    Color c = Color::Green; // Par defaut
    vector<Color> listeCouleur = {
        Color::Cyan, Color::Yellow, Color::Magenta, Color::Green, Color::Red, Color::Blue
    };
    c = listeCouleur[n % listeCouleur.size()];

    OneFigure fig = forList[n];
    for (auto& e : fig)
    {
        e.couleur = c;
        e.sprite.setTexture(tex);
        e.sprite.setColor(c);
        e.sprite.scale(CELL_SIZE/e.sprite.getLocalBounds().width,CELL_SIZE/e.sprite.getLocalBounds().height);
    }
    return fig;
}

Vector2f gridToPixel(Vector2i gridPos) 
{

    return Vector2f(gridPos.x * CELL_SIZE + SPACE, gridPos.y * CELL_SIZE + SPACE - 6);
}
Vector2i pixelToGrid(Vector2f pos)
{
    return {(int)pos.y / CELL_SIZE,(int) pos.x / CELL_SIZE};
}
void checkTetrisMatrix(vector<vector<int>>& tetrisBoard, vector<OneFigure>& cellHistory)
{
    for (int i = 0; i < MATRIX_HEIGTH; ++i)
    {
        bool fullLine = true;
        for (int j = 0; j < MATRIX_WIDTH; ++j)
        {
            if (tetrisBoard[i][j] == EMPTY)
            {
                fullLine = false;
                break;
            }
        }

        if (fullLine)
        {
            score += 10;
            //cout << "Score : " << score << endl;
            // Clear line on the board
            for (int j = 0; j < MATRIX_WIDTH; ++j)
                tetrisBoard[i][j] = EMPTY;
            for (int k = 0; k < tetrisBoard.size(); k++)
            {
                for (int j = 0; j < tetrisBoard[0].size(); j++)
                {
                    if(k == 0)
                        tetrisBoard[k][j] = EMPTY;
                    else
                        tetrisBoard[k][j] = tetrisBoard[k-1][j];
                }
                
            }
            

            // Remove cells from history
            for (auto& figure : cellHistory)
            {
                figure.erase(remove_if(figure.begin(), figure.end(),
                                       [i](const cellTetris& cell) {
                                           return cell.position.y == i;
                                       }),
                             figure.end());
            }

            // Shift everything above down
            for (auto& figure : cellHistory)
            {
                for (auto& cell : figure)
                {
                    if (cell.position.y < i)
                        cell.position.y++;
                }
            }
        }
    }
}

void display(vector<vector<int>> tetrisBoard)
{

    cout << "===========================" << endl;
    for (auto &e : tetrisBoard)
    {
        for (auto &i : e)
        {
            cout << i << " ";
        }
        cout << endl;
    }
}
IntRect toGrid()
{
    return IntRect(0,0,480,480);
}

bool isOnColision(OneFigure& figure,vector<vector<int>> tetrisBoard)
{
    for (auto &e : figure)
        if (e.position.x < 0 || e.position.x >= MATRIX_WIDTH ||
            e.position.y < 0 || e.position.y >= MATRIX_HEIGTH ||
            tetrisBoard[e.position.y][e.position.x] != EMPTY)
                return true;
    
    return false;
}
void printOnMatrix(OneFigure& figure,vector<vector<int>>& tetrisBoard,OneFigure& old)
{
    //cout << "Print on matrix" << endl;
    for (auto &e : figure)
    {
        tetrisBoard[e.position.y][e.position.x] = 1;
    }
    for (auto &e : old)
    {
        tetrisBoard[e.position.y][e.position.x] = EMPTY;
    }
    //display(tetrisBoard);
}
void tetris() {

/// Ouverture de la score
    FILE *back = fopen(".back.txt","r");
    if (back != NULL) {
        fscanf(back, "%d", &bestScore);
        fclose(back);
    } else {
        bestScore = 0; // ou une valeur par défaut
    }
    back = fopen(".back.txt", "w");
    RenderWindow Tetris(VideoMode(WINDOW_SIZE_WIDTH, WINDOW_SIZE_HEIGTH), "TETRIS");

    // Charger la texture du Tetris
    Texture tetrisTexture;
    if (!tetrisTexture.loadFromFile("asset/tetris.png", IntRect(0, 0,480,480))) {
        //cout << "Impossible de charger le fichier de texture" << endl;
        return;
    }

    vector<OneFigure> forList;
    fillListCell(forList);

    Sprite board(tetrisTexture);
    board.scale(WINDOW_SIZE_WIDTH / board.getLocalBounds().width, WINDOW_SIZE_HEIGTH / board.getLocalBounds().height);

    Texture square;
    square.loadFromFile("asset/square.png");

    vector<OneFigure> cellHistory;

    OneFigure cellInAction = randomcell(forList, square);
    //printOnMatrix(cellInAction, tetrisBoard, cellInAction);
    bool end = false;
    Time space(microseconds(200000));

    // Chargement de la police
    


    sf::Text bestScoreTexte;
    queue<OneFigure> premoveRand;
    premoveRand.push(randomcell(forList, square));
    while (Tetris.isOpen()) {
        Event event;
        OneFigure old = cellInAction;
        OneFigure nextFigure = premoveRand.front();
        for (auto &e : nextFigure)
        {
            e.sprite.setPosition(gridToPixel(e.position)+Vector2f(300,120));
        }
        
/// Creation du texte
        sf::Text texte;

        sf::Font font;
        if (!font.loadFromFile("asset/Arial.ttf"))
            return;
        

        texte.setFont(font);                    // Definir la police
        texte.setCharacterSize(24);             // Taille des caracteres
        texte.setString(to_string(score));        // Le texte à afficher
        texte.setFillColor(Color::White);   // Couleur du texte
        texte.setPosition(277.f, 363.f);        // Position (x, y)

        bestScoreTexte.setFont(font);                    // Definir la police
        bestScoreTexte.setCharacterSize(24);             // Taille des caracteres
        bestScoreTexte.setString(to_string(bestScore));        // Le bestScoreTexte à afficher
        bestScoreTexte.setFillColor(Color::White);   // Couleur du bestScoreTexte
        bestScoreTexte.setPosition(277.f, 432.f);        // Position (x, y)
        
        
        //display(tetrisBoard);
        //blockage();
        // Deplacer la figure vers le bas
        for (auto& e : cellInAction)
            e.position.y++; // Deplacement vers le bas

            // Verifier les collisions
        if (isOnColision(cellInAction, tetrisBoard)) {
            // Si collision, on remet la figure en place et l'ajoute a l'historique
            for (auto& e : cellInAction)
                e.position.y--;

            cellHistory.push_back(cellInAction);
            cellInAction = premoveRand.front();
            premoveRand.push(randomcell(forList, square));
            premoveRand.pop();
        }
        
        //printOnMatrix(cellInAction, tetrisBoard, old);
        for (int i = 0; i < tetrisBoard[0].size(); i++)
            if(tetrisBoard[0][i] != EMPTY)
                end = true;
        
        
        while (Tetris.pollEvent(event)) {
            if(end)
            {
                Tetris.close();
                fprintf(back,"%d",max(bestScore,score));
                fclose(back);
                break;
            }
            switch (event.type) {
                case Event::Closed:
                    Tetris.close();
                    fprintf(back,"%d",max(bestScore,score));
                    fclose(back);
                    Tetris.close();
                    break;

                case Event::KeyPressed:
                    switch (event.key.code) {
                        case Keyboard::Left:
                             // Deplacer la figure vers la gauche
                            for (auto& e : cellInAction)
                                e.position.x--;

                            if (isOnColision(cellInAction, tetrisBoard)) {
                                for (auto& e : cellInAction)
                                    e.position.x++; // Annuler le deplacement si collision
                            }
                            break;

                        case Keyboard::Right:
                            // Deplacer la figure vers la droite
                             for (auto& e : cellInAction)
                                e.position.x++;

                            if (isOnColision(cellInAction, tetrisBoard)) {
                                for (auto& e : cellInAction)
                                    e.position.x--; // Annuler le deplacement si collision
                            }
                            break;

                        case Keyboard::Down:
                            space = microseconds(100000);
                            break;

                        case Keyboard::Up:
                            space = microseconds(300000);
                            break;
                        case Keyboard::Space:
                            
                            break;
                        default:
                            break;
                    }
                    break;
                case Event::KeyReleased:
                    space = microseconds(200000);
                default:
                    break;
            }
        }

        Tetris.clear();
        Tetris.draw(board);
        Tetris.draw(texte);
        Tetris.draw(bestScoreTexte);
        for (auto &e : nextFigure)
            Tetris.draw(e.sprite);

        for (auto& e : cellHistory) {
            for (auto& i : e) {
                i.sprite.setPosition(gridToPixel(i.position));
                tetrisBoard[i.position.y][i.position.x] = 1;
                Tetris.draw(i.sprite);
            }
        }
        checkTetrisMatrix(tetrisBoard, cellHistory);
        for (auto& e : cellInAction) {
            e.sprite.setPosition(gridToPixel(e.position));
            Tetris.draw(e.sprite);
        }

        Tetris.display();
        
        sleep(space);
    }
}
int main()
{
    srand(time(NULL));
    tetris();
    return 0;
}
