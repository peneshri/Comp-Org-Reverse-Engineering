#include <iostream>
#include <fstream>
#include <string>
using namespace std;

struct Instructions
{
    string inst;
    int code, functionCode;
};

struct RegTable
{
    string reg;
    int address;
};

string hexToBin(string hexStr);

int findOpCode(string binStr);

int findRS(string binStr);

int findRT(string binStr);

int findRD(string binStr);

int findShAmt(string binStr);

int findFunct(string binStr);

int findImmediate(string binStr);

int lookupR(const Instructions instr[], int numInst, int opCode, int functCode);

int lookupI(const Instructions instr[], int numInst, int opCode);

int lookupRegister(const RegTable registers[], int numRegs, int regAdd);

int main()
{
    Instructions instr[5];
    // add: opcode 0, funct code 32
    instr[0].inst = "add";
    instr[0].code = 0;
    instr[0].functionCode = 32;
    // addi: opcode 8
    instr[1].inst = "addi";
    instr[1].code = 8;
    instr[1].functionCode = -1;
    // sub: opcode 0, funct code 34
    instr[2].inst = "sub";
    instr[2].code = 0;
    instr[2].functionCode = 34;
    // lw: opcode 35
    instr[3].inst = "lw";
    instr[3].code = 35;
    instr[3].functionCode = -1;
    // sw: opcode 43
    instr[4].inst = "sw";
    instr[4].code = 43;
    instr[4].functionCode = -1;

    RegTable registers[6];
    registers[0].reg = "$t0";
    registers[0].address = 8;
    registers[1].reg = "$t1";
    registers[1].address = 9;
    registers[2].reg = "$t2";
    registers[2].address = 10;
    registers[3].reg = "$0";
    registers[3].address = 0;
    registers[4].reg = "$s0";
    registers[4].address = 16;
    registers[5].reg = "$s1";
    registers[5].address = 17;

    ifstream file("test.txt");
    if (!file.is_open())
    {
        cout << "Error: could not open file." << endl;
        return -1;
    }
    string line;
    while(getline(file, line))
    {
        //cout << "Hex: " << line << endl;
        string binLine = hexToBin(line);
        //cout << "Binary: " << binLine << endl;
        int opCode = findOpCode(binLine);
        int RS = findRS(binLine);
        int RT = findRT(binLine);

        // R-Type
        if (opCode == 0)
        {
            //cout << "R-Type" << endl;
            int RD = findRD(binLine);
            int shift = findShAmt(binLine);
            int funct = findFunct(binLine);

            // instruction
            int instNum = lookupR(instr, 5, opCode, funct);
            if (instNum == -1)
            {
                cout << "Error: instruction not found" << endl;
                return -1;
            }
            cout << instr[instNum].inst << " ";

            // RD
            int RDNum = lookupRegister(registers, 6, RD);
            if (RDNum == -1)
            {
                cout << "Error: RD not found" << endl;
                return -1;
            }
            cout << registers[RDNum].reg << ", ";

            // RS
            int RSNum = lookupRegister(registers, 6, RS);
            if (RSNum == -1)
            {
                cout << "Error: RS not found" << endl;
                return -1;
            }
            cout << registers[RSNum].reg << ", ";

            // RT
            int RTNum = lookupRegister(registers, 6, RT);
            if (RTNum == -1)
            {
                cout << "Error: RT not found" << endl;
                return -1;
            }
            cout << registers[RTNum].reg;
        }
        // I-Type
        else
        {
            int immediate = findImmediate(binLine);
            
            // instruction
            int instNum = lookupI(instr, 5, opCode);
            if (instNum == -1)
            {
                cout << "Error: instruction not found" << endl;
                return -1;
            }
            cout << instr[instNum].inst << " ";

            // RT
            int RTNum = lookupRegister(registers, 6, RT);
            if (RTNum == -1)
            {
                cout << "Error: RT not found" << endl;
                return -1;
            }
            cout << registers[RTNum].reg << ", ";

            if (instr[instNum].inst == "addi")
            {
                // RS
                int RSNum = lookupRegister(registers, 6, RS);
                if (RSNum == -1)
                {
                    cout << "Error: RS not found" << endl;
                    return -1;
                }
                cout << registers[RSNum].reg << ", ";

                // immediate
                cout << immediate;
            }
            else
            {
                // immediate
                cout << immediate;

                // RS
                int RSNum = lookupRegister(registers, 6, RS);
                if (RSNum == -1)
                {
                    cout << "Error: RS not found" << endl;
                    return -1;
                }
                cout << "(" << registers[RSNum].reg << ")";
            }
        }
        cout << endl;
    }
    
    file.close();
    return 0;
}

