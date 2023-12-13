# Bobo

## a game engine.

## How to get started

#### Step 1: Copy the following commands into your terminal.

```bash
git clone https://github.com/MoBaydoun/bobo.git
cd bobo
build.bat
```

You'll need to press enter for the build.bat command.

This should clone the project and generate a Visual Studio 2022 project.

#### Step 2: Open the solution file.

The "bobo" repository folder should now contain a "build" folder. Open the "bobo.sln" file inside of this build folder to open the project in Visual Studio.

#### Step 3: Build and run the project.

Click on the "Local Windows Debugger" button.

![image of vs2022](https://cdn.discordapp.com/attachments/308437507148742657/1150649912866906232/image.png)

### Congratulations! You can now start working on the engine 🎉

If you run into any issues regarding spdlog, you might need to run the following command in your terminal:

```bash
git submodule update --init --recursive
```

## Coding Conventions

-   Brackets are always new-line.
-   Locally scoped variables are camelCased.
-   Function names are PascalCased.
-   Static variables must be prefixed with "s\_" and the remaining in PascalCase.
-   Pointer variables must be prefixed with "p\_" and the remaining in PascalCase.
-   Member variables must be prefixed with "m\_" and the remaining in PascalCase.

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

## Instructions

1. Objective of the Game

    - The main goal is to legally pocket the 8-ball (black ball) after pocketing all of your designated group of balls (either stripes or solids).

2. Taking Turns

    - Players take turns shooting the cue ball.
    - A player continues to shoot as long as they legally pocket a ball on each shot.

3. Choosing Groups

    - Once a player pockets a ball from the break, they are assigned that type of ball (stripes or solids).
    - If no balls are pocketed on the break, the table is still “open,” and the next player can attempt to pocket any ball.

4. Legal Shot

    - A shot is considered legal if the player hits one of their own group of balls first and either pockets a ball, or drives any ball (including the cue ball) to a rail.

5. Fouls

    - Fouls include:

        - Failing to hit your own balls first.
        - Pocketing the cue ball (scratch).
        - Not hitting any balls with the cue ball.

    - After a foul, the incoming player gets the cue ball in hand, meaning they can place it anywhere on the table for their shot.

6. Pocketing the 8-Ball

    - A player must pocket the 8-ball in a called pocket after clearing all of their group of balls.
    - Pocketing the 8-ball before clearing your group or pocketing the 8-ball on the same shot as the last of your group results in a loss.

7. Winning the Game

    - The first player to legally pocket the 8-ball after clearing their group of balls wins the game.
