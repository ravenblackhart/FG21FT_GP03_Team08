#include "ZomEquationBPLib.h"

UZomEquationBPLib::UZomEquationBPLib()
{

}

float UZomEquationBPLib::RamEquation(int n)
{
	return (1.f - FMath::Pow(n + 1.f, -0.6f)) / 0.69f;
}
