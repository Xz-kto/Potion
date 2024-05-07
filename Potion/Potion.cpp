#include <iostream>
#include <vector>
#include "CCID.h"

using namespace std;

PotionContainer::PotionContainer(int maxSize)
{
    PotionBox = new Potion * [maxSize];

    for (int i = 0; i < maxSize; i++)
    {
        PotionBox[i] = NULL;
    }

    PotionCount = 0;
    MaxSize = maxSize;
}

PotionContainer::~PotionContainer()
{
    for (int i = 0; i < MaxSize; i++)
    {
        if (PotionBox[i] != NULL)
        {
            delete PotionBox[i];
            PotionBox[i] = NULL;
        }
    }

    delete[] PotionBox;
}

void PotionContainer::AddPotion(Potion* newPotion)
{
    PotionBox[PotionCount] = newPotion;
    PotionCount++;
}

wstring PrintPotionType(const PotionType type)
{
    switch (type)
    {
    case PotionType::Regular: return L"Обычное";
    case PotionType::Explosive: return L"Взрывное";
    case PotionType::Foggy: return L"Туманное";
    default: return L"Неизвестное";
    }
}

wstring PrintPotionEffect(const PotionEffect effect)
{
    switch (effect)
    {
    case PotionEffect::Regeneration: return L"Регенерация";
    case PotionEffect::Poisoning: return L"Отравление";
    case PotionEffect::Levitation: return L"Левитация";
    default: return L"Неизвестное";
    }
}

wstring PrintPotionClass(const PotionClass cl)
{
    switch (cl)
    {
    case PotionClass::Positive: return L"Позитивное";
    case PotionClass::Negative: return L"Негативное";
    case PotionClass::Neutral: return L"Нейтральное";
    default: return L"Неизвестное";
    }
}

void Task(Iterator<Potion*>* it)
{
    for (it->First(); !it->IsDone(); it->Next())
    {
        Potion* currentPotion = it->GetCurrent();
        wcout << PrintPotionType(currentPotion->GetType()) << endl;
        wcout << PrintPotionEffect(currentPotion->GetEffect()) << endl;
        wcout << PrintPotionClass(currentPotion->GetClass()) << endl;
        currentPotion->GetDrink();
        cout << endl << endl;
    }
}

int main()
{
    setlocale(LC_ALL, "Russian");
    wcout << L"Мир зелий" << endl << endl;

    PotionContainer potionBox(100);
    for (int i = 0; i < 25; i++)
    {
        potionBox.AddPotion(new Positive(static_cast<int>(1 + rand() % 3), static_cast<int>(3 + rand() % 6)));
    }
    for (int i = 0; i < 25; i++)
    {
        potionBox.AddPotion(new Negative(static_cast<int>(1 + rand() % 5), static_cast<int>(1 + rand() % 3)));
    }
    for (int i = 0; i < 25; i++)
    {
        potionBox.AddPotion(new Neutral(static_cast<int>(1 + rand() % 9), static_cast<int>(1 + rand() % 9)));
    }

    Iterator<Potion*>* it = new PotionTypeDecorator(potionBox.GetIterator(), PotionType::Explosive);

    Task(it);
    delete it;

    return 0;
}