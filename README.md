# Bobo, a C++ Game Engine & Totally Accurate Pool Simulator 🎱

Bobo is a Game Engine written entirely in C++.

Additionally, **Totally Accurate Pool Simulator**, a simple pool simulator, has been made utilizing the Bobo engine to show what it is capable of.

## How to get started
Commands in code blocks are to be typed into your terminal.

#### Step 1: Clone the repo
```bash
git clone https://github.com/MoBaydoun/bobo.git
```

#### Step 2: Ensure you have CMake installed
```bash
cmake
```
If CMake is installed, some information on its usage will be provided after running this command. If it is not installed, you'll instead receive output along the lines of "'cmake' is not recognized as an internal or external command".

If this is the case, head on over to [cmake.org](https://cmake.org/download/) and install CMake. Any recent x64 Windows version should work.

#### Step 3: Run build.bat
Now that you have CMake installed, open the newly cloned "bobo" folder and locate the file named build.bat within. Double-click on this file to run it. Doing so will build the project and place those built files into a newly created "build" folder. 

Additionally, the newly built solution contained in that "build" folder will be opened in your default version of Visual Studio.

Alternatively, run the following commands to do the above from within your terminal:
```bash
cd bobo
build.bat
```

#### Step 4: Build and run the project.
Click on the "Local Windows Debugger" button.
![image of vs2022](https://cdn.discordapp.com/attachments/308437507148742657/1150649912866906232/image.png)

If Visual Studio errors out after clicking the "Local Windows Debugger" button with an error along the lines of: "The build tools for v143 cannot be found", you may be on a version of Visual Studio not compatible with our project. Try to open the solution in Visual Studio 2022 instead and it should work.

### Congratulations! You can now start working on the project 🎉

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

## Branching
Now that you've made it this far and are ready to make some changes, start by creating your own branch.
(P.S: when I use brackets, it is to show where to insert text, there should be no actual brackets)
#### First, you want to make sure that you are on the right branch:
```bash
git branch
```
If you are on the **main** branch, then you are on the right track!
Then, we will create our own branch with the following command:
```bash
git checkout -b [name of branch]
```
#### Branch Naming Conventions
Just to keep our GitHub tidy and our commit history clean, the following naming conventions will be enforced:
```bash
feature/[feature-title-separated-by-hyphens]
bugfix/[brief-bug-description-separated-by-hyphens]
misc/[janitorial-changes-separated-by-hyphens]
```
The changes will be enforced by a git hook, so that should help you to avoid making any mistakes.
Also, keep your name out of the branch name.

#### Once you're done with your work, you can start a pull request:
First, you need to add, commit, and push your changes.
```bash
git add *
git commit -m "A descriptive message, providing an overview of the changes you are committing."
git push origin [name of branch]
```
Then you can head over to our GitHub page and start click the green merge request button that appears at the top of the page.

## External Libraries

The following external libraries are used in this project:
- Engine Physics: [Jolt](https://github.com/jrouwe/JoltPhysics)
- Engine UI: [ImGui](https://github.com/ocornut/imgui)
- Engine Audio: [FMOD](https://www.fmod.com/download)

Other libraries: 
- [glad](https://github.com/Dav1dde/glad)
- [glfw](https://github.com/glfw/glfw)
- [glm](https://github.com/g-truc/glm)
- [spdlog](https://github.com/gabime/spdlog)
- [stb](https://github.com/nothings/stb)
- [tinyobjectloader](https://github.com/tinyobjloader/tinyobjloader)

## Totally Accurate Pool Simulator Instructions

### Objective of the Game
- The main goal is to legally pocket the 8-ball (black ball) after pocketing all of your designated group of balls (either stripes or solids).

### Taking Turns
- Players take turns shooting the cue ball.
- A player continues to shoot as long as they legally pocket a ball on each shot.

### Choosing Groups
- Once a player pockets a ball from the break, they are assigned that type of ball (stripes or solids).
- If no balls are pocketed on the break, the table is still “open,” and the next player can attempt to pocket any ball.

### Legal Shots
- A shot is considered legal if the player hits one of their own group of balls first and either pockets a ball, or drives any ball (including the cue ball) to a rail.

### Fouls
- Failing to hit your own balls first.
- Pocketing the cue ball (scratch).
- Not hitting any balls with the cue ball.

After a foul, the incoming player gets the cue ball in hand, meaning they can place it anywhere on the table for their shot.

### Pocketing the 8-Ball
- A player must pocket the 8-ball in a called pocket after clearing all of their group of balls.
- Pocketing the 8-ball before clearing your group or pocketing the 8-ball on the same shot as the last of your group results in a loss.

### Winning the Game
- The first player to legally pocket the 8-ball after clearing their group of balls wins the game.