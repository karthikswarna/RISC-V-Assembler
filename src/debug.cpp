#include <iostream>
#include <cstdlib>
#include <string>
#include <cstring>
#include <vector>
#include <algorithm>
#include <fstream>
#include <bitset>
#include <unordered_map>

using namespace std;

typedef struct Opcode
{
    string code;
    string func3;
    string func7;
    string format;
}Opcode;

typedef struct Symbol
{
    string name;
    int address;
}Symbol;

int PC = -1;
int BaseAddress = 0;
int ErrorCount = 0;

static inline void ltrim(string &);
static inline void rtrim(string &);
static inline void trim(string &);
bool isNumeric(string);
void tokenize(const string &, const char*, vector<string>&);
string intTobin(int);
string shamtTobin(int);
string immTobin(int);
string getRegisterCode(string);

int main(int argc, char *argv[])
{
    unordered_map<string, Opcode> OPTAB;    // Opcode table.
    unordered_map<string, int> SYMTAB;      //Symbol table.


    /********************** Creating Opcode table. ************************/
    ifstream opcode_table;
    opcode_table.open("opcode_table");
    string line;

    if (opcode_table.is_open())
    {
        while(getline(opcode_table, line))
        {
            vector<string> property;
            tokenize(line, " ,", property);
            for(auto &i : property)
                trim(i);

            Opcode Node{};
            Node.code = property.at(1);
            Node.func3 = property.at(2);
            Node.func7 = property.at(3);
            Node.format = property.at(4);

            OPTAB.insert(make_pair(property.at(0), Node));
        }
    }
    else
    {
        cout << "Unable to open OPTAB file" << endl << "Assembling Terminated" << endl;
        return 0;
    }

    opcode_table.close();

    /********************** Removing Single line comments ************************/
    string file = argv[1];
    ifstream fin2(file);
    ofstream fout2;
    fout2.open("output_no_final.txt", ofstream::out | ofstream::trunc);

    if(fin2.is_open())
    {
    	while (getline(fin2, line))
        {
    		bool commentStarted = false;
    		string newLine = line;

    		for(string::iterator it = newLine.begin(); it != newLine.end(); it++)
            {
                if(!commentStarted)
                {
                    if(*it == '#')
                    {
                        *it = ' ';
                        commentStarted = true;
                    }
                }
                else if(commentStarted)
                    *it = ' ';
    		}

            bool isEmpty = true;
            for(string::iterator itt = newLine.begin(); itt != newLine.end(); itt++)
                if((int)*itt != 9 && (int)*itt != 13 && (int)*itt != 32)      // 9 is \t, 13 is \n and 32 is space.
                    isEmpty = false;

            if(!isEmpty)
            {
                // cout<<newLine<<endl;
                string instr="";
                for(int i=0;i<newLine.size();i++)
                {
                    if(newLine[i]==' ')
                    {
                        break;
                    }
                    instr += newLine[i];
                }
                Opcode node = OPTAB[instr];
                string frmt = node.format;
                if(frmt=="I")
                {
                    if(instr=="lw")
                    {   
                        string num="";
                        string rd="";
                        string rs="";
                        int j=0;
                        while(newLine[j]!=',')
                        {
                            j++;
                        }
                        j++;
                        // cout<<j<<endl;
                        while(j<newLine.size())
                        {
                            if(newLine[j]==' ')
                            {
                                j++;
                                continue;
                            }
                            if(newLine[j]-'0'>=0 && newLine[j]-'0'<=9)
                            {
                                num+=newLine[j];
                            }
                            if(newLine[j]=='(')
                            {
                                break;
                            }
                            j++;
                        }
                        j=2;
                        int flag=0;

                        while(newLine[j]!=',')
                        {
                            rd += newLine[j];
                            j++;
                        }
                        while(newLine[j]!='(')
                        {
                            j++;
                        }
                        j++;
                        while(newLine[j]!=')')
                        {
                            rs +=newLine[j];
                            j++;
                        }
                        //break;

                        
                        int imm = stoi(num);
                        string tmp = "";
                        if(imm>4095)
                        {
                            int lower = imm & 4095;
                            // cout<<lower<<endl;
                            int upper = imm & 8384512;
                            upper = upper >>12;
                            string l = to_string(lower);
                            string u = to_string(upper);
                            // cout<<upper<<endl;
                            tmp += "auipc " + rd + ", "+ u +"\n";
                            tmp += "lw "+ rd +", " + l + "(" +rs+")";
                            fout2<< tmp<<endl;
                        }
                        else{
                             fout2<<newLine<<endl;
                        }
                        
                        // cout << newLine << endl;
                        // cout<<tmp<<endl;
                      
                        //cout<<rd<<endl;
                    }
                    if(instr =="addi" || instr=="andi" || instr =="ori" || instr =="xori" || instr=="slti" || instr =="sltiu" )
                    {
                        int j=0;
                        string rd="", rs="";
                        while(newLine[j]!=' ')
                        {
                            j++;
                        }
                        j++;
                        while(newLine[j]!=',')
                        {
                            rd+= newLine[j];
                            j++;
                        }
                        j++;
                        while(newLine[j]!=',')
                        {
                            rs += newLine[j];
                            j++;
                        }
                        j++;

                        string num="";
                        while(j<newLine.size())
                        {
                            num += newLine[j];
                            j++;
                        }
                        trim(rd);
                        trim(rs);
                        trim(num);
                        // cout<<"rd= "<<rd<<endl;
                        // cout<<"rs= "<<rs<<endl;
                        // cout<<num<<endl;
                        if(num[0]!='_')
                        {
                            int imm = stoi(num);
                            string t="";
                            string tmp = "";
                            if(imm>4095)
                            {
                                int lower = imm & 4095;
                                int upper = imm & 8384512;
                                upper = upper >>12;
                                string l = to_string(lower);
                                string u = to_string(upper);
                                tmp += "auipc " + rd + ", "+ u ;
                                fout2<<tmp<<endl;
                                t += instr + " " + rd +", " + rs+ " "+ l;
                                fout2<<t<<endl;
                            }
                        }
                        else
                        {
                            fout2<<newLine<<endl;
                        }
                    
                    }
                }
                else{
                fout2 << newLine << endl;
                }
            }

    	}

    	fin2.close();
    }
    
    /********************** Resolving references ************************/
    file = "output_no_final.txt";
    ifstream fin(file);

    ofstream fout;
    if(argc == 2)
        fout.open("output_machine_code.o", ofstream::out | ofstream::trunc);
    else if(argc == 3)
        fout.open(argv[2], ofstream::out | ofstream::trunc);

    //cout << "Pass-1:" << endl;
    if (fin.is_open())
    {
        string op;
        while (getline(fin, line))
        {
            PC++;
            if (line.find(":") != string::npos)                  // If the line contains : then it is label
            {
                //cout << "Label Found!" << endl;
                trim(line);
                op = line.substr(0, line.length() - 1);
                SYMTAB.insert(make_pair(op, PC));
                PC--;
            }
        }

        fin.close();
    }
    else
    {
    	cout << "Unable to open input file. " << endl << "Assembling Terminated" << endl;
    	return 0;
	}

    cout << "\nSYMBOL TABLE:" << endl;
    for(auto i : SYMTAB)
        cout << hex << "0x" << i.second << " " << i.first << endl;


    /********************** Pass-2 of Assembler ************************/
    PC = -1;
    fin.open(file);

    //cout << "Pass-2:" << endl;
    if (fin.is_open())
    {
        string op, binary;
        while (getline(fin, line))
        {
            PC++;
            vector<string> instruction;
            tokenize(line, " ,", instruction);
            for(auto &i : instruction)
                trim(i);

            op = instruction.at(0);           
            if(op.back() == ':')                                // Its a label
            {
                PC--;
                fout << endl;
                continue;
            }
            else
            {
                string temp1, temp2, temp3;
                int temp4;

                if(OPTAB.find(op) == OPTAB.end())       // If the instruction is invalid.
                {
                    ErrorCount++;
                    fout.close();
                    if(argc == 2)
                        fout.open("output_machine_code.o", ofstream::out | ofstream::trunc);
                    else if(argc == 3)
                        fout.open(argv[2], ofstream::out | ofstream::trunc);
                    cout << "Wrong instruction used at line " << dec << PC + 1 << '.' << endl;
                    continue;                    
                }
                
                Opcode current_inst = OPTAB[op];
                fout << "0x" << hex << (PC + BaseAddress) * 4 << ": ";
                
                if(current_inst.format == "R")
                {
                    temp1 = instruction.at(1);
                    temp2 = instruction.at(2);
                    temp3 = instruction.at(3);

                    if(getRegisterCode(temp1) == "111111" || getRegisterCode(temp2) == "111111" || getRegisterCode(temp3) == "111111")
                    {
                        ErrorCount++;
                        fout.close();
                        if(argc == 2)
                            fout.open("output_machine_code.o", ofstream::out | ofstream::trunc);
                        else if(argc == 3)
                            fout.open(argv[2], ofstream::out | ofstream::trunc);
                        cout << "Syntax error at line " << dec << PC + 1 << ": Three valid register operands expected!" << endl;
                        continue;
                    }

                    fout << current_inst.func7 << " " << getRegisterCode(temp3)  << " " << getRegisterCode(temp2) << " " << current_inst.func3 << " " << getRegisterCode(temp1) << " " << current_inst.code << endl;
                }
                else if(current_inst.format == "I")
                {
                    if(instruction.at(0) == "lw" || instruction.at(0) == "lb")
                    {
                        temp1 = instruction.at(1);
                    
                        string::iterator it;
                        for(it = instruction.at(2).begin(); it != instruction.at(2).end(); it++)
                        {
                            if(*it == '(')
                                break;
                            temp3 += *it;
                        }

                        if(!isNumeric(temp3))
                        {
                            ErrorCount++;
                            fout.close();
                            if(argc == 2)
                                fout.open("output_machine_code.o", ofstream::out | ofstream::trunc);
                            else if(argc == 3)
                                fout.open(argv[2], ofstream::out | ofstream::trunc);
                            cout << "Syntax error at line " << dec << PC + 1 << ": Invalid immediate field." << endl;
                            continue;
                        }

                        temp4 = stoi(temp3);                                // temp4 contains offset value.
                        for(it++; it != instruction.at(2).end(); it++)      // temp2 contains register.
                        {
                            if(*it == ')')
                                break;
                            temp2 += *it;
                        }
                    }
                    else
                    {
                        temp1 = instruction.at(1);
                        temp2 = instruction.at(2);

                        if(!isNumeric(temp3))
                        {
                            ErrorCount++;
                            fout.close();
                            if(argc == 2)
                                fout.open("output_machine_code.o", ofstream::out | ofstream::trunc);
                            else if(argc == 3)
                                fout.open(argv[2], ofstream::out | ofstream::trunc);
                            cout << "Syntax error at line " << dec << PC + 1 << ": Invalid immediate field." << endl;
                            continue;
                        }
                        string t = instruction.at(3);

                        if(t[0] =='_')
                        {
                            temp4 = SYMTAB[t];
                        }
                        else
                        {
                            temp4 = stoi(instruction.at(3));
                        }   
                    }

                    // Immediate and shift operands bound check.
                    if(instruction.at(0) == "slli" || instruction.at(0) == "srli")
                    {
                        if(temp4 < 0 || temp4 > 32)
                        {
                            ErrorCount++;
                            fout.close();
                            if(argc == 2)
                                fout.open("output_machine_code.o", ofstream::out | ofstream::trunc);
                            else if(argc == 3)
                                fout.open(argv[2], ofstream::out | ofstream::trunc);
                            cout << "Syntax error at line " << dec << PC + 1 << ": Shift amount out of range!" << endl;
                            continue;
                        }
                    }
                    else if(temp4 < -32768 || temp4 > 32767)
                    {
                        ErrorCount++;
                        fout.close();
                        if(argc == 2)
                            fout.open("output_machine_code.o", ofstream::out | ofstream::trunc);
                        else if(argc == 3)
                            fout.open(argv[2], ofstream::out | ofstream::trunc);
                        cout << "Syntax error at line " << dec << PC + 1 << ": Immediate operand out of range!" << endl;
                        continue;
                    }

                    // Register validity check.
                    if(getRegisterCode(temp1) == "111111" || getRegisterCode(temp2) == "111111")
                    {
                        ErrorCount++;
                        fout.close();
                        if(argc == 2)
                            fout.open("output_machine_code.o", ofstream::out | ofstream::trunc);
                        else if(argc == 3)
                            fout.open(argv[2], ofstream::out | ofstream::trunc);
                        cout << "Syntax error at line " << dec << PC + 1 << ": Two valid register operands are expected." << endl;
                        continue;
                    }

                    // Converting integers to binary form.
                    if(instruction.at(0) == "slli" || instruction.at(0) == "srli")
                        temp3 = "0000000" + shamtTobin(temp4);
                    else
                        temp3 = immTobin(temp4);
                    // reverse(temp3.begin(), temp3.end());
                    fout << temp3 << " " << getRegisterCode(temp2) << " " << current_inst.func3 << " " << getRegisterCode(temp1) << " " << current_inst.code << endl;
                }
                else if(current_inst.format == "S")
                {
                    temp1 = instruction.at(1);
                    
                    string::iterator it;
                    for(it = instruction.at(2).begin(); it != instruction.at(2).end(); it++)
                    {
                        if(*it == '(')
                            break;
                        temp3 += *it;
                    }

                    if(!isNumeric(temp3))
                    {
                        ErrorCount++;
                        fout.close();
                        if(argc == 2)
                            fout.open("output_machine_code.o", ofstream::out | ofstream::trunc);
                        else if(argc == 3)
                            fout.open(argv[2], ofstream::out | ofstream::trunc);
                        cout << "Syntax error at line " << dec << PC + 1 << ": Invalid immediate field." << endl;
                        continue;
                    }

                    temp4 = stoi(temp3);                                // temp4 contains offset value.
                    for(it++; it != instruction.at(2).end(); it++)      // temp2 contains register.
                    {
                        if(*it == ')')
                            break;
                        temp2 += *it;
                    }

                    if(temp4 < -32768 || temp4 > 32767)
                    {
                        ErrorCount++;
                        fout.close();
                        if(argc == 2)
                            fout.open("output_machine_code.o", ofstream::out | ofstream::trunc);
                        else if(argc == 3)
                            fout.open(argv[2], ofstream::out | ofstream::trunc);
                        cout << "Syntax error at line " << dec << PC + 1 << ": Immediate operand out of range!" << endl;
                        continue;
                    }

                    if(getRegisterCode(temp1) == "111111" || getRegisterCode(temp2) == "111111")
                    {
                        ErrorCount++;
                        fout.close();
                        if(argc == 2)
                            fout.open("output_machine_code.o", ofstream::out | ofstream::trunc);
                        else if(argc == 3)
                            fout.open(argv[2], ofstream::out | ofstream::trunc);
                        cout << "Syntax error at line " << dec << PC + 1 << ": Two valid register and one immediate operands expected!" << endl;
                        continue;
                    }

                    temp3 = immTobin(temp4);
                    // reverse(temp3.begin(), temp3.end());
                    fout << temp3.substr(0, 7) << " " << getRegisterCode(temp1) << " " << getRegisterCode(temp2) << " " << current_inst.func3 << " " << temp3.substr(7, 5) << " " << current_inst.code << endl;
                }
                else if(current_inst.format == "SB")
                {
                    temp1 = instruction.at(1);
                    temp2 = instruction.at(2);
                    temp3 = instruction.at(3);  // Label

                    if(SYMTAB.find(temp3) == SYMTAB.end() && !isNumeric(temp3))
                    {
                        ErrorCount++;
                        fout.close();
                        if(argc == 2)
                            fout.open("output_machine_code.o", ofstream::out | ofstream::trunc);
                        else if(argc == 3)
                            fout.open(argv[2], ofstream::out | ofstream::trunc);
                        cout << "Syntax error at line " << dec << PC + 1 << ": Undefined label." << endl;
                        continue;
                    }

                    if(getRegisterCode(temp1) == "111111" || getRegisterCode(temp2) == "111111")
                    {
                        ErrorCount++;
                        fout.close();
                        if(argc == 2)
                            fout.open("output_machine_code.o", ofstream::out | ofstream::trunc);
                        else if(argc == 3)
                            fout.open(argv[2], ofstream::out | ofstream::trunc);
                        cout << "Syntax error at line " << dec << PC + 1 << ": Two valid registers are expected." << endl;
                        continue;
                    }

                    // temp3 is directly a number(offset).
                    if(isNumeric(temp3))
                    {
                        temp4 = stoi(temp3);        // Offset from PC.
                        if(abs(temp4) > 4096)
                        {
                            ErrorCount++;
                            fout.close();
                            if(argc == 2)
                                fout.open("output_machine_code.o", ofstream::out | ofstream::trunc);
                            else if(argc == 3)
                                fout.open(argv[2], ofstream::out | ofstream::trunc);
                            cout << "Error at line " << dec << PC + 1 << ": Branch can only be done in the range of 1K instructions." << endl;
                            continue;
                        }
                    }
                    // temp3 is a label.
                    else
                    {
                        temp4 = SYMTAB[temp3];      // Address of instructions.
                        if(abs(temp4 - PC*4) > 4096)
                        {
                            ErrorCount++;
                            fout.close();
                            if(argc == 2)
                                fout.open("output_machine_code.o", ofstream::out | ofstream::trunc);
                            else if(argc == 3)
                                fout.open(argv[2], ofstream::out | ofstream::trunc);
                            cout << "Error at line " << dec << PC + 1 << ": Branch can only be done in the range of 1K instructions." << endl;
                            continue;
                        }
                    }
                    temp3 = immTobin(temp4);
                    fout << temp3.at(0) << temp3.substr(2, 6) << " " << getRegisterCode(temp2) << " " << getRegisterCode(temp1) << " " << current_inst.func3 << " " << temp3.substr(8, 4) << temp3.at(1) << " " << current_inst.code << endl;
                }
                else if(current_inst.format == "U")
                {
                    temp1 = instruction.at(0);
                    temp2 = instruction.at(1);
                    temp3 = instruction.at(2);
                    // cout<<temp1<<endl;
                    // cout<<temp2<<endl;
                    // cout<<temp3<<endl;

                    string temp4 = getRegisterCode(temp2);
                    Opcode optab = OPTAB[temp1];
                    string u_op = optab.code;
                    string imm = bitset<20>(temp3).to_string();
                    fout<<imm<<" "<<temp4<<" "<<u_op<<endl;

                }
                else if(current_inst.format == "UJ")
                {
                    temp1 = instruction.at(1);
                    temp2 = instruction.at(2);  // Label

                    if(SYMTAB.find(temp2) == SYMTAB.end() && !isNumeric(temp3))
                    {
                        ErrorCount++;
                        fout.close();
                        if(argc == 2)
                            fout.open("output_machine_code.o", ofstream::out | ofstream::trunc);
                        else if(argc == 3)
                            fout.open(argv[2], ofstream::out | ofstream::trunc);
                        cout << "Syntax error at line " << dec << PC + 1 << ": Undefined label." <<  endl;
                        continue;
                    }

                    if(getRegisterCode(temp1) == "111111")
                    {
                        ErrorCount++;
                        fout.close();
                        if(argc == 2)
                            fout.open("output_machine_code.o", ofstream::out | ofstream::trunc);
                        else if(argc == 3)
                            fout.open(argv[2], ofstream::out | ofstream::trunc);
                        cout << "Syntax error at line " << dec << PC + 1 << ": Invalid register operand." << endl;
                        continue;
                    }

                    // temp3 is directly a number(offset).
                    if(isNumeric(temp2))
                        temp4 = stoi(temp2);        // Address of instructions.
                    // temp3 is a label.
                    else
                        temp4 = SYMTAB[temp2];      // Address of instructions.
                    temp3 = intTobin(temp4);
                    if(abs(temp4 - PC*4) > 1048576)
                    {
                        ErrorCount++;
                        fout.close();
                        if(argc == 2)
                            fout.open("output_machine_code.o", ofstream::out | ofstream::trunc);
                        else if(argc == 3)
                            fout.open(argv[2], ofstream::out | ofstream::trunc);
                        cout << "Error at line " << dec << PC + 1 << ": Jump can only be done in the range of 256K instructions." << endl;
                        continue;
                    }
                    // reverse(temp3.begin(), temp3.end());
                    fout << temp3.at(0) << temp3.substr(10, 10) << temp3.at(9) << temp3.substr(1, 8) << " " << getRegisterCode(temp1) << " " << current_inst.code << endl;
                }
            }
        }
    }
    else 
    {
    	cout << "Unable to open input file. " << endl << "Assembling Terminated" << endl;
    	return 0;
	}
    
    if(ErrorCount != 0)
    {
        fout.close();
        if(argc == 2)
            fout.open("output_machine_code.o", ofstream::out | ofstream::trunc);
        else if(argc == 3)
            fout.open(argv[2], ofstream::out | ofstream::trunc);
    }
    fin.close();
    fout.close();

    /*cout << "Symbol Table:" << endl;
    FILE *symbol_table = fopen("symbol_table", "w+");
    SYMTAB.print(symbol_table);
    fclose(symbol_table);*/
    return 0;
}

