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

