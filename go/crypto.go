package main

import (
	"bufio"
	"encoding/binary"
	"fmt"
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

func shamirProtocol(inNameFile string) {
	infile, err := os.Open(inNameFile)
	if err != nil {
		fmt.Println("Ошибка открытия файла!")
		return
	}
	defer infile.Close()

	var sb strings.Builder
	reader := bufio.NewReader(infile)
	for {
		b, err := reader.ReadByte()
		if err != nil {
			break
		}
		sb.WriteByte(b)
	}
	text := sb.String()

	p := generatePrime(1000, 10000)
	fmt.Println("Простое число p =", p)

	Ca := generateSecretKey(p)
	Da := searchReciprocalNum(Ca, p-1)
	fmt.Println("Ключи Алисы: Ca =", Ca, ", Da =", Da)

	Cb := generateSecretKey(p)
	Db := searchReciprocalNum(Cb, p-1)
	fmt.Println("Ключи Боба: Cb =", Cb, ", Db =", Db)

	var x1Values, x2Values, x3Values, x4Values []int

	fmt.Println("\nШаг 1: Алиса вычисляет x1 = m^Ca mod p")
	for _, c := range text {
		m := int(c)
		x1 := powMod(m, Ca, p)
		x1Values = append(x1Values, x1)
	}
	fmt.Print("Введите имя файла для сохранения x1: ")
	var x1NameFile string
	fmt.Scanln(&x1NameFile)
	saveToFileInt(x1NameFile, x1Values)
	fmt.Println("Промежуточный результат x1 сохранен в файл:", x1NameFile)

	fmt.Println("Шаг 2: Боб вычисляет x2 = x1^Cb mod p")
	for _, x1 := range x1Values {
		x2 := powMod(x1, Cb, p)
		x2Values = append(x2Values, x2)
	}
	fmt.Print("Введите имя файла для сохранения x2: ")
	var x2NameFile string
	fmt.Scanln(&x2NameFile)
	saveToFileInt(x2NameFile, x2Values)
	fmt.Println("Промежуточный результат x2 сохранен в файл:", x2NameFile)

	fmt.Println("Шаг 3: Алиса вычисляет x3 = x2^Da mod p")
	for _, x2 := range x2Values {
		x3 := powMod(x2, Da, p)
		x3Values = append(x3Values, x3)
	}
	fmt.Print("Введите имя файла для сохранения x3: ")
	var x3NameFile string
	fmt.Scanln(&x3NameFile)
	saveToFileInt(x3NameFile, x3Values)
	fmt.Println("Промежуточный результат x3 сохранен в файл:", x3NameFile)

	fmt.Println("Шаг 4: Боб вычисляет x4 = x3^Db mod p")
	var decryptedText strings.Builder
	for _, x3 := range x3Values {
		x4 := powMod(x3, Db, p)
		x4Values = append(x4Values, x4)
		decryptedText.WriteRune(rune(x4))
	}
	fmt.Print("Введите имя файла для сохранения x4: ")
	var x4NameFile string
	fmt.Scanln(&x4NameFile)
	saveToFileInt(x4NameFile, x4Values)
	fmt.Println("Итоговый результат x4 сохранен в файл:", x4NameFile)

	fmt.Print("Введите имя файла для сохранения расшифрованного текста: ")
	var decryptedFile string
	fmt.Scanln(&decryptedFile)
	err = os.WriteFile(decryptedFile, []byte(decryptedText.String()), 0644)
	if err != nil {
		fmt.Println("Ошибка создания файла с расшифрованным текстом!")
		return
	}
	fmt.Println("Расшифрованный текст сохранен в файл:", decryptedFile)
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
func saveToFileInt(filename string, values []int) {
	outfile, err := os.Create(filename)
	if err != nil {
		fmt.Println("Ошибка создания файла:", filename)
		return
	}
	defer outfile.Close()

	for _, v := range values {
		_ = binary.Write(outfile, binary.LittleEndian, int32(v))
	}
}
