package main

import (
	"bufio"
	"encoding/binary"
	"fmt"
	"io"
	"math/rand"
	"os"
	"strings"
	"time"
)

func generatePrime(min, max int) int {
	seed := time.Now().UnixNano()
	r := rand.New(rand.NewSource(seed))
	var prime int
	for {
		prime = r.Intn(max-min+1) + min
		if isPrime(prime) {
			break
		}
	}
	return prime
}

func generateSecretKey(p int) int {
	seed := time.Now().UnixNano() ^ int64(p)
	r := rand.New(rand.NewSource(seed))
	var key int
	for {
		key = r.Intn((p-2)-2+1) + 2
		if gcd(key, p-1) == 1 {
			break
		}
	}
	return key
}

func shamirProtocol(inNameFile *string, choice int) {
	var text, outNameFile string
	if choice == 1 {
		infile, err := os.Open(*inNameFile)
		if err != nil {
			return
		}
		defer infile.Close()
		reader := bufio.NewReader(infile)
		var sb strings.Builder
		for {
			b, err := reader.ReadByte()
			if err != nil {
				if err == io.EOF {
					break
				}
				return
			}
			sb.WriteByte(b)
		}
		text = sb.String()

		p := generatePrime(1000, 10000)
		fmt.Println("Простое число p =", p)
		Ca := generateSecretKey(p)
		Da := searchReciprocalNum(Ca, p-1)
		fmt.Println("Ключи Алисы: Ca =", Ca, ", Da =", Da)
		Cb := generateSecretKey(p)
		Db := searchReciprocalNum(Cb, p-1)
		fmt.Println("Ключи Боба: Cb =", Cb, ", Db =", Db)

		var encrypted []int32
		for _, c := range text {
			m := int(c)
			x1 := powMod(m, Ca, p)
			x2 := powMod(x1, Cb, p)
			x3 := powMod(x2, Da, p)
			encrypted = append(encrypted, int32(x3))
		}

		fmt.Print("Введите имя файла для сохранения: ")
		fmt.Fscanln(os.Stdin, &outNameFile)

		outfile, err := os.Create(outNameFile)
		if err != nil {
			return
		}
		defer outfile.Close()
		for _, num := range encrypted {
			_ = binary.Write(outfile, binary.LittleEndian, num)
		}

	} else if choice == 2 {
		infile, err := os.Open(*inNameFile)
		if err != nil {
			return
		}
		defer infile.Close()

		var p, Ca, Da, Cb, Db int
		fmt.Print("Введите простое число p: ")
		fmt.Fscanln(os.Stdin, &p)
		fmt.Print("Введите секретный ключ Алисы Ca: ")
		fmt.Fscanln(os.Stdin, &Ca)
		fmt.Print("Введите обратный ключ Алисы Da: ")
		fmt.Fscanln(os.Stdin, &Da)
		fmt.Print("Введите секретный ключ Боба Cb: ")
		fmt.Fscanln(os.Stdin, &Cb)
		fmt.Print("Введите обратный ключ Боба Db: ")
		fmt.Fscanln(os.Stdin, &Db)

		var encrypted []int32
		for {
			var num int32
			if err := binary.Read(infile, binary.LittleEndian, &num); err != nil {
				if err == io.EOF {
					break
				}
				return
			}
			encrypted = append(encrypted, num)
		}

		var decrypted strings.Builder
		for _, x := range encrypted {
			m := powMod(int(x), Db, p)
			decrypted.WriteRune(rune(m))
		}

		fmt.Print("Введите имя файла для сохранения: ")
		fmt.Fscanln(os.Stdin, &outNameFile)

		outfile, err := os.Create(outNameFile)
		if err != nil {
			return
		}
		defer outfile.Close()
		_, _ = outfile.WriteString(decrypted.String())
	}
}

func mitmAttack() {
	p := generatePrime(1000, 10000)
	fmt.Println("Простое число p =", p)
	Ca := generateSecretKey(p)
	Da := searchReciprocalNum(Ca, p-1)
	fmt.Println("Ключи Алисы: Ca =", Ca, ", Da =", Da)
	Cb := generateSecretKey(p)
	Db := searchReciprocalNum(Cb, p-1)
	fmt.Println("Ключи Боба: Cb =", Cb, ", Db =", Db)
	Cm := generateSecretKey(p)
	Dm := searchReciprocalNum(Cm, p-1)
	fmt.Println("Ключи Екатерины: Cm =", Cm, ", Dm =", Dm)

	fmt.Print("\nВведите сообщение для шифрования: ")
	reader := bufio.NewReader(os.Stdin)
	msgLine, _ := reader.ReadString('\n')
	message := strings.TrimSpace(msgLine)

	fmt.Println("\n=== ЭМУЛЯЦИЯ АТАКИ MitM ===")
	fmt.Println("1. Екатерина перехватывает сообщение от Алисы к Бобу")
	fmt.Println("2. Екатерина подменяет открытые ключи своими")

	var intercepted []int32
	var malloryDecrypted []int32
	fmt.Println("\nПроцесс перехвата и подмены:")
	for _, c := range message {
		m := int(c)
		x1 := powMod(m, Ca, p)
		fmt.Printf("Алиса -> Екатерине: %d ^ %d mod %d = %d\n", m, Ca, p, x1)
		x2 := powMod(x1, Dm, p)
		malloryMessage := powMod(x2, Cm, p)
		fmt.Printf("Екатерина расшифровала: %d ^ %d mod %d = %d\n", x1, Dm, p, x2)
		fmt.Printf("Екатерина знает исходное сообщение: %c\n", byte(x2))
		malloryDecrypted = append(malloryDecrypted, int32(x2))
		x3 := powMod(malloryMessage, Cb, p)
		intercepted = append(intercepted, int32(x3))
		fmt.Printf("Екатерина -> Бобу: %d ^ %d mod %d = %d\n", malloryMessage, Cb, p, x3)
	}

	fmt.Print("\nЕкатерина перехватила и расшифровала сообщение: ")
	for _, num := range malloryDecrypted {
		fmt.Printf("%c", num)
	}
	fmt.Println()

	var bobDecrypted strings.Builder
	fmt.Println("\nБоб расшифровывает (думая, что это от Алисы):")
	for _, x := range intercepted {
		m := powMod(int(x), Db, p)
		bobDecrypted.WriteByte(byte(m))
		fmt.Printf("Боб: %d ^ %d mod %d = %d -> '%c'\n", x, Db, p, m, byte(m))
	}

	fmt.Println("\nБоб получил (искаженное) сообщение:", bobDecrypted.String())
	fmt.Println("Екатерина успешно перехватила и прочитала сообщение!")
	modifiedMessage := "hack"
	fmt.Println("Екатерина изменяет сообщение на:", modifiedMessage)

	var modifiedEncrypted []int32
	for _, c := range modifiedMessage {
		m := int(c)
		encrypted := powMod(m, Cm, p)
		encrypted = powMod(encrypted, Cb, p)
		modifiedEncrypted = append(modifiedEncrypted, int32(encrypted))
	}

	var finalDecrypted strings.Builder
	for _, x := range modifiedEncrypted {
		m := powMod(int(x), Db, p)
		finalDecrypted.WriteByte(byte(m))
	}

	fmt.Println("Боб получает подмененное сообщение:", finalDecrypted.String())
	fmt.Println("✓ Атака MITM завершена успешно!")
}