string hexToBin(string hexStr)
{
    string binaryStr;
    for (int i = 2; i < 10; i++)
    {
        if (hexStr[i] == '0')
        {
            binaryStr += "0000";
        }
        else if (hexStr[i] == '1')
        {
            binaryStr += "0001";
        }
        else if (hexStr[i] == '2')
        {
            binaryStr += "0010";
        }
        else if (hexStr[i] == '3')
        {
            binaryStr += "0011";
        }
        else if (hexStr[i] == '4')
        {
            binaryStr += "0100";
        }
        else if (hexStr[i] == '5')
        {
            binaryStr += "0101";
        }
        else if (hexStr[i] == '6')
        {
            binaryStr += "0110";
        }
        else if (hexStr[i] == '7')
        {
            binaryStr += "0111";
        }
        else if (hexStr[i] == '8')
        {
            binaryStr += "1000";
        }
        else if (hexStr[i] == '9')
        {
            binaryStr += "1001";
        }
        else if (tolower(hexStr[i]) == 'a')
        {
            binaryStr += "1010";
        }
        else if (tolower(hexStr[i]) == 'b')
        {
            binaryStr += "1011";
        }
        else if (tolower(hexStr[i]) == 'c')
        {
            binaryStr += "1100";
        }
        else if (tolower(hexStr[i]) == 'd')
        {
            binaryStr += "1101";
        }
        else if (tolower(hexStr[i]) == 'e')
        {
            binaryStr += "1110";
        }
        else if (tolower(hexStr[i]) == 'f')
        {
            binaryStr += "1111";
        }
        else
        {
            cout << "Error: string contains non-hexidecimal character." << endl;
            return "";
        }
    }
    return binaryStr;
}

int findOpCode(string binStr)
{
    string codeStr = binStr.substr(0, 6);
    return stoi(codeStr, nullptr, 2);
}

int findRS(string binStr)
{
    string codeStr = binStr.substr(6, 5);
    return stoi(codeStr, nullptr, 2);
}

int findRT(string binStr)
{
    string codeStr = binStr.substr(11, 5);
    return stoi(codeStr, nullptr, 2);
}

int findRD(string binStr)
{
    string codeStr = binStr.substr(16, 5);
    return stoi(codeStr, nullptr, 2);
}

int findShAmt(string binStr)
{
    string codeStr = binStr.substr(21, 5);
    return stoi(codeStr, nullptr, 2);
}

int findFunct(string binStr)
{
    string codeStr = binStr.substr(26, 6);
    return stoi(codeStr, nullptr, 2);
}

int findImmediate(string binStr)
{
    string codeStr = binStr.substr(16, 16);
    return stoi(codeStr, nullptr, 2);
}

int lookupR(const Instructions instr[], int numInst, int opCode, int functCode)
{
    for (int i = 0; i < numInst; i++)
    {
        if (instr[i].code == opCode && instr[i].functionCode == functCode)
        {
            return i;
        }
    }
    return -1;
}

int lookupI(const Instructions instr[], int numInst, int opCode)
{
    for (int i = 0; i < numInst; i++)
    {
        if (instr[i].code == opCode)
        {
            return i;
        }
    }
    return -1;
}

int lookupRegister(const RegTable registers[], int numRegs, int regAdd)
{
    for (int i = 0; i < numRegs; i++)
    {
        if (registers[i].address == regAdd)
        {
            return i;
        }
    }
    return -1;
}