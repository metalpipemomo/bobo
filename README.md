# Bobo
## a game engine.

## How to get started
#### Step 1: Copy the following commands into your terminal.
```bash
git clone https://github.com/MoBaydoun/bobo.git
gen
```
This should clone the project and generate a Visual Studio 2022 project.

#### Step 2: Open the solution file.
Double click on the following file.
![image of bobo.sln](https://media.discordapp.net/attachments/308437507148742657/1150649088946208778/image.png?width=850&height=467)

#### Step 3: Build and run the project.
Click on the "Local Windows Debugger" button.
![image of vs2022](https://cdn.discordapp.com/attachments/308437507148742657/1150649912866906232/image.png)

### Congratulations! You can now start working on the engine 🎉
If you run into any issues regarding spdlog, you might need to run the following command in your terminal:
```bash
git submodule update --init --recursive
```

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