#include <iostream>
#include <fstream>
#include <stack>
#include <vector>

using namespace std;

struct transition
{
    int startState, destinationState;
    char character;
    char characterNeededInStack, characterPushedIntoStack;
};
/**
 * @brief read the automaton details from the file and create the needed arrays in memory
 *
 * @param beginState is an integer that represents the starting state of the APD
 * @param finalStates is an array of integers that contains the final states
 * @param T is an array of "struct transition" that contains all the transitions of the automaton
 */
void readAutomaton(int &beginState, vector<int> &finalStates, vector<transition> &T)
{
    int i;
    unsigned numberOfTransitions, numberOfFinalStates;
    char unusedCharacter;
    ifstream fin("..\\APD Settings.txt");

    fin >> beginState;

    fin >> numberOfFinalStates;
    finalStates.resize(numberOfFinalStates);
    for(i = 0; i < numberOfFinalStates; i++)
        fin >> finalStates[i];


    fin >> numberOfTransitions;
    T.resize(numberOfTransitions);
    for(i = 0; i < numberOfTransitions; i++)
    {
        fin >> T[i].startState;
        fin >> T[i].destinationState;
        fin >> T[i].character;
        fin >> unusedCharacter;
        fin >> T[i].characterNeededInStack;
        fin >> unusedCharacter;
        fin >> unusedCharacter;
        fin >> T[i].characterPushedIntoStack;
    }

    fin.close();
}
/**
 * @brief checks if the given state "x" is a final state
 *
 * @param finalStates is an array of integers that contains the final states
 * @param x is an integer that represents the state that will be verified
 * @return true if state "x" is a final state, else return false
 */
bool isFinalState(vector<int> &finalStates, int x)
{
    int i;

    for(i = 0; i < finalStates.size(); i++)
        if(finalStates[i] == x)
            return true;
    return false;
}
/**
 * @brief checks if the given state "state" has at least a transition that doesnt read input from the given word (lambda character)
 *
 * @param T is an array of "struct transition" that contains all the transitions of the automaton
 * @param state is an integer that represents the state that will be verified
 * @return true if "state" has at least one transition with character lambda, else return false
 */
bool stateHasTransitionWithLambda(vector<transition> &T, int &state)
{
    int i;

    for(i = 0; i < T.size(); i++)
        if((state == T[i].startState) && (T[i].character == '*'))
            return true;
    return false;
}

/**
 * @brief reads the input of the given word and adds all the resulted states into the array "resultedStates"
 *
 * @param currentState is an integer that represents the state that has been reached until now on this branch of recursive calls
 * @param word is an array of characters that contains the words that has to be verified
 * @param resultedStates is an array of integers that contains all the resulted states that were found until now
 * @param S is a stack of characters that is used to implement the APD
 * @param T is an array of "struct transition" that contains all the transitions of the automaton
 */
void APD(int currentState, char word[101], vector<int> &resultedStates, stack<char> S, vector<transition> &T)
{
    int i;
    stack<char> copyS;

    copyS = S;

    if (word[0] == '\0')
    {
        if(!stateHasTransitionWithLambda(T, currentState))
            resultedStates.push_back(currentState);
        else
            for(i = 0; i < T.size(); i++)
            {
            S = copyS;
            if((T[i].startState == currentState) && (T[i].character == '*'))
                if( (!S.empty() && (T[i].characterNeededInStack == S.top())) || (T[i].characterNeededInStack == '*') )
                {
                    if(T[i].characterNeededInStack != '*')
                        S.pop();

                    if(T[i].characterPushedIntoStack != '*')
                        S.push(T[i].characterPushedIntoStack);

                    APD(T[i].destinationState, word, resultedStates, S, T);
                }
        }
    }
    else
        for(i = 0; i < T.size(); i++)
        {
            S = copyS;
            if((T[i].startState == currentState) && ((word[0] == T[i].character) || (T[i].character == '*')))
                if( (!S.empty() && (T[i].characterNeededInStack == S.top())) || (T[i].characterNeededInStack == '*') )
                {
                    if(T[i].characterNeededInStack != '*')
                        S.pop();

                    if(T[i].characterPushedIntoStack != '*')
                        S.push(T[i].characterPushedIntoStack);

                    if(T[i].character != '*')
                        APD(T[i].destinationState, word + 1, resultedStates, S, T);
                    else
                        APD(T[i].destinationState, word, resultedStates, S, T);
                }
        }
}

void printResult(vector<int> &resultedStates, vector<int> &finalStates)
{
    int i;
    bool foundFinalState = false;

    for(i = 0; i < resultedStates.size(); i++)
        if(isFinalState(finalStates, resultedStates[i]))
        {
            foundFinalState = true;
            break;
        }

    if(foundFinalState)
        cout << "Word accepted";
    else
        cout << "Word unaccepted";
}

int main()
{
    int beginState = -1;
    char word[101];
    vector<transition> T;
    vector<int> finalStates;
    vector<int> resultedStates;
    stack<char> S;


    readAutomaton(beginState, finalStates, T);

    cout << "Word = ";
    cin.get(word, 100);

    APD(beginState, word, resultedStates, S, T);

    printResult(resultedStates, finalStates);

    cout << '\n';
    return 0;
}