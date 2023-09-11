# Bobo
## a game engine.

Will add any necessary config steps here.

## Coding Conventions

- Brackets are always new-line.
- Locally scoped variables are camelCased.
- Function names are PascalCased.
- Static variables must be prefixed with "s_" and the remaining in PascalCase.
- Pointer variables must be prefixed with "p_" and the remaining in PascalCase.
- Member variables must be prefixed with "m_" and the remaining in PascalCase.

```c++
// Brackets:
if (true)
{

}

// Variables:

int main()
{
    int camelCase = 5; //local
}

class Example
{
public:
    void Foo();
    void Bar()
    {
        bool localVariable = true;
    }
private:
    static int s_StaticVariable;
    int* p_PointerVariable;
    int m_MemberVariable;

    static int* s_StaticPointerVariable; // static supersedes pointer. s -> p -> m
}
```