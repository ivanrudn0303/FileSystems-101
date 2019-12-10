package main

import (
    "fmt"
    "io/ioutil"
    "net/http"
    "time"
)

const REQUESTS = 16

func main() {
    for i := 0; i < 1000; i++ {
        start := time.Now()
        for k := 0; k < REQUESTS; k++ {
            resp, err := http.Get("http://127.0.0.1:8080/test")
            if err != nil {
                panic(err)
            }
            defer resp.Body.Close()
            _, err = ioutil.ReadAll(resp.Body)
            if err != nil {
                panic(err)
            }
        }
        duration := time.Since(start)
        fmt.Println(int64(duration))
    }
}
