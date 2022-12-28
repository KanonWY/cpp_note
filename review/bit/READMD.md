### Bit Code

#### 1. Basic

`Decimal`

```bash
normal person use in live.
int a = 199; //c is 199
```

 `Binary`

```bash
use in computer, every code in computer use Binary.
in C++, append front 0b or 0B in number, it just a Binary number.
int b = 0;
b = 0b1;			// b is 1
b = 0b11;			// b is 3
b = 0B111			// b is 7
b = 0B1111			// b is 15
b = 0B11111			// b is 31
b = 0B111111		// b is 63
b = 0B1111111		// b is 127
b = 0B11111111 		// b is 255
```

 `Octal`

```bash
in C++, append front 0 in number, it just a Octal number.
for example:
int c = 011;    //actual a is 9.
```

 `Hex`

```bash
in C++, append front 0x  or 0X in number, it just a Hex number.
twe Hex bit is a Byte. So a short is Twe Byte, a short is  four Hex bit.
for example:
int d = 0xff;  //0B11111111 = 255
d = 0xfff	   //0B111111111111 = 4095
d = 0xffff	   //0B1111111111111111 = 65535
```

#### 2 Decimal to other

- Decimal to Binary

```cpp
template<typename T>
std::string DecimalToBinary(T a){
    
}
```





