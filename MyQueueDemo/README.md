#  如何设计一种完善的数据类型——以Queue为例
- step one：
  首先明确该类型的特征

  - 数据结构
    - 队列存储有序的项目序列————有序排列
    - 队列所能容纳的项目数有一定的限制
  - 类行为
    - 能够创建空队列
    - 能够检查队列是否为空
    - 能够检查队列是否已满
    - 能够在队尾添加项目
    - 能够在队首删除项目
    - 能够确定获取队列中的项目数目
    - 能够获取队列的size

- step two：
  根据类特征设计接口函数(行为)
  ```C++{.line-numbers}
  class Queue
  { 
      /*
       *    如何来限制队列的项目数呢？  
       *        Old C Style:    Macro
       *            #define LIMIT   20
       *        OOP Style(C++ Style):   const static    or      class enum
       *            1.const static int Limit = 20
       *            2.enum { Q_Limit = 20 };
      */
        enum { Q_SIZE = 10 };
    private:
        //  data struct
    public:
        //  behavior
        Queue(int qs = Q_SZIE );            //  default Queue Size constructor function
        ~Queue();                           //  destroy constructor function
        bool IsEmpty() const;               //  
        bool IsFull() const;
        bool EnQueue(const Item& item) const;
        bool DeQueue(Item& item) const;
        int QueueCount() const;
  }
  ```
- step three
  确定好接口后根据其接口行为来设计数据结构
  - 创建Queue类型行为
    两种数据类型：数组和链表
    - 数组
      牵扯到的数据移动太过复杂，与链表的先入先出行为不太符合；但是可以使用改良后的循环数组。
    - 链表
      方便，没有数据移动问题且与先入先出的队列锲合度比较高
    ```C++{.line-numbers}
    struct Node
    {
        Item item;
        struct Node* next;
    }
    ```
    每个项目的数据类型是确定了，但是根据相应的接口，为了方便管理这些数据，有增加了管理成员的一些基本类型

    - 整理
      ```C++{.line-numbers}
      class Queue
      {
        private:
            //  class scope definition
            //  Node is nested structure definition local to this class
            struct Node {Item item; struct Node* next};
            //  following data using for manager Node struct
            Node* front;        //  pointer to front of Queue
            Node* rear;         //  pointer to rear of Queue
            const int qsize;    //  maximum number of items in Queue
      }
      ```

    - 此处涉及到常量qsize的初始化问题
      一般认为是在调用类构造函数时来实现的初始化，其实并不是这样的。在构造函数被调用时，已经完成了对类成员的内存空间申请。所以执行的构造函数其实是一个赋值初始化的过程。对于普通变量而言，这样当然没有什么问题，但是对于cosnt限制的常量而言，在申请内存空间时就已经完成初始化了，但是又使用构造函数对常量进行修改，此时编译器就会报错。C++提供的解决方案时在申请内存空间时使用了一个成员初始化列表来解决这个问题。
      ```C++{.line-numbers}
      Queue::Queue(int qs) : qsize(qs)  //  initialize qsize to qs
      {
          front = rear = NULL;
          items = 0;
      }
      ```
      注意：常量的初始化位置也是很有讲究的，表明在调用构造函数时，刚进行参数复制以及内存申请的同时进行了初始化。

      这儿就有一个问题：既然成员初始化列表能够用于初始化const常量，那么能不能用来初始化变量呢？
      可以，使用逗号表达式实现
      ```C++{.line-numbers}
      classname::classname(int i, int j, int k) : m_i(i), m_j(j), m_k(k)
      {

      }
      ```
- supplement
  嵌套结构体与类
  在类声明中的声明结构体、类或者枚举都是被嵌套在类中的；所以作用域为整个类、这种声明不会创建具体的数据对象，而只是表明类可以使用这样的类型。————这一点与在.h文件中声明在.c/.cpp文件中使用并没有任何区别。
  其次，对于类嵌套类型，同样由于关键字private和public而存在两种状态。其中如果是定义在private中的数据类型声明，则只能在该类中使用——private需要保证数据安全。而public中声明的数据结构却可以开放给外部用来使用。————这这一点上，实现了类设计之初的兼容。
  其实对于我而言，这一点与普通的c语言很像。你在.c文件的头部如果使用了static限制的类型声明，则其他的文件是无法使用该数据类型的、但是如果你将该声明写入到了头文件中，别的文件在引入头文件的前提下，也是可以访问和使用该类型声明的。

- step three
  现在经过第一步和第二步，行为接口以及实现行为的数据结构体都已经实现了，可以开始来编写具体的行为实现代码。————类方法
  
  - supplement
    类中的static与const，需要拆分来认识到它们的差异
    - static
      表明该变量是静态存储的，即具有文件作用域————存在状态
    - const
      表明该变量是一个常量，不能对其进行值的修改
    - 在类中对static和const进行排列组合就得到了一下多种数据类型
      - static
        - 类静态成员函数
          一般好像没有这样使用过，我的理解是具有private属性的内联成员函数
        - 类静态成员变量
          具有整个程序生命周期的成员变量。
      - const
        - 类常量函数
          在函数头最后使用const来表明，该关键字说明，该函数不会对当前对象中的private数据做出任何更改
        - 类常量变量
          表明该类成员是一个常量，在整个对象的生命周期内不能对其做出任何更改
      - const static
          具有程序生命周期的类成员常量变量————需要在类的外部进行初始化，如果每次都是在调用构造函数时对其进行初始化，这样会造成重定义和修改const变量。
  - 总结：
    我的理解，将类视为一份独立的源文件，每次使用该类型时都会执行其相应的代码。————相当于函数调用，虽然执行的是同一份函数，但是其申请的执行该函数的空间是不一样的，所以可以嵌套多重调用。但是一旦类中涉及到了static关键字的变量，相当于函数调用中使用static关键字，该变量作用域虽然没变，但是其符号在整个函数执行空间将会是对一无二的。所以在使用该类来声明另外一个变量时，又会重新定义相同名称的具有程序生命周期的变量，这回导致重定义。