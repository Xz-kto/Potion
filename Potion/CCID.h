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

class PotionContainerIterator : public Iterator<Potion*>
{
private:
	Potion** PotionBox;
	int Count;
	int Pos;

public:
	PotionContainerIterator(Potion** potionBox, int count)
	{
		PotionBox = potionBox;
		Count = count;
		Pos = 0;
	}

	void First() { Pos = 0; }
	void Next() { Pos++; }
	bool IsDone() const { return Pos >= Count; }
	Potion* GetCurrent() const { return PotionBox[Pos]; }
};

class PotionContainer
{
private:
	Potion** PotionBox;
	int PotionCount;
	int MaxSize;
	
public:
	PotionContainer(int maxSize);
	virtual ~PotionContainer();
	void AddPotion(Potion* newPotion);
	int GetCount() const { return PotionCount; }
	Potion* GetByIndex(int index) const { return PotionBox[index]; }

	Iterator<Potion*>* GetIterator()
	{
		return new PotionContainerIterator(PotionBox, PotionCount);
	}
};

class VectorPotionBoxIterator : public Iterator<Potion*>
{
private:
	vector<Potion*> PotionBox;
	int Count;
	int Pos;

public:
	VectorPotionBoxIterator(vector<Potion*> potionBox, int count)
	{
		PotionBox = potionBox;
		Count = count;
		Pos = 0;
	}
	void First() { Pos = 0; }
	void Next() { Pos++; }
	bool IsDone() const { return Pos >= Count; }
	Potion* GetCurrent() const { return PotionBox[Pos]; }
};

class VectorPotionCantainer
{
private:
	vector<Potion*> PotionBox;
	int Count;

public:
	void AddPotion(Potion* newPotion) { PotionBox.push_back(newPotion); }
	int GetCount() const { return PotionBox.size(); }
	Potion* GetByIndex(int index) const { return PotionBox[index]; }

	Iterator<Potion*>* GetIterator()
	{
		return new VectorPotionBoxIterator(PotionBox, Count);
	}
};

class PotionTypeDecorator : public IteratorDecorator<Potion*>
{
private:
	PotionType TargetType;
public:
	PotionTypeDecorator(Iterator<Potion*>* it, PotionType targetType) : IteratorDecorator(it)
	{
		TargetType = targetType;
	}

	void First()
	{
		It->First();
		while (!It->IsDone() && It->GetCurrent()->GetType() != TargetType)
		{
			It->Next();
		}
	}

	void Next()
	{
		do
		{
			It->Next();
		}
		while (!It->IsDone() && It->GetCurrent()->GetType() != TargetType);
	}
};

class PotionEffectDecorator : public IteratorDecorator<Potion*>
{
private:
	PotionEffect TargetEffect;
public:
	PotionEffectDecorator(Iterator<Potion*>* it, PotionEffect targetEffect) : IteratorDecorator(it)
	{
		TargetEffect = targetEffect;
	}

	void First()
	{
		It->First();
		while (!It->IsDone() && It->GetCurrent()->GetEffect() != TargetEffect)
		{
			It->Next();
		}
	}

	void Next()
	{
		do
		{
			It->Next();
		}
		while (!It->IsDone() && It->GetCurrent()->GetEffect() != TargetEffect);
	}
};

class PotionClassDecorator : public IteratorDecorator<Potion*>
{
private:
	PotionClass TargetClass;
public:
	PotionClassDecorator(Iterator<Potion*>* it, PotionClass targetClass) : IteratorDecorator(it)
	{
		TargetClass = targetClass;
	}

	void First()
	{
		It->First();
		while (!It->IsDone() && It->GetCurrent()->GetClass() != TargetClass)
		{
			It->Next();
		}
	}

	void Next() {
		do 
		{
			It->Next();
		} 
		while (!It->IsDone() && It->GetCurrent()->GetClass() != TargetClass);
	}
};

#endif