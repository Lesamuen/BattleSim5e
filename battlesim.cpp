#include <iostream>
#include <random>
#include <fstream>
#include <vector>
#include <string>

using namespace std;

random_device rd;

mt19937_64 rng = mt19937_64(rd());

/*Roll (num) dice with (size) sides, and return the sum of the results.*/
int r (const int& num, const int& size)
{
	uniform_int_distribution<int> uid(1, size);
    int sum = 0;
    for (int i = 0; i < num; i++)
    {
        sum += uid(rng);
    }
    return sum;
}

/*Parse dice string.
Only addition.
-XdXkl/hXr/o</>X>X+-X*/
int roll (string dice)
{
    int result = 0;
    int n = dice.size();
    vector<string> rolls;

    // Split to rolls
    int j = 0;
    for (int i = 0; i < n; i++)
    {
        if (dice[i] == '+')
        {
            rolls.push_back(dice.substr(j, i - j));
            j = i + 1;
        }
    }
    rolls.push_back(dice.substr(j));

    n = rolls.size();
    for (int i = 0; i < n; i++)
    {
        string roll = rolls[i];
        int m = roll.size();
        j = 0;
        bool neg = (roll[0] == '-');
        if (neg)
        {
            j++;
        }
        
        // Init for first num
        string currentNum = "";
        while (j < m && roll[j] >= 48 && roll[j] <= 57)
        {
            currentNum += roll[j++];
        }
        // Test if just number
        if (j == m)
        {
            if (neg)
            {
                result -= stoi(currentNum);
            }
            else
            {
                result += stoi(currentNum);
            }
            continue;
        }

        // Must therefore be dice
        int numDice = stoi(currentNum);
        j++;
        currentNum = "";
        while (j < m && roll[j] >= 48 && roll[j] <= 57)
        {
            currentNum += roll[j++];
        }
        int numSides = stoi(currentNum);

        // Test for other conditions

        // 0 for none, 1 for keep low, 2 for keep high
        int keep = 0;
        int keepNum = 0;

        // 0 for none, 1 for reroll until, 2 for reroll once
        int reroll = 0;
        bool greater;
        int rerollNum = 0;

        bool min = 0;
        int atLeast = 0;

        while (j < m)
        {
            currentNum = "";
            // Test for 'keep'
            if (roll[j] == 'k')
            {
                j++;
                if (roll[j] == 'l')
                {
                    keep = 1;
                }
                else
                {
                    keep = 2;
                }
                j++;

                while (j < m && roll[j] >= 48 && roll[j] <= 57)
                {
                    currentNum += roll[j++];
                }

                keepNum = stoi(currentNum);
                
                // logical max for keep number
                if (keepNum > numDice)
                {
                    keepNum = numDice;
                }
            }
            // Test for 'reroll'
            else if (roll[j] == 'r')
            {
                j++;
                if (roll[j] == 'o')
                {
                    reroll = 2;
                }
                else
                {
                    reroll = 1;
                }
                j++;
                if (roll[j] == '<')
                {
                    greater = 0;
                }
                else
                {
                    greater = 1;
                }
                j++;

                while (j < m && roll[j] >= 48 && roll[j] <= 57)
                {
                    currentNum += roll[j++];
                }

                rerollNum = stoi(currentNum);
            }
            // Test for 'at least'
            else if (roll[j] == '>')
            {
                j++;
                min = 1;

                while (j < m && roll[j] >= 48 && roll[j] <= 57)
                {
                    currentNum += roll[j++];
                }

                atLeast = stoi(currentNum);
            }
        }

        // Actually evaluate the sub-roll now
        int subResult = 0;
        int diceResults[numDice];
        for (j = 0; j < numDice; j++)
        {
            diceResults[j] = r(1, numSides);

            // Evaluate rerolling
            if (reroll == 1)
            {
                while (greater ? (diceResults[j] > rerollNum) : (diceResults[j] < rerollNum))
                {
                    diceResults[j] = r(1, numSides);
                }
            }
            else if (reroll == 2 && (greater ? (diceResults[j] > rerollNum) : (diceResults[j] < rerollNum)))
            {
                diceResults[j] = r(1, numSides);
            }

            // Evaluate minimum
            if (min && diceResults[j] < atLeast)
            {
                diceResults[j] = atLeast;
            }
        }
        // Evaluate keeping
        if (keep)
        {
            // Do insertion-sort cause too lazy to implement faster algorithm
            int key;
            for (int k = 1; k < numDice; k++)
            {
                key = diceResults[k];
                j = k - 1;
                while (j >= 0 && diceResults[j] > key)
                {
                    diceResults[j + 1] = diceResults[j];
                    j--;
                }
                diceResults[j + 1] = key;
            }

            // Sum kept dice
            if (keep == 1)
            {
                for (j = 0; j < keepNum; j++)
                {
                    subResult += diceResults[j];
                }
            }
            else
            {
                for (j = numDice - keepNum; j < numDice; j++)
                {
                    subResult += diceResults[j];
                }
            }
        }
        else
        {
            for (j = 0; j < numDice; j++)
            {
                subResult += diceResults[j];
            }
        }
        // Evaluate subtraction
        if (neg)
        {
            result -= subResult;
        }
        else
        {
            result += subResult;
        }
    }

    return result;
}

