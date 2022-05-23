# BattleSim5e
Dice Rolling Statistics Generator, mainly for D&amp;D 5e martial turns.

Created by Samuel Nguyen (2022)

# How to Use
File name must be "input.txt" and be in the same folder as the program. It will output to "output.txt" in the same folder.
Please run it using "battlesim.bat" located in the same folder as the program, instead of the program itself; the progam should be named "battlesim.exe"
If .csv's are outputted, then a MatLab function script "simGraph.m" is provided to graph them.

===input.txt FORMAT===
1. Number of Simulations per Turn
I find that 10000 is 'good enough' for accuracy on the magnitude of about 0.5 and will make the program run almost instant.
100000 for program speed of about a few seconds but more accuracy.
2. Target AC
This is the armor class used to simulate the enemy; i.e. how many hits actually connect.
3. Character Name
Name used to refer to this simulated turn
4. Export or not
0 false, 1 true; if true, next line is filename
Will export first tuple as avg, stdev; rest of tuples are dmg, #sims pairs in ascending dmg order.
5. Attacks
If no attacks (why would you have no attacks?), then put "END" immediately after. Otherwise, put an empty line after 3.
5a. Advantage
0 for flat roll, 1 for disadvantage, 2 for advantage
5b. Bonus
The roll for attack bonus.
5c. Damage
The roll for base damage.
5d. Crit
The minimum crit value; most characters will have "20", Improved Critical gives "18" or "19".
5e. Crit Damage
The roll for crit damage, ADDED ON TOP of base damage on crit, NOT replacing.
If another attack, then add an empty line, and then repeat 5a-5e. Otherwise, put "END".
6. On Hit Effects
If no effects, then put "END" immediately after the previous "END". Otherwise, put an empty line.
6a. Limit
The amount of times this on hit damage bonus can be used per turn. If there is no limit, you should add it to the attack itself.
6b. Damage
The roll for base damage.
6c. Crit Damage
The roll for crit damage, ADDED ON TOP of base damage on crit.
If antoher effect, then add an empty line, then repeat 6a-6c. Otherwise, put "END".

===ROLL STRING FORMAT===
There can NEVER be any spaces.
(Almost) always starts with a number. It can be a number by itself to add a flat value.
If adding a negative, then predate the sub-roll with a "-".
If it is not just a number, it must be followed by "d", then the number of sides on the dice, i.e. "20", where the number before d will be the number of dice.

After that, the following options can go in any order, or don't have to exist.
"k" signifies keeping. After that must be "l" for keep lowest or "h" for keep highest. Then, a number for how many dice to keep.
"r" signifies rerolling. After that can be "o" to signify rerolling once, or not to reroll until condition is true. After that must be ">" for greater than comparison, or "<" for less than comparison. Then, a number for the dice to be compared to. This is per dice.
">" by itself signifies at least. After it must a be a number for the dice to be compared to. If less than this number, it will become that number, per dice.

Order of evaluation is "r",">","k","-"

The subrolls are separated by "+" with no spaces. If subtracting, then instead add negatives; i.e. "+-"

For example:
"6d9kl3>3ro<2+-1d20+-3"
Rolls 6 9-sided dice, rerolling each at least once if they are less than 2. If they are still less than 3, they become 3. Then, the 3 lowest are kept, and added together.
Then, a 20-sided dice is rolled, and subtracted from the result.
Then, 3 is subtracted from the result.

# Changelog
Version 1.4 (2022-05-13)
Add avg and stdev to exported data in the beginning as a tuple

Version 1.3 (2022-05-13)
Updated exported data to be sorted bar chart data, instead of raw simulation data

Version 1.2 (2022-05-13)
Added ability to generate and export data as CSV

Version 1.1 (2022-05-13)
Added catch for keep number being greater than the total number of dice.

Version 1.0 (2022-05-13)
Basic working program release.
Mostly focused on weapon/spell attack-focused characters; anything that causes saving throws not included.
Full spellcasters rarely do the same thing every turn, and are focused instead on versatility rather than constant damage output.
Numerical instead of analytical calculation, so ease of access at the cost of accuracy and time.