# 数组和切片

## 数组

**数组的长度不可以改变**，对于 go 来说，**存储元素相同，大小不同的数组**是完全不同的，数组有两种初始化方式

```go
arr1 := [3]int{1, 2, 3}
arr2 := [...]int{1, 2, 3}
```

如果数组中元素的个数小于等于 4 个，那么所有的变量会直接在栈上初始化，如果大于 4 个，变量就会在静态存储区初始化然后拷贝到栈上。在创建数组的代码中可以看到，**数组不会被创建到堆上，数组拥有类型 Elem 和 容量 Bound 两个属性**

```go
func NewArray(elem *Type, bound int64) *Type {
	if bound < 0 {
		Fatalf("NewArray: invalid bound %v", bound)
	}
	t := New(TARRAY)
	t.Extra = &Array{Elem: elem, Bound: bound}
	t.SetNotInHeap(elem.NotInHeap())
	return t
}
```

## 切片

数组一般不常用，平时使用切片，即动态数组。创建切片的函数如下

```go
func NewSlice(elem *Type) *Type {
	if t := elem.Cache.slice; t != nil {
		if t.Elem() != elem {
			Fatalf("elem mismatch")
		}
		return t
	}

	t := New(TSLICE)
	t.Extra = Slice{Elem: elem}
	elem.Cache.slice = t
	return t
}
```

上述方法返回的结构体 TSLICE 中的 Extra 字段是一个只包含切片内元素类型的 Slice{Elem: elem} 结构，也就是说切片内元素的类型是在编译期间确定的，编译器确定了类型之后，会将类型存储在 Extra 字段中帮助程序在运行时动态获取。

编译期间的切片是 Slice 类型的，但是在运行时切片由如下的 SliceHeader 结构体表示，其中 Data 字段是指向数组的指针，Len 表示当前切片的长度，而 Cap 表示当前切片的容量，也就是 Data 数组的大小：

```go
type SliceHeader struct {
	Data uintptr
	Len  int
	Cap  int
}
```

如果切片发生逃逸或者非常大时，会被 [runtime.makeslice](https://github.com/golang/go/blob/440f7d64048cd94cba669e16fe92137ce6b84073/src/runtime/slice.go#L34-L50) 函数在堆上进行初始化。如果当前的切片不会发生逃逸并且切片非常小的时候，`make([]int, 3, 4)` 会被直接转换成如下所示的代码：

```go
var arr [4]int
n := arr[:3]
```

### 追加和扩容

在切片容量足够的时候，中间会拆分 `append` 关键字

```go
// append(slice, 1, 2, 3)
ptr, len, cap := slice
newlen := len + 3
if newlen > cap {
    ptr, len, cap = growslice(slice, newlen)
    newlen = len + 3
}
*(ptr+len) = 1
*(ptr+len+1) = 2
*(ptr+len+2) = 3
return makeslice(ptr, newlen, cap)
```

我们已经通过 append 关键字被转换的控制流了解了在切片容量足够时如何向切片中追加元素，但是当切片的容量不足时就会调用 runtime.growslice 函数为切片扩容，扩容就是为切片分配一块新的内存空间并将原切片的元素全部拷贝过去，我们分几部分分析该方法：

```go
func growslice(et *_type, old slice, cap int) slice {
	newcap := old.cap
	doublecap := newcap + newcap
	if cap > doublecap {
		newcap = cap
	} else {
		if old.len < 1024 {
			newcap = doublecap
		} else {
			for 0 < newcap && newcap < cap {
				newcap += newcap / 4
			}
			if newcap <= 0 {
				newcap = cap
			}
		}
	}
  // 到这里解决了切片容量的问题
```

在分配内存空间之前需要先确定新的切片容量，Go 语言根据切片的当前容量选择不同的策略进行扩容：

- 如果期望容量大于当前容量的两倍就会使用期望容量；
- 如果当前切片容量小于 1024 就会将容量翻倍；
- 如果当前切片容量大于 1024 就会每次增加 25% 的容量，直到新容量大于期望容量；

### 切片的拷贝

注意使用 copy 操作进行切片拷贝的时候，**需要 make 一个 len(src_slice) 的切片**，注意这里是深拷贝，从 slice1 的内存复制数据到 slice2

```go
slice2 := make([]string,len(slice1))
copy(slice2, slice1)
```

### 切片使用的坑

由于切片的数据结构，如果不使用 copy 函数，直接使用赋值，那么在不扩容的情况下，新旧切片使用的数据指针是同一个，修改会发生连锁反应

```go
func TestTest2(t *testing.T) {
	slice := []int{1, 2, 3, 4}
	slice1 := slice
	slice1[0] = 2
	fmt.Println(slice)
	fmt.Println(slice1)
}
/* output
[2 2 3 4]
[2 2 3 4]
 */
```

同样的思路作为函数参数，注意切片作为函数参数传入时，在函数内进行了切片扩展，那么外层切片将只会保留之前 cap 内的变化，一般的做法可以：① 传入切片的指针 ② 将扩容后的切片作为函数的返回值返回

### 多维切片

这里以二维切片为例，由于数组元素也是切片，那么在 append 的时候，每个切片元素会具有相同的指针（上面的 SliceHeader 结构介绍过），所以修改切片元素的时候会影响另外的位置，具体可以看这个例子

```go
func TestTest1(t *testing.T) {
	test_arr()
}
func test_arr() {
	a := []int{1, 2, 3}
	var c [][]int
	b := a
	c = append(c, b)
	c = append(c, b)
	fmt.Println(c)
	b[0] = 9
	fmt.Println(c)
	fmt.Println(a)
	change(a)
	fmt.Println(c)
	fmt.Println(b)
}

func change(cp []int) {
	cp[0] = 6
}
/* output
[[1 2 3] [1 2 3]]
[[9 2 3] [9 2 3]]
[9 2 3]
[[6 2 3] [6 2 3]]
[6 2 3]
 */
```

## 参考资料

- [draveness-数组](https://draveness.me/golang/docs/part2-foundation/ch03-datastructure/golang-array/)
- [draveness-切片](https://draveness.me/golang/docs/part2-foundation/ch03-datastructure/golang-array-and-slice/)