struct martialTurn {
    /*//{whether melee or ranged}
    {none, disadv, or adv}
    {attack bonus roll}
    {dmg roll}
    {min crit num}
    {crit dmg roll}*/
    struct attack {
        //bool ranged;
        int adv;
        string bonus;
        string dmg;
        int crit;
        string critdmg;

        attack (int adv, string bonus, string dmg, int crit, string critdmg)
        {
            //this->ranged = ranged;
            this->adv = adv;
            this->bonus = bonus;
            this->dmg = dmg;
            this->crit = crit;
            this->critdmg = critdmg;
        }
    };
    vector<attack> attacks;

    /*{max num of times in a turn}
    //{doesn't matter, melee only, ranged only}
    {dmg roll}*/
    struct onHit {
        int limit;
        //int type;
        string dmg;
        string critdmg;

        onHit (int limit, string dmg, string critdmg)
        {
            this->limit = limit;
            //this->type = type;
            this->dmg = dmg;
            this->critdmg = critdmg;
        }
    };
    vector<onHit> onHitEffects;

    string name;

    bool exp;
    string expname;

    /*Simulate damage of a turn, given target AC*/
    int takeTurn (int AC)
    {
        int totalDamage = 0;
        int numAttacks = attacks.size();

        // Create trackers for on-hit effects
        int numEffects = onHitEffects.size();
        int effectTrackers[numEffects];
        for (int i = 0; i < numEffects; i++)
        {
            effectTrackers[i] = onHitEffects[i].limit;
        }

        // Roll attacks
        for (int i = 0; i < numAttacks; i++)
        {
            int d20;
            // Check for advantage
            if (attacks[i].adv == 2)
            {
                d20 = roll("2d20kh1");
            }
            else if (attacks[i].adv == 1)
            {
                d20 = roll("2d20kl1");
            }
            else
            {
                d20 = r(1, 20);
            }

            // Crit fail = automiss; crit success = autohit + dmg
            if (d20 == 1)
            {
                continue;
            }
            bool crit = (d20 >= attacks[i].crit);

            // Add attack bonus
            d20 += roll(attacks[i].bonus);

            // Do damage if hit
            if (crit || d20 >= AC)
            {
                totalDamage += roll(attacks[i].dmg) + (crit ? roll(attacks[i].critdmg) : 0);
                // Apply on hit effects
                for (int j = 0; j < numEffects; j++)
                {
                    if (effectTrackers[j] > 0)
                    {
                        totalDamage += roll(onHitEffects[j].dmg) + (crit ? roll(onHitEffects[j].critdmg) : 0);
                        effectTrackers[j]--;
                    }
                }
            }
        }

        return totalDamage;
    }

    martialTurn (string name)
    {
        this->name = name;
    }
};