bool isNumeric(string str)
{
    for (int i = 0; i < str.length(); i++)
        if (isdigit(str[i]) == false)
            return false;
    return true;
}

static inline void ltrim(string &s)
{
    s.erase(s.begin(), find_if(s.begin(), s.end(), [](unsigned char ch)
    {
        return !isspace(ch);
    }));
}

static inline void rtrim(string &s)
{
    s.erase(find_if(s.rbegin(), s.rend(), [](unsigned char ch)
    {
        return !isspace(ch);
    }).base(), s.end());
}

static inline void trim(string &s)
{
    ltrim(s);
    rtrim(s);
}

void tokenize(const string &str, const char *delim, vector<string> &out)
{
    size_t curr, prev = 0;
    while ((curr = str.find_first_of(delim, prev)) != string::npos)
    {
        if (curr > prev)
            out.push_back(str.substr(prev, curr-prev));
        prev = curr + 1;
    }

    if (prev < str.length())
        out.push_back(str.substr(prev, string::npos));
}
   
string intTobin(int input)
{
    string str = bitset<20>(input).to_string();
    return str;
}

string shamtTobin(int input)
{
    string str = bitset<5>(input).to_string();
    return str;
}

string immTobin(int input)
{
    string str = bitset<12>(input).to_string();
    return str;
}

