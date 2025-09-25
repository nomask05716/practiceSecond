package main

import (
	"bufio"
	"fmt"
	"os"
	"strings"
)

const (
	decompositionDegree = 1
	euclidAlgo          = 2
	euclidAlgoForC      = 3
	cryptoProtocol      = 4
	chainSht            = 5
	MitM                = 6
)

func main() {
	running := true
	var choiceMenu int
	reader := bufio.NewReader(os.Stdin)

	for running {
		choiceMenu = ValidInputMainMenu()
		switch choiceMenu {
		case decompositionDegree:
			footing := exp()
			degr := degree()
			mod := modul()
			ferma := threoremFerma(footing, degr, mod)
			if ferma {
				fmt.Println("a^x mod p = 1")
			} else {
				res := powMod(footing, degr, mod)
				fmt.Println("a^x mod p =", res)
			}
		case euclidAlgo:
			var u, v int
			a := exp()
			b := exp()
			_ = extendedGcd(a, b, &u, &v)
			fmt.Println("u = ", u, " v = ", v)
		case euclidAlgoForC:
			c := exp()
			mod := modul()
			d := searchReciprocalNum(c, mod)
			fmt.Println("d = ", d)
		case cryptoProtocol:
			pathFile := validInputPathFile()
			shamirProtocol(pathFile)
		case chainSht:
			a := exp()
			b := exp()
			res := chainShotes(a, b)
			fmt.Print(" [")
			for i, val := range res {
				if i < len(res)-1 {
					fmt.Printf("%d, ", val)
				} else {
					fmt.Printf("%d", val)
				}
			}
			fmt.Println("]")
			diophantineEquation()
		case MitM:
			mitmAttack()
		}

		fmt.Print("\nХотите продолжить? (y/n): ")
		line, _ := reader.ReadString('\n')
		line = strings.TrimSpace(line)
		var continueChoice string
		if len(line) > 0 {
			continueChoice = line[:1]
		} else {
			cont, _ := reader.ReadString('\n')
			cont = strings.TrimSpace(cont)
			if len(cont) > 0 {
				continueChoice = cont[:1]
			}
		}

		if continueChoice != "y" && continueChoice != "Y" {
			running = false
			fmt.Println("Выход из программы...")
		}
	}
}
