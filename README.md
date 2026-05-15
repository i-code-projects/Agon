# Agôn
Agôn is a terminal-based RPG game made in c++ with full support for mods through AgônScript (.as), which is a lightweight, simple Data-Markup-Language designed specifically for Agôn.
## Key Features
- **Terminal based**. Agôn is fully ran from any standard terminal. It is built from termux.
- **Data driven**. The entities to items to attacks, everything in Agôn is built to be data driven.
- **Complex mechanics**. Agôn as an RPG has deep mechanics such as specific heights for attacks and armor such as **lower** or **upper** and more.
- **Massive content potential**. Agôn is built in a way that allows massive amounts of content and in the future may have custom **actions** per mod.
- **Gameplay Scalability**. the arena game mode which is the main game mode for progression progressively scales based on player power and scale.
## Installation
- **Prerequisites**. A compiler that supports c++17 and up (eg g++ or clang).
- **Build Instructions** go to agonProject/src/ and type the following
```g++ modLoader.cpp agon.cpp -o ../agon```
- **Opening The Game** go to the agonProject/ folder and open agon **or** type 
```../agon```
from the terminal if you did the last step
## AgônScript (.as) Overview
AgônScript is a simple Data-Markup-Language designed specifically for Agôn. its simple syntax consists of
- **Headers** like =attacks=, =entities=, =weapons= etc.
- **Object Instances** like a - followed by the name of you object. eg 
```-Cool sword```
- **Key Value System** uses colons for its key-value system.
- **No Specific Order For Objects** do you want to define entities first? AgônScript supports any order of definitions.
- **No need to use EVERY key value pair for EVERY object.** all objects have their own default values.
- **Built In Debugger** running the game through the terminal from ```agonProject/``` using the command ```./agon -d``` activates the built in **mod debugger** to debug mods.
- **Other Simple syntax Rules** like how it **uses tabs for readability** and how **names and values can include spaces without the need of quotes.**
## AgônScript Example
```=data=
name:my awesome mod
=weapons=
-Cool Sword
price:100
dmg mult:1.2
-Awesome Sword
price:300
dmg mult: 2.5
=armor=
-Fabulous Tunic
price:200
protection:2
height:upper
=entities=
-Da King Of Da CASTLE
hp:100
skill:20
armor:Fabulous Tunic
weapon:Cool Sword
attacks:punch,kick
```
**to load in mods** use the in-game mod loader by entering mods then the name of the mod without the .as extension.
# Controls
- **The controls are all command based**
- **Every available control is shown in terminal at all moments**
# To do List
- [ ] Add magic
- [ ] Add enchantments
- [ ] Add status effect
- [ ] Add gameplay
- [ ] Add Shop
- [ ] Add Fishing
- [ ] Add modding support for Magic, Enchantments and Status Effects
# Small Note
agon currently is in development of the modding system and the gameplay **has not been developed yet**
