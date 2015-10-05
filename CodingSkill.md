这是一个简单得无聊的问题, 又是一个困扰了很多年的问题.
```
if (A)
{
    if (B)
    {
         if (C)
         {
         }
         else
         {
            return -3;
         }
    }
    else
    {
        return -2;
    }
}
else
{
    return -1;
}
```

现在喜欢这么写,但是他们说这样程序有多出口. 不符合规范.
```
if (!A)
{
    return -1;
}

if (!B)
{
    return -2;
}

if (!C)
{
    return -3;
}
```

一般C语言都这么写, 但是有些人不推荐使用goto.
```
if (!create)
  goto createerr
if (!connect)
goto connecterr
if (!operation)
goto operr

operr:
connecterr:
createerr:
```

又在Chinaunix上学习到一个新的写法，人类智慧真是无穷的。
```
do
{
if(!a)  break;
//a真的情况
....
if(!b) break;
....
}
while(0);
```