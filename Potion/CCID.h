#ifndef CCIDH
#define CCIDH
#include "ITERDEC.h"
#include "sqlite3.h"
#include <fstream>
#include <iostream>
#include <vector>
#include <format>

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
	int GetLevel() { return PotionLevel; }
	int GetTimeEffect() const { return TimeEffect; }
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

class BaseContainer {
public:
	BaseContainer() {};
	~BaseContainer() {};
	virtual void AddPotion(Potion*) = 0;
	virtual int GetCount() = 0;
	virtual Iterator<Potion*>* GetIterator() = 0;
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

class PotionContainer : public BaseContainer
{
private:
	Potion** PotionBox;
	int PotionCount;
	int MaxSize;
	
public:
	PotionContainer(int maxSize);
	virtual ~PotionContainer();
	void AddPotion(Potion* newPotion);
	int GetCount() { return PotionCount; }
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

class VectorPotionCantainer : public BaseContainer
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

class sqlite3container : public BaseContainer 
{
private:
	sqlite3* database;
	int count;
public:
	sqlite3container()
	{
		count = 0;
		char* err = 0;
		ofstream dbf("potion.db");
		dbf.close();
		int result = sqlite3_open("potion.db", &database);
		if (result != SQLITE_OK)
		{
			wcout << L"Ошибка создания подключения к базе данных! Ошибка: " << sqlite3_errmsg(database) << endl;
			sqlite3_free(err);
			sqlite3_close(database);
			remove("potion.db");
			exit(-1);
		}
		const char* SQL = "CREATE TABLE Potions (id INTEGER PRIMARY KEY AUTOINCREMENT, type INTEGER, level INTEGER, effect INTEGER, class INTEGER, duration INTEGER)";
		result = sqlite3_exec(database, SQL, 0, 0, &err);
		if (result != SQLITE_OK)
		{
			wcout << L"Ошибка создания таблицы! Ошибка: " << err << endl;
			sqlite3_free(err);
			sqlite3_close(database);
			remove("potion.db");
			exit(-1);
		}
	}
	~sqlite3container()
	{
		sqlite3_close(database);
		remove("potion.db");
	}
	void AddPotion(Potion* newPotion)
	{
		char* err;
		string SQL = format("INSERT INTO Potions(type, level, effect, class, duration) VALUES({}, {}, {}, {}, {})", 
			static_cast<int>(newPotion->GetType()), 
			newPotion->GetLevel(), static_cast<int>(newPotion->GetEffect()), 
			static_cast<int>(newPotion->GetClass()), newPotion->GetTimeEffect());
		int result = sqlite3_exec(database, SQL.c_str(), 0, 0, &err);
		if (result != SQLITE_OK)
		{
			wcout << L"Ошибка добавления зелья в таблицу! Ошибка: " << err << endl;
			sqlite3_free(err);
			sqlite3_close(database);
			remove("potion.db");
			exit(-1);
		}
		count++;
		delete newPotion;
	}
	Potion* GetById(int id)
	{
		sqlite3_stmt* statement;
		string SQL = format("SELECT * FROM Potions WHERE id = {}", id);
		int result = sqlite3_prepare_v2(database, SQL.c_str(), -1, &statement, NULL);
		if (result != SQLITE_OK)
		{
			wcout << L"Ошибка выполнения запроса! Ошибка: " << sqlite3_errmsg(database) << endl;
			sqlite3_close(database);
			remove("potion.db");
			exit(-1);
		}
		result = sqlite3_step(statement);
		if (result == SQLITE_ROW)
		{
			PotionType type = static_cast<PotionType>(sqlite3_column_int(statement, 1));
			int pot_level = sqlite3_column_int(statement, 2);
			PotionEffect effect = static_cast<PotionEffect>(sqlite3_column_int(statement, 3));
			PotionClass pot_class = static_cast<PotionClass>(sqlite3_column_int(statement, 4));
			int TimeEffect = sqlite3_column_int(statement, 5);
			Potion* potion;
			switch (pot_class)
			{
			case PotionClass::Negative:
				potion = new Negative(TimeEffect, pot_level);
				break;
			case PotionClass::Neutral:
				potion = new Neutral(TimeEffect, pot_level);
				break;
			case PotionClass::Positive:
				potion = new Positive(TimeEffect, pot_level);
				break;
			default:
				wcout << L"Попалось неизвестное зелье, лучше выкинуть всю партию..." << endl;
				sqlite3_close(database);
				remove("potion.db");
				exit(-1);
			}
			return potion;
		}
		else
		{
			wcout << L"Не удалось получить зелье по идентификатору! Ошибка: " << sqlite3_errmsg(database) << endl;
			sqlite3_close(database);
			remove("potion.db");
			exit(-1);
		}
	}
	int GetCount() { return count; }
	Iterator<Potion*>* GetIterator();
};

class sqlite3contIterator : public Iterator<Potion*>
{
private:
	sqlite3container* cont;
	int Pos;
public:
	sqlite3contIterator(sqlite3container* pcont)
	{
		cont = pcont;
		Pos = 1;
	}
	void First() { Pos = 1; }
	void Next() { Pos++; }
	bool IsDone() const { return Pos > cont->GetCount(); }
	Potion* GetCurrent() const { return cont->GetById(Pos); }
};

Iterator<Potion*>* sqlite3container::GetIterator() { return new sqlite3contIterator(this); }

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

#endif //CCIDH