string getRegisterCode(string reg)
{
    string s;

    if (reg == "zero" || reg == "x0")
        s = "00000";
    else if (reg == "ra" || reg == "x1")
        s = "00001";
    else if (reg == "sp" || reg == "x2")
        s = "00010";
    else if (reg == "gp" || reg == "x3")
        s = "00011";
    else if (reg == "tp" || reg == "x4")
        s = "00100";
    else if (reg == "t0" || reg == "x5")
        s = "00101";
    else if (reg == "t1" || reg == "x6")
        s = "00110";
    else if (reg == "t2" || reg == "x7")
        s = "00111";
    else if (reg == "s0" || reg == "fp" || reg == "x8")
        s = "01000";
    else if (reg == "s1" || reg == "x9")
        s = "01001";
    else if (reg == "a0" || reg == "x10")
        s = "01010";
    else if (reg == "a1" || reg == "x11")
        s = "01011";
    else if (reg == "a2" || reg == "x12")
        s = "01100";
    else if (reg == "a3" || reg == "x13")
        s = "01101";
    else if (reg == "a4" || reg == "x14")
        s = "01110";
    else if (reg == "a5" || reg == "x15")
        s = "01111";
    else if (reg == "a6" || reg == "x16")
        s = "10000";
    else if (reg == "a7" || reg == "x17")
        s = "10001";
    else if (reg == "s2" || reg == "x18")
        s = "10010";
    else if (reg == "s3" || reg == "x19")
        s = "10011";
    else if (reg == "s4" || reg == "x20")
        s = "10100";
    else if (reg == "s5" || reg == "x21")
        s = "10101";
    else if (reg == "s6" || reg == "x22")
        s = "10110";
    else if (reg == "s7" || reg == "x23")
        s = "10111";
    else if (reg == "s8" || reg == "x24")
        s = "11000";
    else if (reg == "s9" || reg == "x25")
        s = "11001";
    else if (reg == "s10" || reg == "x26")
        s = "1010";
    else if (reg == "s11" || reg == "x27")
        s = "11011";
    else if (reg == "t3" || reg == "x28")
        s = "11100";
    else if (reg == "t4" || reg == "x29")
        s = "11101";
    else if (reg == "t5" || reg == "x30")
        s = "11110";
    else if (reg == "t6" || reg == "x31")
        s = "11111";
    else
        s = "111111";

    return s;
}