#include <SFML/Graphics.hpp>
#include <vector>
#include <iostream>

const int SIZE=16;
const int ARRAY_SIZE=SIZE*SIZE;
const int FIELD_SIZE=820;
const int CELL_SIZE=50;


//Вспомогательный класс для движений клавиш
enum class Direction {Left=0, Right=1, Up=2, Down=3};

//======================================================//
//Класс для игрового поля
class Field : public sf::Drawable, public sf::Transformable
{
protected:
    //массив, который смотрит, башня какого типа стоит в клетке(скорее всего потом это будет не int, а tower)
    int places[ARRAY_SIZE];
    //массив отвечающий за дорогу для монстров

    sf::Font font;
    int current_position;

    int road[ARRAY_SIZE];
    int road_starting_cell;
    int road_finish_cell;
public:

    Field()
    {
        font.loadFromFile("Bebas.ttf");
        Init();
    }

    int get_road_starting_cell()
    {
        return road_starting_cell;
    }

    void set_road_starting_cell(int value)
    {
        road_starting_cell=value;
    }

    void set_road_finish_cell(int value)
    {
        road_finish_cell=value;
    }

    /*
    void set_road(int* arr)
    {
        *road = arr;
    }*/

    int get_road_finish_cell()
    {
        return road_finish_cell;
    }

    int* get_road()
    {
        return road;
    }

    //создаем дорогу(потом сделаем рандомное создание)

    void MakeRoad1()
    {
        road_starting_cell =32;
        road_finish_cell = 223;

        for (int i=32;i<=45;i++)
            road[i]=1;
        road[61]=1;
        road[77]=1;
        road[93]=1;
        road[109]=1;
        road[125]=1;
        road[141]=1;
        road[157]=1;
        for (int i=166;i<=173;i++)
            road[i]=1;
        road[150]=1;
        road[134]=1;
        road[118]=1;
        road[102]=1;
        for (int i=82;i<=86;i++)
            road[i]=1;
        for (int i=210;i<=223;i++)
            road[i]=1;
        road[98]=1;
        road[114]=1;
        road[130]=1;
        road[146]=1;
        road[162]=1;
        road[178]=1;
        road[194]=1;
    }

    void Init()
    {
        for(int i=0;i<ARRAY_SIZE;i++)
            places[i]=0;
        for(int i=0;i<ARRAY_SIZE;i++)
            road[i]=0;
        MakeRoad1();
        current_position = 0;
    }

    //отрисовываем поле в этой функции

    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const
    {
        states.transform *= getTransform();
        sf::Color color = sf::Color(200, 100, 200);

        //готовим рамку для поля

        sf::RectangleShape shape(sf::Vector2f(FIELD_SIZE, FIELD_SIZE));
        shape.setOutlineThickness(2.0);
        shape.setOutlineColor(color);
        shape.setFillColor(sf::Color::Transparent);
        target.draw(shape, states);

        //готовим форму для клетки

        shape.setSize(sf::Vector2f(CELL_SIZE - 2, CELL_SIZE - 2));
        shape.setOutlineThickness(2.0);
        //shape.setOutlineColor(color);
        //shape.setFillColor(sf::Color::Transparent);

        // Подготавливаем текстовую заготовку для отрисовки номеров плашек
        sf::Text text("", font, 52);

        for (int i=0;i<ARRAY_SIZE;i++)
        {
            //рассчитываем цвет для клетки

            sf::Color temp_color = sf::Color(255, 255, 255);
            if (road[i]==1)
                temp_color=sf::Color(128, 64, 0);
            else
                if (places[i]==0)
                    temp_color=sf::Color(255, 255, 255);
                if (places[i]==1)
                    temp_color=sf::Color(100,0,0);
                if (places[i]==2)
                    temp_color=sf::Color(0,100,0);
                if (places[i]==3)
                    temp_color=sf::Color(0,0,100);

            //цвет для границы

            shape.setFillColor(temp_color);
            if (current_position==i)
                shape.setOutlineColor(sf::Color::Red);
            else
                shape.setOutlineColor(sf::Color::Transparent);

            text.setFillColor(sf::Color(0,0,0));
            if (road[i]!=1)
                text.setString("A");
            if (road[i]==1)
                text.setString("");

            //считаем координаты клетки

            sf::Vector2f position(i % SIZE * CELL_SIZE + 10.0, i / SIZE * CELL_SIZE + 10.0);
            shape.setPosition(position);

            // Позицию текста подбирал вручную
			text.setPosition(position.x + (CELL_SIZE*2.75/10), position.y - (CELL_SIZE/5));

            //отрисовываем плашку
            target.draw(shape, states);

            // Отрисовываем номер плашки
			target.draw(text, states);
        }

    };

