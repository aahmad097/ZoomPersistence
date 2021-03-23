package main

import (

	"io/ioutil"
)

func main(){

	dayda := []byte("Hello from Builder!\n")
	ioutil.WriteFile("out.c", dayda, 0644)

}