#include "pch.h"
#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/version.hpp>
#include <boost/asio/connect.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>

namespace beast = boost::beast;     // from <boost/beast.hpp>
namespace http = beast::http;       // from <boost/beast/http.hpp>
namespace net = boost::asio;        // from <boost/asio.hpp>
using tcp = net::ip::tcp;           // from <boost/asio/ip/tcp.hpp>



/*
-1+-3
3-+3

*/
//поиск знака
bool FindSign(std::string str, char sign, size_t &signPos)
{
	for (size_t i = 0; i < str.length(); i++)
	{
		if (str[i] == sign && i != 0)
		{
			signPos = i;
			return true;
		}
	}

	return false;


	return signPos != std::string::npos;
}


//удаление пробелов
void RemoveGaps(std::string &str)
{
	for (size_t i = 0; i < str.length(); i++)
	{
		if (str[i] == ' ')
		{
			str.erase(i, 1);
		}
	}
}

//обработка строки
bool ProcessingSTR(std::vector <std::string> &request)
{
	RemoveGaps(request[0]);

	size_t signPos{};

	if (!FindSign(request[0], '-', signPos))
		if (!FindSign(request[0], '+', signPos))
		{
			std::cout << "The sign cannot be found" << std::endl;
			return false;
		}

	request[1] = request[0][signPos];
	request[2] = request[0].substr(signPos + 1, request[0].length());
	request[0] = request[0].erase(signPos, request[0].length());


	return true;
}

// http запрос методом GET и вывод ответа на экран
int main()
{
	setlocale(0, "");

	std::vector <std::string> request(3);
	std::getline(std::cin, request[0]);
	//std::cin >> request[0]; //ввод запроса

	//std::vector <std::string> h_request = Processor(request);

	if (!ProcessingSTR(request))
	{
		return -1;
	}


	try {
		std::string const host = "157.230.27.215";
		std::string const port = "80";
		std::stringstream url;
		std::string action;

		if (request[1] == "-") //выбор нужного обращения
			action = "/calc/diff/";
		else
			action = "/calc/sum/";


		url << action << request[0] << "/" << request[2];


		std::cout << "\n\n\n" << url.str() << "\n\n\n" << std::endl;

		std::string const target = url.str();
		int const version = 11;
		std::cout << target;

		net::io_context ioc;  //требуется для всех вводов-выводов

		// объекты исполняющие ф-цию ввода-вывода 
		tcp::resolver resolver(ioc); //распознаватель 
		auto const results = resolver.resolve(host, port); //шаблон локатора предоставляемых услуг 

		beast::tcp_stream stream(ioc); //используеся протокол TCP
		stream.connect(results); //функция connect используется процессом-клиентом для установления связи с сервером

		// настройка сообщения http запроса методом GET
		http::request<http::string_body> req{ http::verb::get, target, version };
		//заголовки
		req.set(http::field::host, host);
		req.set(http::field::user_agent, "gg/test");

		http::write(stream, req); //отправка http запроса на удаленный хост

		beast::flat_buffer buffer; //буфер для чтения ОБЯЗАТЕЛЕН к сохранению

		http::response<http::dynamic_body> res; //контейнер для хранения ответа

		http::read(stream, buffer, res); //получение http ответа

		std::cout << res << std::endl; //вывод ответа

		beast::error_code ec; // код ошибки
		stream.socket().shutdown(tcp::socket::shutdown_both, ec); //закрытие сокета

		if (ec && ec != beast::errc::not_connected) //если произошла ошибка кроме not_connected 
			throw beast::system_error{ ec }; //сигнал о возникновении ошибки

	}
	catch (std::exception const& e) { //обработка исключений
		std::cerr << "Error: " << e.what() << std::endl;
		return EXIT_FAILURE;
	}

	return 0;
}




int main0()
{
	setlocale(0, ""); //русская локализация

	std::string request; //строка запроса
	std::getline(std::cin, request); //ввод строки с клавиатуры

	/*
	value1 - первый параметр
	value2 - второй параметр
	sign - знак (+-)
	*/
	std::string value1, value2, sign; 


	//поиск и удаление пробелов
	for (size_t i = 0; i < request.size(); i++) //перебираем все символы в строке
	{
		if (request[i] == ' ') //если среди них есть пробел
		{
			request.erase(i, 1); //удаление пробела
		}
	}


	//поиск знака
	size_t signIndex = request.find('-'); //поиск чимвола "-"
	if (signIndex != std::string::npos) //если символ был найден, то ...
	{
		sign = "-"; //записываем в строку "-"
	}
	else //если минус не найден, то ...
	{
		signIndex = request.find('+'); //ищем плюс
		if (signIndex != std::string::npos) //если плюс найден, то ...
		{
			sign = "+"; //запиываем в строку "+"
		}
		else //если не было найдено ни плюса, ни минуса, то ...
		{
			std::cout << "The sign can't be find" << std::endl; //выводим ошибку и ...
			return -1; //завершаем выполнение программы
		}
	}



	value1 = request.substr(0, signIndex); //записать первое слагаемое (до знака) в строку
	value2 = request.substr(signIndex + 1, request.size()); //записать второе слагаемое (после знака) в строку


	try {
		std::string const host = "157.230.27.215";
		std::string const port = "80";
		std::stringstream url;
		std::string action;

		if (sign == "-") //выбор нужного обращения
			action = "/calc/diff/";
		else
			action = "/calc/sum/";


		url << action << value1 << "/" << value2; //запись ссылки (url)


		std::string const target = url.str();
		int const version = 11;
		std::cout << target;

		net::io_context ioc;  //требуется для всех вводов-выводов

		// объекты исполняющие ф-цию ввода-вывода 
		tcp::resolver resolver(ioc); //распознаватель 
		auto const results = resolver.resolve(host, port); //шаблон локатора предоставляемых услуг 

		beast::tcp_stream stream(ioc); //используеся протокол TCP
		stream.connect(results); //функция connect используется процессом-клиентом для установления связи с сервером

		// настройка сообщения http запроса методом GET
		http::request<http::string_body> req{ http::verb::get, target, version };
		//заголовки
		req.set(http::field::host, host);
		req.set(http::field::user_agent, "gg/test");

		http::write(stream, req); //отправка http запроса на удаленный хост

		beast::flat_buffer buffer; //буфер для чтения ОБЯЗАТЕЛЕН к сохранению

		http::response<http::dynamic_body> res; //контейнер для хранения ответа

		http::read(stream, buffer, res); //получение http ответа

		std::cout << res << std::endl; //вывод ответа

		beast::error_code ec; // код ошибки
		stream.socket().shutdown(tcp::socket::shutdown_both, ec); //закрытие сокета

		if (ec && ec != beast::errc::not_connected) //если произошла ошибка кроме not_connected 
			throw beast::system_error{ ec }; //сигнал о возникновении ошибки

	}
	catch (std::exception const& e) { //обработка исключений
		std::cerr << "Error: " << e.what() << std::endl;
		return EXIT_FAILURE;
	}

	return 0;
}