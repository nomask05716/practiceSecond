package main

import (
	"fmt"
	"math"
)

func isPrime(number int) bool {
	if number <= 1 {
		return false
	}
	if number == 2 {
		return true
	}
	if number%2 == 0 {
		return false
	}
	for num := 3; num <= int(math.Sqrt(float64(number))); num += 2 {
		if number%num == 0 {
			return false
		}
	}
	return true
}

func powMod(a, b, m int) int {
	result := 1
	a = a % m
	for b > 0 {
		// fmt.Println("Степень =", b)
		if b%2 == 1 {
			result = (a * result) % m
		}

		a = (a * a) % m
		// fmt.Println("a =", a)
		b /= 2
		// fmt.Println("Промежуточный результат =", result)

	}
	return result
}

func gcd(a, b int) int {
	for b != 0 {
		tmp := b
		b = a % b
		a = tmp
	}
	return a
}

func threoremFerma(a, b, p int) bool {
	if isPrime(p) && (gcd(a, p) == 1) && (p-b == 1) {
		return true
	} else if gcd(a, p) != 1 {
		fmt.Println("Число a должно быть взаимно простым с p")
		return false
	} else {
		return false
	}
}

func extendedGcd(a, b int, u, v *int) int {
	if b == 0 {
		*u = 1
		*v = 0
		return a
	}
	var u1, v1 int
	// fmt.Println("extendedGcd(", b, ",", a%b, ")")
	g := extendedGcd(b, a%b, &u1, &v1)
	// fmt.Println("u1 =", u1, ", v1 =", v1)
	*u = v1
	*v = u1 - (a/b)*v1
	// fmt.Println("Вычисляем новые коэффициенты:")
	// fmt.Println("u = v1 =", v1)
	// fmt.Println("v = u1 - (a/b)*v1 =", u1, "-(", a, "/", b, ")*", v1, "=", u1, "-", (a/b), "*", v1, "=", *v)
	return g
}

func searchReciprocalNum(c, m int) int {
	if m <= 0 {
		panic("Модуль должен быть положительным")
	}
	var u, v int
	g := extendedGcd(c, m, &u, &v)
	if g != 1 {
		panic("Уравнение не имеет решения: числа не взаимно просты")
	}
	d := (u%m + m) % m
	return d
}

func chainShotes(a, b int) []int {
	var coefficients []int
	for b != 0 {
		coefficients = append(coefficients, a/b)
		fmt.Printf("Целая часть: %d / %d = %d\n", a, b, a/b)
		remainder := a % b
		fmt.Printf("Остаток: %d %% %d = %d\n", a, b, remainder)
		temp := b
		b = remainder
		a = temp
		fmt.Printf("Новые значения: a = %d, b = %d\n", a, b)
	}
	return coefficients
}

func diophantineEquation() {
	var u, v int
	a, b, c := 275, 145, 10
	if c%gcd(a, b) == 0 {
		_ = extendedGcd(a, b, &u, &v)
	} else {
		fmt.Println("Решения в целых числах не существует")
	}
}