    void Build_Tower(int number)
    {
        if (road[current_position] == 0)
            places[current_position] = number;
    }

    //функция-движение выделенной клетки

    void moving(Direction direction)
    {
        //находим координаты клетки

        int col = current_position % SIZE;
        int row = current_position / SIZE;

        if (direction == Direction::Left) col--;
        if (direction == Direction::Right) col++;
        if (direction == Direction::Up) row--;
        if (direction == Direction::Down) row++;

        if (col<0) col=0;
        if (col>SIZE-1) col=SIZE-1;
        if (row<0) row=0;
        if (row>SIZE-1) row=SIZE-1;

        current_position = SIZE*row + col;
    }
};
//======================================================//

class Enemy : public sf::Drawable, public sf::Transformable
{
protected:
    int max_health=100;
    int current_health=100;
    float speed=1.0;
    sf::Font font;
    int position_x;
    int position_y;

    std::vector <int> traectory = {};
    int road_starting_cell;
    int road_finish_cell;
    int road[ARRAY_SIZE];
    int cur_cell=0;
public:
    Enemy()
    {
        font.loadFromFile("Bebas.ttf");
        Init();
    }

    void Init()
    {
        position_x = 0;
        position_y = 0;
        current_health = max_health;
    }

    void set_speed(float value)
    {
        speed=value;
    }

    void set_max_health(int value)
    {
        max_health = value;
    }

    void set_road_starting_cell(int value)
    {
        road_starting_cell=value;
    }

    void set_road_finish_cell(int value)
    {
        road_finish_cell=value;
    }

    int get_road_starting_cell()
    {
        return road_starting_cell;
    }

    int get_road_finish_cell()
    {
        return road_finish_cell;
    }

    void set_road(int *arr)
    {
        std::copy(arr, arr + ARRAY_SIZE, std::begin(road));
    }

    void Make_Traectory()
    {
        int cur_cell=road_starting_cell;
        int prev_cell=-1;

        traectory.push_back(road_starting_cell);
        std::cout << "VOSHLY V CYCLE" << '\n';
        while (cur_cell != road_finish_cell)
        {
            int next_cell=0;
            if (road[cur_cell+1]==1 and prev_cell!=cur_cell+1)
                next_cell=cur_cell+1;
            if (cur_cell>0)
                if (road[cur_cell-1]==1 and prev_cell!=cur_cell-1)
                    next_cell=cur_cell-1;
            if (road[cur_cell+SIZE]==1 and prev_cell!=cur_cell+SIZE)
                next_cell=cur_cell+SIZE;
            if (cur_cell>=SIZE)
                if (road[cur_cell-SIZE]==1 and prev_cell!=cur_cell-SIZE)
                    next_cell=cur_cell-SIZE;
            if (next_cell!=0)
            {
                traectory.push_back(next_cell);
                prev_cell=cur_cell;
                cur_cell = next_cell;
            }
            if (next_cell==0)
                break;
        }
        std::cout << "VYSHLY V CYCLE" << '\n';
        traectory.push_back(road_finish_cell);
    }

    void setpos()
    {
        position_x = road_starting_cell % SIZE * CELL_SIZE + 60.0;
        position_y = road_starting_cell / SIZE * CELL_SIZE + 60.0;
        cur_cell = 0;
    }

    void moving()
    {
        sf::Vector2f dir(0,0);
        if (cur_cell!=road_finish_cell)
            if (traectory[cur_cell+1] - traectory[cur_cell] == 1)
            {
                dir.x = 1;
                dir.y = 0;
            }
            if (traectory[cur_cell+1] - traectory[cur_cell] == -1)
            {
                dir.x = -1;
                dir.y = 0;
            }
            if (traectory[cur_cell+1] - traectory[cur_cell] == SIZE)
            {
                dir.x = 0;
                dir.y = 1;
            }
            if (traectory[cur_cell+1] - traectory[cur_cell] == -SIZE)
            {
                dir.x = 0;
                dir.y = -1;
            }
        position_x += speed*dir.x;
        position_y += speed*dir.y;

        //корявый метод проверки достигли ли мы следующей клетки или нет

        sf::Vector2f div1(position_x - (traectory[cur_cell] % SIZE * CELL_SIZE + 60.0), position_y - (traectory[cur_cell] / SIZE * CELL_SIZE + 60.0));
        sf::Vector2f div2(position_x - (traectory[cur_cell+1] % SIZE * CELL_SIZE + 60.0), position_y - (traectory[cur_cell+1] / SIZE * CELL_SIZE + 60.0));

        if (dir.x!=0)
            if (div1.x*div2.x > 0)
                cur_cell++;
        if (dir.y!=0)
            if (div1.y*div2.y > 0)
                cur_cell++;

        //std::cout << div1.x << ' ' << div1.y << '\n';
        //std::cout << div2.x << ' ' << div2.y << '\n';
        //std::cout << '\n';
    }

    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const
    {
        states.transform *= getTransform();

        //готовим форму для врага

        sf::RectangleShape shape(sf::Vector2f(FIELD_SIZE, FIELD_SIZE));

        shape.setSize(sf::Vector2f(CELL_SIZE - 10, CELL_SIZE - 10));
        shape.setOutlineThickness(2.0);
        shape.setOutlineColor(sf::Color::White);
        shape.setFillColor(sf::Color::Black);

        // Подготавливаем текстовую заготовку для отрисовки номеров плашек
        sf::Text text("", font, 30);

        text.setFillColor(sf::Color(255,255,255));
        text.setString(std::to_string(current_health));

        //считаем координаты врага

        sf::Vector2f position(position_x,position_y);
        shape.setPosition(position);

        // Позицию текста подбирал вручную
        text.setPosition(position_x , position_y);

        //отрисовываем врага
        target.draw(shape, states);

        // Отрисовываем здоровье
        target.draw(text, states);
    };
};

