package main

import (
	"fmt"

	"golang.org/x/tour/tree"
)

func Walk(t *tree.Tree, ch chan int) {
	if t.Left != nil {
		Walk(t.Left, ch)
	}
	ch <- t.Value
	if t.Right != nil {
		Walk(t.Right, ch)
	}
}

func Same(t1, t2 *tree.Tree) bool {
	ct1 := make(chan int, 10)
	ct2 := make(chan int, 10)

	go func() {
		Walk(t1, ct1)
		defer close(ct1)
	}()

	go func() {
		Walk(t2, ct2)
		defer close(ct2)
	}()

	for {
		v1, ok1 := <-ct1
		v2, ok2 := <-ct2

		if v1 != v2 {
			return false
		}

		if !ok1 && !ok2 {
			break
		}

	}

	return true
}

func main() {
	t1, t2 := tree.New(3), tree.New(3)

	fmt.Println(Same(t1, t2))
}
