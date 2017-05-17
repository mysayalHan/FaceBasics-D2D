#pragma once
#ifndef GETWEATHER_H
#define GETWEATHER_H

#include "SomeStruct.h"

class GetWeather 
{
protected:
	std::string city_id;
	std::string api;

public:
	GetWeather();
	GetWeather(std::string & city_id);
	~GetWeather();

public:
	void setCity(std::string & city_id);
	Weather getWeather();

private:
	void init();
};

#endif // !GETWEATHER_H