//структура для врагов

struct GameEnemy {
    Enemy himself;
    float time;
    int max_health;
    float speed = 1.0;
};

//======================================================//
class Game
{
protected:
    int Current_money;
    sf::Font font;
    sf::RenderWindow& window;
public:
    Field GameField;
    GameEnemy ARRAY_ENEMY[10];

    Game(sf::RenderWindow& win) : window(win)
    {
        font.loadFromFile("Bebas.ttf");
        Init();
    }
    void Init()
    {
        Current_money = 100;

        //==========================================//
        /*
        for (int i=0;i<ARRAY_SIZE;i++)
            Tester.road[i] = GameField.get_road()[i];
        */
        //===========================================//
        for (int i=0;i<10;i++)
        {
            ARRAY_ENEMY[i].himself.set_speed(ARRAY_ENEMY[i].speed);
            ARRAY_ENEMY[i].himself.set_max_health(ARRAY_ENEMY[i].max_health);

            ARRAY_ENEMY[i].himself.set_road(GameField.get_road());
            ARRAY_ENEMY[i].himself.set_road_starting_cell(GameField.get_road_starting_cell());
            ARRAY_ENEMY[i].himself.set_road_finish_cell(GameField.get_road_finish_cell());
            ARRAY_ENEMY[i].himself.Make_Traectory();
            ARRAY_ENEMY[i].himself.setpos();
        }
        /*
        for ( int n : Tester.traectory ) {
        std::cout << n << '\n';}*/
    }

    void Update_Enemies()
    {
        for (int i=0;i<10;i++)
            ARRAY_ENEMY[i].himself.moving();
        for (int i=0;i<10;i++)
            window.draw(ARRAY_ENEMY[i].himself);
    }
};
//======================================================//
int main()
{
    sf::RenderWindow window(sf::VideoMode(920, 920), "SouTD");
    window.setFramerateLimit(60);

    Game game(window);
    game.GameField.setPosition(50.0, 50.0);
    sf::Event event;

    while (window.isOpen())
    {
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed) window.close();
            if (event.type == sf::Event::KeyPressed)
            {
                // Получаем нажатую клавишу - выполняем соответствующее действие
                if (event.key.code == sf::Keyboard::Escape) window.close();
                if (event.key.code == sf::Keyboard::Left) game.GameField.moving(Direction::Left);
                if (event.key.code == sf::Keyboard::Right) game.GameField.moving(Direction::Right);
                if (event.key.code == sf::Keyboard::Up) game.GameField.moving(Direction::Up);
                if (event.key.code == sf::Keyboard::Down) game.GameField.moving(Direction::Down);
                //строим башню
                if (event.key.code == sf::Keyboard::Num1) game.GameField.Build_Tower(1);
                if (event.key.code == sf::Keyboard::Num2) game.GameField.Build_Tower(2);
                if (event.key.code == sf::Keyboard::Num3) game.GameField.Build_Tower(3);
                // Новая игра
                if (event.key.code == sf::Keyboard::F2)
                {
                    game.Init();
                    //move_counter = 100;
                }
            }
        }

        // Выполняем необходимые действия по отрисовке
        window.clear();
        //game.Tester.moving();
        window.draw(game.GameField);
        game.Update_Enemies();
        //window.draw(game.Tester);
        //game.Update();
        //window.draw(text);
        window.display();
    }

    return 0;
}
