package main

import "fmt"

// List represents a singly-linked list that holds
// values of any type.
type List[T comparable] struct {
	next *List[T]
	val  T
}

type ErrList struct {
	cause string
}

func (e ErrList) Error() string {
	return e.cause
}

func (l List[T]) find(x T) (*List[T], error) {
	if l.val == x {
		return &l, nil
	}
	if l.next == nil {
		return nil, ErrList{fmt.Sprintf("The value %v is not present in the List", x)}
	}
	return l.next.find(x)
}

func (l *List[T]) add(x T) *List[T] {
	return &List[T]{l, x}
}

func main() {
	l := &List[int]{nil, 2}
	l = l.add(5)

	node, err := l.find(2)

	if err != nil {
		fmt.Println(err)
		return
	}

	fmt.Println("Founded value:", node.val)

}
