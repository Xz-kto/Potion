#ifndef CCIDH
#define CCIDH
#include "ITERDEC.h"
#include <vector>
#include <iostream>

using namespace std;

enum class PotionType : int
{
	Regular,
	Explosive,
	Foggy,
	Unknown = -1
};

enum class PotionEffect : int
{
	Regeneration,
	Poisoning,
	Levitation,
	Unknown = -1
};

enum class PotionClass : int
{
	Positive,
	Negative,
	Neutral,
	Unknown = -1
};

class Potion
{
protected:
	PotionType Type;
	PotionEffect Effect;
	int PotionLevel;
	int TimeEffect;

public:
	Potion(int PotionTimeEffect, int PotionPotionLevel)
	{
		PotionLevel = PotionPotionLevel;
		TimeEffect = PotionTimeEffect;
	};
	PotionType GetType() const { return Type; }
	PotionEffect GetEffect() const { return Effect; }
	virtual PotionClass GetClass() const = 0;
	virtual void GetPotionType() = 0;
	virtual void GetDrink() = 0;
	virtual void GetCook() = 0;
};

class Positive : public Potion
{
public:
	Positive(int PotionTimeEffect, int PotionPotionLevel) : Potion(PotionTimeEffect, PotionPotionLevel) { Type = PotionType::Explosive; Effect = PotionEffect::Regeneration; }
	PotionClass GetClass() const { return PotionClass::Positive; }
	void GetPotionType() { wcout << L"Тип: позитивное"; }
	void GetDrink() { wcout << L"Применили зелье регенерации"; }
	void GetCook() { wcout << L"Сварили зелье регенерации"; }
};

class Negative : public Potion
{
public:
	Negative(int PotionTimeEffect, int PotionPotionLevel) : Potion(PotionTimeEffect, PotionPotionLevel) { Type = PotionType::Foggy; Effect = PotionEffect::Poisoning; }
	PotionClass GetClass() const { return PotionClass::Negative; }
	void GetPotionType() { wcout << L"Тип: негативное"; }
	void GetDrink() { wcout << L"Применили зелье отравления"; }
	void GetCook() { wcout << L"Сварили зелье регенерации"; }
};

class Neutral : public Potion
{
public:
	Neutral(int PotionTimeEffect, int PotionPotionLevel) : Potion(PotionTimeEffect, PotionPotionLevel) { Type = PotionType::Regular; Effect = PotionEffect::Levitation; }
	PotionClass GetClass() const { return PotionClass::Neutral; }
	void GetPotionType() { wcout << L"Тип: нейтральное"; }
	void GetDrink() { wcout << L"Применили зелье регенерации"; }
	void GetCook() { wcout << L"Сварили зелье левитации"; }
};