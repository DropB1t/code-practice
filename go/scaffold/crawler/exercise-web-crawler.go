package main

import (
	"fmt"
	"sync"
)

var wg sync.WaitGroup

type Fetcher interface {
	Fetch(url string, cache *URLsCache) (body string, urls []string, err error)
}

type fakeResult struct {
	body string
	urls []string
}

type fakeFetcher map[string]*fakeResult

type URLsCache struct {
	m    sync.Mutex
	data map[string]*fakeResult
}

func (f fakeFetcher) Fetch(url string, cache *URLsCache) (string, []string, error) {
	cache.m.Lock()
	if res, ok := cache.data[url]; ok {
		cache.m.Unlock()
		return res.body, res.urls, nil
	}
	cache.m.Unlock()

	if res, ok := f[url]; ok {
		cache.m.Lock()
		cache.data[url] = res
		cache.m.Unlock()
		return res.body, res.urls, nil
	}

	return "", nil, fmt.Errorf("not found: %s", url)
}

func Crawl(url string, depth int, fetcher Fetcher, cache *URLsCache) {
	defer wg.Done()
	if depth <= 0 {
		return
	}

	body, urls, err := fetcher.Fetch(url, cache)
	if err != nil {
		fmt.Println(err)
		return
	}
	fmt.Printf("found: %s %q\n", url, body)

	for _, u := range urls {
		wg.Add(1)
		go Crawl(u, depth-1, fetcher, cache)
	}
}

func main() {
	cache := URLsCache{data: make(map[string]*fakeResult)}
	wg.Add(1)
	go Crawl("https://golang.org/", 4, fetcher, &cache)
	wg.Wait()
}

var fetcher = fakeFetcher{
	"https://golang.org/": &fakeResult{
		"The Go Programming Language",
		[]string{
			"https://golang.org/pkg/",
			"https://golang.org/cmd/",
		},
	},
	"https://golang.org/pkg/": &fakeResult{
		"Packages",
		[]string{
			"https://golang.org/",
			"https://golang.org/cmd/",
			"https://golang.org/pkg/fmt/",
			"https://golang.org/pkg/os/",
		},
	},
	"https://golang.org/pkg/fmt/": &fakeResult{
		"Package fmt",
		[]string{
			"https://golang.org/",
			"https://golang.org/pkg/",
		},
	},
	"https://golang.org/pkg/os/": &fakeResult{
		"Package os",
		[]string{
			"https://golang.org/",
			"https://golang.org/pkg/",
		},
	},
}
