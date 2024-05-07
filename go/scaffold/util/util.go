package util

import (
	"sync"
)

var allNodesWaitGroup sync.WaitGroup

func GoNode(f func()) {
	allNodesWaitGroup.Add(1)
	go func() {
		defer allNodesWaitGroup.Done()
		f()
	}()
}

func WaitForAllNodes() {
	allNodesWaitGroup.Wait()
}
