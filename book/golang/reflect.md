# golang 反射

## 编程语言中反射的概念

在计算机科学领域，反射是指一类应用，它们能够自描述和自控制。也就是说，这类应用通过采用某种机制来实现对自己行为的描述（self-representation）和监测（examination），并能根据自身行为的状态和结果，调整或修改应用所描述行为的状态和相关的语义。

每种语言的反射模型都不同，并且有些语言根本不支持反射。Golang 语言实现了反射，反射机制就是在运行时动态的调用对象的方法和属性，官方自带的 reflect 包就是反射相关的，只要包含这个包就可以使用。

多插一句，Golang 的 gRPC 也是通过反射实现的。

## interface 和 反射

- 变量包括（type, value）两部分
- type 包括 static type 和 concrete type. 简单来说 static type 是你在编码是看见的类型(如 int、string)，concrete type 是 runtime 系统看见的类型
- 类型断言能否成功，取决于变量的 concrete type，而不是 static type. 因此，一个 reader 变量如果它的 concrete type 也实现了 write 方法的话，它也可以被类型断言为 writer.

接下来要讲的反射，就是建立在类型之上的，Golang 的指定类型的变量的类型是静态的（也就是指定 int、string 这些的变量，它的 type 是 static type），在创建变量的时候就已经确定，反射主要与 Golang 的 interface 类型相关（它的 type 是 concrete type），只有 interface 类型才有反射一说。

## reflect 的基本功能 TypeOf 和 ValueOf

既然反射就是用来检测存储在接口变量内部(值 value；类型 concrete type) pair 对的一种机制。那么在 Golang 的 reflect 反射包中有什么样的方式可以让我们直接获取到变量内部的信息呢？ 它提供了两种类型（或者说两个方法）让我们可以很容易的访问接口变量内容，分别是 reflect.ValueOf() 和 reflect.TypeOf()，看看官方的解释

```go
// ValueOf returns a new Value initialized to the concrete value
// stored in the interface i.  ValueOf(nil) returns the zero
func ValueOf(i interface{}) Value {...}

翻译一下：ValueOf用来获取输入参数接口中的数据的值，如果接口为空则返回0

// TypeOf returns the reflection Type that represents the dynamic type of i.
// If i is a nil interface value, TypeOf returns nil.
func TypeOf(i interface{}) Type {...}

翻译一下：TypeOf用来动态获取输入参数接口中的值的类型，如果接口为空则返回nil
```

`reflect.TypeOf()`是获取 pair 中的 type，`reflect.ValueOf()`获取 pair 中的 value，举个例子

```go
package main

import (
	"fmt"
	"reflect"
)

type Node struct {
	A string `json:"a"`
	B int    `json:"b"`
	C byte   `json:"c"`
}

func main() {
	var num float64 = 1.2345

	fmt.Println("type: ", reflect.TypeOf(num))
	fmt.Println("value: ", reflect.ValueOf(num))

	var node Node = Node{
		A: "test",
		B: 1,
		C: 2,
	}
	fmt.Println("type: ", reflect.TypeOf(node))
	fmt.Println("value: ", reflect.ValueOf(node))
}

/*ouput

type:  float64
value:  1.2345
type:  main.Node
value:  {test 1 2}
*/
```

## 从 relfect.Value 中获取接口 interface 的信息

可以使用强制类型转化 `realValue := value.Interface().(已知的类型)` 来得到对应的变量，需要注意的是，**转换的时候，要区分是指针还是变量，如果转换的类型不完全符合，则直接 panic，类型要求非常严格！**

```go
package main

import (
	"fmt"
	"reflect"
)

func main() {
	var num float64 = 1.2345

	pointer := reflect.ValueOf(&num)
	value := reflect.ValueOf(num)

	// 可以理解为“强制转换”，但是需要注意的时候，转换的时候，如果转换的类型不完全符合，则直接panic
	// Golang 对类型要求非常严格，类型一定要完全符合
	// 如下两个，一个是*float64，一个是float64，如果弄混，则会panic
	convertPointer := pointer.Interface().(*float64)
	convertValue := value.Interface().(float64)

	fmt.Println(convertPointer)
	fmt.Println(convertValue)
}

/* output

0xc0000140a0
1.2345
 */
```

## 未知原有类型【遍历探测其 Filed】

很多情况下，我们可能并不知道其具体类型，那么这个时候，该如何做呢？需要我们进行遍历探测其 Filed 来得知，示例如下:
