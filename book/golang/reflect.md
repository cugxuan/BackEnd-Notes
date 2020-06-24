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

```go
package main

import (
	"fmt"
	"reflect"
)

type User struct {
	Id   int
	Name string
	Age  int
}

func (u User) ReflectCallFunc() {
	fmt.Println("Allen.Wu ReflectCallFunc")
}

func main() {
	user := User{1, "Allen.Wu", 25}

	DoFiledAndMethod(user)

}

// 通过接口来获取任意参数，然后一一揭晓
func DoFiledAndMethod(input interface{}) {

	getType := reflect.TypeOf(input)
	fmt.Println("get Type is :", getType.Name())

	getValue := reflect.ValueOf(input)
	fmt.Println("get all Fields is:", getValue)

	// 获取方法字段
	// 1. 先获取interface的reflect.Type，然后通过NumField进行遍历
	// 2. 再通过reflect.Type的Field获取其Field
	// 3. 最后通过Field的Interface()得到对应的value
	for i := 0; i < getType.NumField(); i++ {
		field := getType.Field(i)
		value := getValue.Field(i).Interface()
		fmt.Printf("%s: %v = %v\n", field.Name, field.Type, value)
	}

	// 获取方法
	// 1. 先获取interface的reflect.Type，然后通过.NumMethod进行遍历
	for i := 0; i < getType.NumMethod(); i++ {
		m := getType.Method(i)
		fmt.Printf("%s: %v\n", m.Name, m.Type)
	}
}

/* output

get Type is : User
get all Fields is: {1 Allen.Wu 25}
Id: int = 1
Name: string = Allen.Wu
Age: int = 25
ReflectCallFunc: func(main.User)
 */
```

通过运行结果可以得知获取未知类型的 interface 的具体变量及其类型的步骤为：

1. 先获取 interface 的 reflect.Type，然后通过 NumField 进行遍历
2. 再通过 reflect.Type 的 Field 获取其 Field
3. 最后通过 Field 的 Interface()得到对应的 value

通过运行结果可以得知获取未知类型的 interface 的所属方法（函数）的步骤为：

1. 先获取 interface 的 reflect.Type，然后通过 NumMethod 进行遍历
2. 再分别通过 reflect.Type 的 Method 获取对应的真实的方法（函数）
3. 最后对结果取其 Name 和 Type 得知具体的方法名
4. 也就是说反射可以将“反射类型对象”再重新转换为“接口类型变量”
5. struct 或者 struct 的嵌套都是一样的判断处理方式

## 通过 reflect.Value 设置实际变量的值

reflect.Value 是通过 reflect.ValueOf(X)获得的，**只有当 X 是指针的时候，才可以通过 reflec.Value 修改实际变量 X 的值**，即：要修改反射类型的对象就一定要保证其值是“addressable”的。

示例如下：

```go
package main

import (
	"fmt"
	"reflect"
)

func main() {

	var num float64 = 1.2345
	fmt.Println("old value of pointer:", num)

	// 通过reflect.ValueOf获取num中的reflect.Value，注意，参数必须是指针才能修改其值
	pointer := reflect.ValueOf(&num)
	newValue := pointer.Elem()

	fmt.Println("type of pointer:", newValue.Type())
	fmt.Println("settability of pointer:", newValue.CanSet())

	// 重新赋值
	newValue.SetFloat(77)
	fmt.Println("new value of pointer:", num)

	////////////////////
	// 如果reflect.ValueOf的参数不是指针，会如何？
	pointer = reflect.ValueOf(num)
	//newValue = pointer.Elem() // 如果非指针，这里直接panic，“panic: reflect: call of reflect.Value.Elem on float64 Value”
}

/* output

old value of pointer: 1.2345
type of pointer: float64
settability of pointer: true
new value of pointer: 77
*/
```

1. 需要传入的参数是\* float64 这个指针，然后可以通过 pointer.Elem()去获取所指向的 Value，注意一定要是指针。
2. 如果传入的参数不是指针，而是变量，那么
   - 通过 Elem 获取原始值对应的对象则直接 panic
   - 通过 CanSet 方法查询是否可以设置返回 false
3. newValue.CantSet()表示是否可以重新设置其值，如果输出的是 true 则可修改，否则不能修改，修改完之后再进行打印发现真的已经修改了。
4. reflect.Value.Elem() 表示获取原始值对应的反射对象，只有原始对象才能修改，当前反射对象是不能修改的
5. 也就是说如果要修改反射类型对象，其值必须是“addressable”【对应的要传入的是指针，同时要通过 Elem 方法获取原始值对应的反射对象】
6. struct 或者 struct 的嵌套都是一样的判断处理方式

## 通过 reflect.ValueOf 来进行方法的调用

这算是一个高级用法了，前面我们只说到对类型、变量的几种反射的用法，包括如何获取其值、其类型、如果重新设置新值。但是在工程应用中，另外一个常用并且属于高级的用法，就是通过 reflect 来进行方法【函数】的调用。比如我们要做框架工程的时候，需要可以随意扩展方法，或者说用户可以自定义方法，那么我们通过什么手段来扩展让用户能够自定义呢？关键点在于用户的自定义方法是未可知的，因此我们可以通过 reflect 来搞定

