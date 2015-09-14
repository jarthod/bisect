package main

import (
  "os"
  "io"
  "bufio"
  "fmt"
)

func bisect(file io.Reader, from int, to int, pattern string) int {
  fmt.Println("bisect from", from, "to", to, " pattern: ", pattern)
  if from == to {
    fmt.Println("result is", to)
    return to
  }
  mid := (form + to) / 2
  fmt.Println("mid = ", mid)

  // seek to mid position
  _, err = f.Seek(mid, 0)
  if err != nil { panic(err) }

  // read until enf of line
  reader := bufio.NewReader(f)
  str, err := reader.ReadString('\n')
  if err != nil { panic(err) }

  // update mid position to be beginning of line
  mid += len(str)
  fmt.Println("mid adjusted = ", mid)

  // read complete line
  str, err = reader.ReadString('\n')
  if err != nil { panic(err) }

  // compare with pattern
  fmt.Println("current line:", str[3:23])
  if str[3:23] < pattern {
    fmt.Println("too low")
    return bisect(file, mid, to, pattern);
  } else {
    fmt.Println("too high")
    return bisect(file, from, mid, pattern);
  }
  return 0
}

func main() {
  pattern := "[2015-03-22T06:37:04"
  // Open log file
  f, err := os.Open("benchmark/production.log")
  if err != nil { panic(err) }

  // Close file on exit
  defer func() {
    if f.Close() != nil { panic(err) }
  }()

  _, err = f.Seek(10000, 0)
  if err != nil { panic(err) }

  reader := bufio.NewReader(f)
  str, err := reader.ReadString('\n')
  if err != nil { panic(err) }
  str, err = reader.ReadString('\n')
  if err != nil { panic(err) }

  // scanner := bufio.NewScanner(f)
  // for scanner.Scan() {
  //   fmt.Println(scanner.Text())
  // }
  // if err := scanner.Err(); err != nil { panic(err) }
}