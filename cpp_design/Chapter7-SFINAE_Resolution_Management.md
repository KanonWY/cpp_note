### 1.Function overloading
```cpp
void f(int i);                                    //1
void f(long i, long j);                           //2
void f(double i, double j = 0);                   //3

f(1);     //1
f(5l,5);  //2
f(5, 5);  //? ambiguous!
f(5.0);   //3
f(5.0f);  //3 float => double is ok!
f(5l);    // ambiguous ==>  long to int? long to double? ambiguous!
```
The third function call, 2 or 3 ? While the conversion from int to long may seem better
than the one from int to double , C++ does not see it this way. This call is ambiguous.  
The last call is again ambiguous—both conversions to
double and to int are considered of equal weight, thus the first and third overloads are
equally good.
### 2. Template function
Template function, which parameter type are deduced from the type of the arguments at the call site.
we need to learn about resolution in the presence of template function.
consider the following example:
```cpp
void f(int x);               //1.
void f(long x);              //2.
template<typename T>         //3
void f(T i)
{}

f(5);       //1
f(5l);      //2
f(5.0);     //3
```
The first call, if  an exact match to a non-template function is found in  the overload set, it's always considered the best overload.  
The template function can also be instantiated with an exact match, but non-template function that matches exactly is  consider a better overload.
The third call, the double type that can be converted to int or long, or substituted for T to make the template instantiation an excat match.So the 
template function instantiated to an exact match is the next best overload  and is selected.  
multiple template function that can have their template parameters substituted to match the argument types of call?
```cpp
void f(int i);    //1.
template<typename T>
void f(T i)       //2.
{}
template<typename T>
void f(T* i)      //3.
{}

f(5);         //1
f(5l);        //2
int a  = 200;
f(&a);        //3
```
the first call is again an exact match to  the non-template function, and so is resloved.
the second call is overload exactly the template function 2.
the third call can aslo overload excatly by template function 2, and template function 3. But the template function 3 can only match calls with pointer
function. So base the princple of narrower overload is consider a better, we choose template function 3 to match.  
This is a new notion, specific to templates—instead of choosing better conversions
(in general, fewer or simpler conversions), we select the overload that is harder to instantiate.

Finally, there is one more kind of  function that can match any function all with the same name which take variable arguments.
```cpp
void f(int i);  //1
void f(...) {};

```
### 3. Type subsititution in template function
we must carefully differentiate the two steps in instantiating a template function to match a particular call:     
(1) the types of the template parameters are deduced from the argument types(type deduction)   
(2) Once the types are dudected, the concrete types are subsitituted for all parameter type.(type substitution).  
#### type dudection and substitution
Dudection is the process of guessing(what should the template type or types, be in order to match the call).
an example to understand the type dudection:
```cpp
template<typename T>
void f(T i, T* p)
{}

int i;
f(5, &i);     // T dudect to int.
f(5l, &i);    // ambiguous!
```
The second call, first parameter is dudected as long, but the second parameter is dudected as int.So type deduction fails as ambiguous.
One important detail: **conversions are not considered whe deduceting template types**.
The ambiguous deduction can be resolved by explicitly specifying the template type which removes the need for type deduction:
```cpp
f<int>(5l, &i);       //T = int.
```
Template function can have default arguments, just like non-template function, however, the values of these arguments are not used to deduce types.
```cpp
void f(int i, int j = 2);   //1
template<typename T>
void f(T i, T* p = NULL);   //2

int i = 0;
f(5);       // 1
f(5l);      // 2
```
Type deduction can be  ambiguous when different concrete type can be deduced from different arguments. if this happens, it means we have failed 
to deduce the argument types and cannot use this template function. Type substitution is never ambiguous-once we know what T is, we simply substitution
that type   every time we see T in the function definition. This process can also fail, but in a different way.
#### Substitution failure
Once we have deduced the template parameter types, type substitution is a pureluy mechanical process. But Not all types are created equal, and some allow
more liberties than others. Consider below code:
```cpp
template<typename T>
void f(T i, typename T::t& j) {  
}

template<typename T>
void f(T i, T j) {
}

struct A
{
    struct t {int i;};
    t i;
};

A a{5};
f(a,a.i);   //T A
f(5, 7);    /T int
```
#### Substitution Failure Is Not An Error
The rule that a substitution failure arising from an expression that would be invaild with the specified or deduced types does not make the whole program
invalid is known as SFINAE.This rule is essential for using template functions in C++; without SFINAE, it would be impossible to write many otherwise 
perfectly valid programs.
#### Taking control of overload resolution
The rule that a failure of the template argument substitution is not an error—the SFINAE rule—had to be added to the language simply to make certain narrowly defined template functions possible. But the ingenuity of a C++ programmer knows no bounds, and so SFINAE was repurposed and exploited to manually control the overload set by intentionally causing substitution failures.  
Let's consider in detail how SFINAE can be used to knock out an undesirable overload from the overload set.
##### basic SFINAE
consider such question: we have some general code that we want to call for all object, except for built-in types.normal solution is that for built-in types we have special code.But we can use SFINAE to simpler to somehow test whether our type is a class or not.










