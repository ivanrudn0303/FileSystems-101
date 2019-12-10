package main

import (
    "io"
    "log"
    "net/http"
    "math/rand"
    "time"
)

func main() {
    rand.Seed(42)
    http.HandleFunc("/", func(w http.ResponseWriter, _ *http.Request) {
            rnd := rand.Intn(100)
            log.Print(rnd)
            if rnd == 0 {
                log.Print("Hard Request")
                time.Sleep(20 * time.Millisecond)
                io.WriteString(w, "Hard Request\n")
            } else {
                io.WriteString(w, "Easy Request\n")
            }
        },
    )
    log.Fatal(http.ListenAndServe(":8080", nil))
}
