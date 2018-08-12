#include <iostream>
#include <random>
#include <ctime>
#include <type_traits>
#define _USE_MATH_DEFINES
#include <math.h>

#include <SFML/Window.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/CircleShape.hpp>

using std::enable_if_t;
using std::is_floating_point_v;

using sf::RenderWindow;
using sf::Event;
using sf::VideoMode;
using sf::CircleShape;
using sf::Color;
using sf::Clock;

inline int rand_val(int _min, int _max)
{
	return _min + rand() % (_max - _min + 1);
}

template<typename T, typename = enable_if_t<is_floating_point_v<T>>>
constexpr inline T to_radian(T deg) noexcept
{
	return deg * M_PI / static_cast<T>(180);
}

void update_my_window(Clock& t, const RenderWindow& window, CircleShape& circle, int step, int& angle)
{
	angle %= 360;	// чтобы отсечь лишнее(если угол больше 360)

	static int count = 0;
	static bool downsize = false;	// окружность уменьшается / увеличивается
	constexpr float min_size = 10;	// минимальный размер окружности
	constexpr float max_size = 500;	// максимальный размер окружности

	float radius = circle.getRadius();	// радиус окружности

	if ((t.getElapsedTime()).asSeconds() >= 0.005f)
	{
		if (++count >= 30)
		{
			radius += (downsize) ? -1 : 1;

			if (radius >= max_size){
				downsize = true;
			}
			else if (radius <= min_size){
				downsize = false;
			}
			circle.setRadius(radius);
			count = 0;
		}

		float pos_x = circle.getPosition().x + step * cos(to_radian(static_cast<float>(angle)));
		float pos_y = circle.getPosition().y + step * sin(to_radian(static_cast<float>(angle)));

		const auto win_sz_x = static_cast<float>(window.getSize().x);
		const auto win_sz_y = static_cast<float>(window.getSize().y);

		if (pos_x < 0.f || pos_x + radius * 2.f >= win_sz_x)
		{
			angle = 180 - angle;
			circle.setFillColor(Color(rand_val(0, 255), rand_val(0, 255), rand_val(0, 255)));
		}

		if (pos_y < 0.f)
		{
			if (angle == 270 || angle == -90) {
				angle = 90;
			}
			else {
					angle = angle * 2 + 90;
			}
			circle.setFillColor(Color(rand_val(0, 255), rand_val(0, 255), rand_val(0, 255)));
		}
		else if (pos_y + radius * 2.f >= win_sz_y)
		{
			if (angle == 90 || angle == -270)
				angle = 270;
			else {
				angle = angle * 2 - 90;
			}
			circle.setFillColor(Color(rand_val(0, 255), rand_val(0, 255), rand_val(0, 255)));
		}

		pos_x = circle.getPosition().x + step * cos(to_radian(static_cast<float>(angle)));
		pos_y = circle.getPosition().y + step * sin(to_radian(static_cast<float>(angle)));

		circle.setPosition(pos_x, pos_y);
		t.restart();
	}
}

int main()
{
	RenderWindow window(VideoMode(1920, 1080), "My Window", sf::Style::Fullscreen);

	CircleShape csh(50.f);
	csh.setPosition(window.getSize().x / 2.f - csh.getRadius(), window.getSize().y / 2.f - csh.getRadius());
	csh.setFillColor(Color::Green);

	Clock my_clock;		// время пошло
	time_t curr_time;
	time(&curr_time);
	srand(static_cast<unsigned int>(curr_time));

	constexpr int step = 2;			// шаг
	int angle = rand_val(0, 360);	// угол отражения

	// программа будет запущена до тех пор, пока окно открыто
	while (window.isOpen())
	{
		// проверка всех событий окна, произошедших с последней итерации цикла
		Event event;
		while (window.pollEvent(event))
		{
			// пользователь попытался закрыть окно: мы закрываем окно
			if (event.type == Event::Closed) {
				window.close();
			}
		}

		// обновляем окно
		update_my_window(my_clock, window, csh, step, angle);

		window.clear();
		window.draw(csh);
		window.display();
	}

	return 0;
}
