package main

import (
	"bufio"
	"fmt"
	"os"
	"strconv"
	"strings"
)

func ValidInputMainMenu() int {
	reader := bufio.NewReader(os.Stdin)
	for {
		fmt.Print("Введите номер задания: \n1 - a^x mod p. \n2 - с*d mod m=1 (через u и v). \n3 - с^-1 mod m=d. \n4 - Криптопротокол Шамира. \n5 - Цепная дробь && диофантово уравнение. \n6 - МitM \nВаш выбор >>")
		input, _ := reader.ReadString('\n')
		input = strings.TrimSpace(input)

		numAlgo, err := strconv.Atoi(input)
		if err != nil {
			fmt.Println("Ошибка: введено не число. Попробуйте ещё раз.")
			continue
		}
		if numAlgo < 1 {
			fmt.Println("Ошибка: Номер алгоритма начинается с 1.")
			continue
		}
		if numAlgo > 6 {
			fmt.Println("Ошибка: Всего 6 алгоритмов.")
			continue
		}
		return numAlgo
	}
}
func exp() int {
	reader := bufio.NewReader(os.Stdin)
	for {
		fmt.Print("Введите число>>")
		input, _ := reader.ReadString('\n')
		input = strings.TrimSpace(input)

		numAlgo, err := strconv.Atoi(input)
		if err != nil {
			fmt.Println("Ошибка: введено не число. Попробуйте ещё раз.")
			continue
		}
		return numAlgo
	}
}

func degree() int {
	reader := bufio.NewReader(os.Stdin)
	for {
		fmt.Print("Введите степень числа >>")
		input, _ := reader.ReadString('\n')
		input = strings.TrimSpace(input)

		numAlgo, err := strconv.Atoi(input)
		if err != nil {
			fmt.Println("Ошибка: введено не число. Попробуйте ещё раз.")
			continue
		}
		return numAlgo
	}
}

func modul() int {
	reader := bufio.NewReader(os.Stdin)
	for {
		fmt.Print("Введите модуль >>")
		input, _ := reader.ReadString('\n')
		input = strings.TrimSpace(input)
		numAlgo, err := strconv.Atoi(input)
		if err != nil {
			fmt.Println("Ошибка: введено не число. Попробуйте ещё раз.")
			continue
		}
		return numAlgo
	}
}

func validInputPathFile() string {
	reader := bufio.NewReader(os.Stdin)
	for {
		fmt.Print("Введите полный путь до файла(если в одной директории,то достаточно названия файла) >>")
		input, _ := reader.ReadString('\n')
		input = strings.TrimSpace(input)
		return input
	}
}
