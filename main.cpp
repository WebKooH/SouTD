#include <SFML/Graphics.hpp>

const int SIZE=16;
const int ARRAY_SIZE=SIZE*SIZE;
const int FIELD_SIZE=820;
const int CELL_SIZE=50;


//¬спомогательный класс дл€ движений клавиш
enum class Direction {Left=0, Right=1, Up=2, Down=3};

//======================================================//
// ласс дл€ игрового пол€
class Field : public sf::Drawable, public sf::Transformable
{
protected:
    //массив, который смотрит, башн€ какого типа стоит в клетке(скорее всего потом это будет не int, а tower)
    int places[ARRAY_SIZE];
    //массив отвечающий за дорогу дл€ монстров
    int road[ARRAY_SIZE];
    sf::Font font;
    int current_position;
public:
    Field()
    {
        font.loadFromFile("Bebas.ttf");
        Init();
    }

    //создаем дорогу(потом сделаем рандомное создание)

    void MakeRoad1()
    {
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

        //готовим рамку дл€ пол€

        sf::RectangleShape shape(sf::Vector2f(FIELD_SIZE, FIELD_SIZE));
        shape.setOutlineThickness(2.0);
        shape.setOutlineColor(color);
        shape.setFillColor(sf::Color::Transparent);
        target.draw(shape, states);

        //готовим форму дл€ клетки

        shape.setSize(sf::Vector2f(CELL_SIZE - 2, CELL_SIZE - 2));
        shape.setOutlineThickness(2.0);
        //shape.setOutlineColor(color);
        //shape.setFillColor(sf::Color::Transparent);

        // ѕодготавливаем текстовую заготовку дл€ отрисовки номеров плашек
        sf::Text text("", font, 52);

        for (int i=0;i<ARRAY_SIZE;i++)
        {
            //рассчитываем цвет дл€ клетки

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

            //цвет дл€ границы

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

            // ѕозицию текста подбирал вручную
			text.setPosition(position.x + (CELL_SIZE*2.75/10), position.y - (CELL_SIZE/5));

            //отрисовываем плашку
            target.draw(shape, states);

            // ќтрисовываем номер плашки
			target.draw(text, states);
        }

    };

    void Build_Tower(int number)
    {
        if (road[current_position] == 0)
            places[current_position] = number;
    }

    //функци€-движение выделенной клетки

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
class Game
{
protected:
    int Current_money;
    sf::Font font;
public:
    Field GameField;
    Game()
    {
        font.loadFromFile("Bebas.ttf");
        Init();
    }
    void Init()
    {
        Current_money = 100;
    }
};
//======================================================//
int main()
{
    sf::RenderWindow window(sf::VideoMode(920, 920), "SouTD");
    window.setFramerateLimit(60);

    Game game;
    game.GameField.setPosition(50.0, 50.0);
    sf::Event event;

    while (window.isOpen())
    {
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed) window.close();
            if (event.type == sf::Event::KeyPressed)
            {
                // ѕолучаем нажатую клавишу - выполн€ем соответствующее действие
                if (event.key.code == sf::Keyboard::Escape) window.close();
                if (event.key.code == sf::Keyboard::Left) game.GameField.moving(Direction::Left);
                if (event.key.code == sf::Keyboard::Right) game.GameField.moving(Direction::Right);
                if (event.key.code == sf::Keyboard::Up) game.GameField.moving(Direction::Up);
                if (event.key.code == sf::Keyboard::Down) game.GameField.moving(Direction::Down);
                //строим башню
                if (event.key.code == sf::Keyboard::Num1) game.GameField.Build_Tower(1);
                if (event.key.code == sf::Keyboard::Num2) game.GameField.Build_Tower(2);
                if (event.key.code == sf::Keyboard::Num3) game.GameField.Build_Tower(3);
                // Ќова€ игра
                if (event.key.code == sf::Keyboard::F2)
                {
                    game.Init();
                    //move_counter = 100;
                }
            }
        }

        // ¬ыполн€ем необходимые действи€ по отрисовке
        window.clear();
        window.draw(game.GameField);
        //game.Update();
        //window.draw(text);
        window.display();
    }


    return 0;
}