int main ()
{
    // Get input of character turns to test
    fstream file;
    file.open("input.txt", ios::in);

    /*File Format:
    NUMBER OF SIMULATIONS PER TURN
    AC OF SUPPOSED OPPONENT
    CHARACTER1NAME
    EXPORT?
    EXPORTNAME

    Attack1Advantage
    Attack1Bonus
    Attack1Damage
    Attack1Crit
    Attack1CritDamage

    Attack2Advantage
    ...
    END

    OnHit1Limit
    OnHit1Damage
    OnHit1CritDamage

    OnHit2Limit
    ...
    END
    CHARACTER2NAME
    ...*/
    
    vector<martialTurn> turns;
    int numTurns = 0;
    string line;
    getline(file, line);
    int sims = stoi(line);
    getline(file, line);
    int AC = stoi(line);

    while (getline(file, line))
    {
        // Get name
        turns.emplace_back(line);
        numTurns++;

        getline(file, line);
        if (stoi(line))
        {
            getline(file, turns[numTurns - 1].expname);
            turns[numTurns - 1].exp = 1;
        }
        else
        {
            turns[numTurns - 1].exp = 0;
        }

        // Get attacks
        getline(file, line);
        while (line != "END")
        {
            getline(file, line);
            int adv = stoi(line);
            string bonus;
            string dmg;
            getline(file, bonus);
            getline(file, dmg);
            getline(file, line);
            int crit = stoi(line);
            string critdmg;
            getline(file, critdmg);

            turns[numTurns - 1].attacks.emplace_back(adv, bonus, dmg, crit, critdmg);

            getline(file, line);
        }

        // Get on hit damage
        getline(file, line);
        while (line != "END")
        {
            getline(file, line);
            int limit = stoi(line);
            string dmg;
            string critdmg;
            getline(file, dmg);
            getline(file, critdmg);

            turns[numTurns - 1].onHitEffects.emplace_back(limit, dmg, critdmg);

            getline(file, line);
        }
    }

    file.close();

    // Calculate stats, using variable simulations (def in input) each turn
    // avg, stdev
    vector<vector<float>> stats;
    for (int i = 0; i < numTurns; i++)
    {
        cout << "Generating stats for \"" << turns[i].name << "\"..." << endl;
        int simulations[sims];
        float avg = 0;
        float stdev = 0;
        for (int j = 0; j < sims; j++)
        {
            simulations[j] = turns[i].takeTurn(AC);
            avg += simulations[j];
        }
        avg /= sims;

        for (int j = 0; j < sims; j++)
        {
            stdev += pow(avg - simulations[j], 2);
        }
        stdev = sqrt(stdev/sims);

        stats.emplace_back();
        stats[i].push_back(avg);
        stats[i].push_back(stdev);

        // Export simulations, if wanted
        if (turns[i].exp)
        {
            vector<int> keys;
            int numKeys = 0;
            vector<int> vals;
            for (int j = 0; j < sims; j++)
            {
                // binary search
                int key = simulations[j];
                int a = 0;
                int b = numKeys;
                while (b > a)
                {
                    int newKey = (b + a)/2;
                    if (keys[newKey] == key)
                    {
                        vals[newKey]++;
                        break;
                    }
                    else if (key < keys[newKey])
                    {
                        b = newKey;
                    }
                    else
                    {
                        a = newKey + 1;
                    }
                }
                
                // key already exists
                if (b > a)
                {
                    continue;
                }

                // need to allocate for new key
                numKeys++;
                keys.insert(keys.begin() + a, key);
                vals.insert(vals.begin() + a, 1);
            }

            ofstream expFile(turns[i].expname);
            expFile << avg << ",";
            for (int j = 0; j < numKeys - 1; j++)
            {
                expFile << keys[j] << ",";
            }
            expFile << keys[numKeys - 1] << endl;
            
            expFile << stdev << ",";
            for (int j = 0; j < numKeys - 1; j++)
            {
                expFile << vals[j] << ",";
            }
            expFile << vals[numKeys - 1];

            expFile.close();
        }
    }

    // Finally, output those stats
    file.open("output.txt", ios::out);
    file << "WARNING\n"
    << "Take these statistics with a grain of salt!\n"
    << "This does not take into account things like on-the-spur decisions, death and debilitations, external buffs, etc.\n"
    << "This only assumes that the character attacks as is programmed against something that doesn't change in AC.\n"
    << "It also naively numerically instead of analytically  generates and calculates these stats,\n"
    << "so there isn't a 100% confidence but probably more like a 99% depending on the amount of simulations.\n\n"
    << "===PARAMETERS===\n"
    << "Target's AC: " << AC << endl
    << "Number of Simulations per Turn: " << sims << endl << endl
    << "===STATISTICS===\n";
    for (int i = 0; i < numTurns; i++)
    {
        file << "For \"" << turns[i].name << "\":" << endl;
        file << "Average Damage Output: " << stats[i][0] << endl;
        file << "Damage Standard Deviation: " << stats[i][1] << endl;
        file << "There is a 68% confidence the damage is between " << (stats[i][0] - stats[i][1]) << " and " << (stats[i][0] + stats[i][1]) << endl;
        file << "There is a 95% confidence the damage is between " << (stats[i][0] - 2*stats[i][1]) << " and " << (stats[i][0] + 2*stats[i][1]) << endl;
        file << "There is a 99.7% confidence the damage is between " << (stats[i][0] - 3*stats[i][1]) << " and " << (stats[i][0] + 3*stats[i][1]) << endl << endl;
    }

    file.close();

    cout << "Done!" << endl;
}