```go
package main

import (
	"fmt"
	"reflect"
)

type User struct {
	Id   int
	Name string
	Age  int
}

func (u User) ReflectCallFuncHasArgs(name string, age int) {
	fmt.Println("ReflectCallFuncHasArgs name: ", name, ", age:", age, "and origal User.Name:", u.Name)
}

func (u User) ReflectCallFuncNoArgs() {
	fmt.Println("ReflectCallFuncNoArgs")
}

// 如何通过反射来进行方法的调用？
// 本来可以用u.ReflectCallFuncXXX直接调用的，但是如果要通过反射，那么首先要将方法注册，也就是MethodByName，然后通过反射调动mv.Call

func main() {
	user := User{1, "Allen.Wu", 25}

	// 1. 要通过反射来调用起对应的方法，必须要先通过reflect.ValueOf(interface)来获取到reflect.Value，得到“反射类型对象”后才能做下一步处理
	getValue := reflect.ValueOf(user)

	// 一定要指定参数为正确的方法名
	// 2. 先看看带有参数的调用方法
	methodValue := getValue.MethodByName("ReflectCallFuncHasArgs")
	args := []reflect.Value{reflect.ValueOf("wudebao"), reflect.ValueOf(30)}
	methodValue.Call(args)

	// 一定要指定参数为正确的方法名
	// 3. 再看看无参数的调用方法
	methodValue = getValue.MethodByName("ReflectCallFuncNoArgs")
	args = make([]reflect.Value, 0)
	methodValue.Call(args)
}

/* output

ReflectCallFuncHasArgs name:  wudebao , age: 30 and origal User.Name: Allen.Wu
ReflectCallFuncNoArgs
*/
```

1. 要通过反射来调用起对应的方法，必须要先通过 reflect.ValueOf(interface)来获取到 reflect.Value，得到“反射类型对象”后才能做下一步处理
2. reflect.Value.MethodByName 这.MethodByName，需要指定准确真实的方法名字，如果错误将直接 panic，MethodByName 返回一个函数值对应的 reflect.Value 方法的名字。
3. []reflect.Value，这个是最终需要调用的方法的参数，可以没有或者一个或者多个，根据实际参数来定。
4. reflect.Value 的 Call 这个方法，这个方法将最终调用真实的方法，参数务必保持一致，如果 reflect.Value'Kind 不是一个方法，那么将直接 panic。
5. 本来可以用 u.ReflectCallFuncXXX 直接调用的，但是如果要通过反射，那么首先要将方法注册，也就是 MethodByName，然后通过反射调用 methodValue.Call

## Golang 的反射 reflect 性能

Golang 的反射很慢，这个和它的 API 设计有关。在 java 里面，我们一般使用反射都是这样来弄的。

```go
Field field = clazz.getField("hello");
field.get(obj1);
field.get(obj2);
```

这个取得的反射对象类型是 java.lang.reflect.Field。它是可以复用的。只要传入不同的 obj，就可以取得这个 obj 上对应的 field。

但是 Golang 的反射不是这样设计的:

```go
type_ := reflect.TypeOf(obj)
field, _ := type_.FieldByName("hello")
```

这里取出来的 field 对象是 reflect.StructField 类型，但是它没有办法用来取得对应对象上的值。如果要取值，得用另外一套对 object，而不是 type 的反射

```go
type_ := reflect.ValueOf(obj)
fieldValue := type_.FieldByName("hello")
```

这里取出来的 fieldValue 类型是 reflect.Value，它是一个具体的值，而不是一个可复用的反射对象了，每次反射都需要 malloc 这个 reflect.Value 结构体，并且还涉及到 GC。

Golang reflect 慢主要有两个原因

1. 涉及到内存分配以及后续的 GC；
2. reflect 实现里面有大量的枚举，也就是 for 循环，比如类型之类的。

## 总结

上述详细说明了 Golang 的反射 reflect 的各种功能和用法，都附带有相应的示例，相信能够在工程应用中进行相应实践，总结一下就是：

- 反射可以大大提高程序的灵活性，使得 interface{}有更大的发挥余地
  - 反射必须结合 interface 才玩得转
  - 变量的 type 要是 concrete type 的（也就是 interface 变量）才有反射一说
- 反射可以将“接口类型变量”转换为“反射类型对象”
  - 反射使用 TypeOf 和 ValueOf 函数从接口中获取目标对象信息
- 反射可以将“反射类型对象”转换为“接口类型变量
  - reflect.value.Interface().(已知的类型)
  - 遍历 reflect.Type 的 Field 获取其 Field
- 反射可以修改反射类型对象，但是其值必须是“addressable”
  - 想要利用反射修改对象状态，前提是 interface.data 是 settable,即 pointer-interface
- 通过反射可以“动态”调用方法
- 因为 Golang 本身不支持模板，因此在以往需要使用模板的场景下往往就需要使用反射(reflect)来实现

## 参考资料

- [Golang 的反射 reflect 深入理解和示例](https://studygolang.com/articles/12348?fr=sidebar)
