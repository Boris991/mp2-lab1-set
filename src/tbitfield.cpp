// ННГУ, ВМК, Курс "Методы программирования-2", С++, ООП
//
// tbitfield.cpp - Copyright (c) Гергель В.П. 07.05.2001
//   Переработано для Microsoft Visual Studio 2008 Сысоевым А.В. (19.04.2015)
//
// Битовое поле

#include "tbitfield.h"
#include "math.h"
#include <iostream>
TBitField::TBitField(int len)
{
    if (len < 0) throw "unable to create the BitField with negative length";
    BitLen = len;//размер универса
    MemLen = (len - 1) / (sizeof(int) * 8) + 1;
    pMem = new TELEM[MemLen];
    for (int i = 0; i < MemLen; i++)
    {
        pMem[i] = 0;
    }
}

TBitField::TBitField(const TBitField& bf) // конструктор копирования
{
    this->BitLen = bf.BitLen;
    this->MemLen = bf.MemLen;
    pMem = new TELEM[MemLen];
    for (int i = 0; i < MemLen; i++)
    {
        pMem[i] = bf.pMem[i];
    }
}

TBitField::~TBitField()
{
    delete[]pMem;
    MemLen = 0;
    BitLen = 0;
}
int TBitField::GetBit(const int n) const // получить значение бита
{
    if (n >= BitLen || n < 0) throw "wrong input variable";
    int bit;
    bit = (pMem[GetMemIndex(n)] & GetMemMask(n)) / GetMemMask(n);
    return bit;
    // return 0;
}
void TBitField::SetBit(const int n) // установить бит
{
    if (n >= BitLen || n < 0) throw "wrong input variable";
    pMem[GetMemIndex(n)] |= GetMemMask(n);
}
int TBitField::GetLength(void) const // получить длину (к-во битов)
{
    return BitLen;
}
void TBitField::ClrBit(const int n) // очистить бит
{
    if (n >= BitLen || n < 0) throw "wrong input variable";
    pMem[GetMemIndex(n)] =pMem[GetMemIndex(n)] & (~(GetMemMask(n)));;
}
int TBitField::GetMemIndex(const int n) const // индекс Мем для бита n
{
    return n / (sizeof(int) * 8);
}

TELEM TBitField::GetMemMask(const int n) const // битовая маска для бита n
{
    TELEM mask = 1 << (n % (sizeof(int) * 8));
    return mask;
}
// битовые операции
TBitField& TBitField::operator=(const TBitField& bf) // присваивание
{
        if (MemLen != 0)
        {
            delete[]pMem;
        }
        BitLen = bf.BitLen;
        MemLen = bf.MemLen;
        pMem = new TELEM[MemLen];
    for (int i = 0; i < MemLen; i++)
    {
        pMem[i] = bf.pMem[i];
    }
    return *this;
}
int TBitField::operator==(const TBitField& bf) const // сравнение
{
    if (this->BitLen != bf.BitLen)
    {
        return 0;
    }
    else
    { 
        for (int i = 0; i < this->MemLen; i++)
        {
            if (this->pMem[i] != bf.pMem[i])
            {
                return 0;
            }
        }
        return 1;
    }
    
    //return 0;
}
int TBitField::operator!=(const TBitField& bf) const // сравнение
{
    if (this->BitLen != bf.BitLen)
    {
        return 1;
    }
    else
    {
        for (int i = 0; i < this->MemLen; i++)
        {
            if (this->pMem[i] != bf.pMem[i])
            {
                return 1;
            }
        }
        return 0;
    }
    // return 0;
}

TBitField TBitField::operator|(const TBitField& bf) // операция "или"
{
    if (BitLen > bf.BitLen)
    {
        TBitField temp(BitLen);
        int r = MemLen - bf.MemLen;
        for (int i = 0; i < r; i++)
        {
            temp.pMem[i] = pMem[i];
        }
        for (int i = r; i < MemLen; i++)
        {
            temp.pMem[i] = pMem[i] | bf.pMem[i - r];
        }
        return temp;
    }
    else
    {
        TBitField temp(bf.BitLen);
        int sub = bf.MemLen - MemLen;

        for (int i = 0; i < sub; i++)
        {
            temp.pMem[i] = bf.pMem[i];
        }
        for (int i = sub; i < MemLen; i++)
        {
            temp.pMem[i] = bf.pMem[i] | pMem[i - sub];
        }
        return temp;
    }
}
TBitField TBitField::operator&(const TBitField& bf) // операция "и"
{
    if (BitLen > bf.BitLen)
    {
        TBitField temp(BitLen);
        int sub = MemLen - bf.MemLen;

        for (int i = 0; i < sub; i++)
        {
            temp.pMem[i] = 0;
        }
        for (int i = sub; i < MemLen; i++)
        {
            temp.pMem[i] = pMem[i] & bf.pMem[i - sub];
        }
        return temp;
    }
    else
    {
        TBitField temp(bf.BitLen);
        int r = bf.MemLen - MemLen;

        for (int i = 0; i < r; i++)
        {
            temp.pMem[i] = 0;
        }
        for (int i = r; i < MemLen; i++)
        {
            temp.pMem[i] = bf.pMem[i] & pMem[i - r];
        }
        return temp;
    }
    
}
TBitField TBitField::operator~(void) // отрицание
{
    TBitField temp(BitLen);
    for (int i = 0; i < BitLen; i++)
    {
        if (!GetBit(i)) temp.SetBit(i);
    }
    return temp;
    //TBitField temp(BitLen);
    //for (int i = 0; i < BitLen; i++)
    //{
    //    temp.pMem[i]= !(this->pMem[i]);
    //}
    //return temp;
}
// ввод/вывод

istream& operator>>(istream& istr, TBitField& bf) // ввод
{
    string bitstring;
    istr >> bitstring;
    TBitField bf1(bitstring.length());
    for (int i = 0; i < bitstring.length(); i++)
    {
        if (bitstring[i] == '1')
            bf1.SetBit(i);
    }
    bf = bf1;
    return istr;

}

ostream& operator<<(ostream& ostr, const TBitField& bf) // вывод
{
    ostr << " { ";
    for (int i = 0; i < bf.BitLen; i++)
        ostr << bf.GetBit(i) << " ";
    ostr << " } ";
    return ostr;
}
