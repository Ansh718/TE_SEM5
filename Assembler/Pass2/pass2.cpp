#include <bits/stdc++.h>
using namespace std;

class Pass1
{
    string icOpcode, icOp1, icOp2, lc;
    friend class Code;

public:
    Pass1()
    {
        lc = icOpcode = icOp1 = icOp2 = "";
    }
};

class Symbol
{
    int addr;
    string sym;
    friend class Code;

public:
    Symbol()
    {
        sym = "";
        addr = 0;
    }
};

class Literal
{
    int addr;
    string lit;
    friend class Code;

public:
    Literal()
    {
        lit = "";
        addr = 0;
    }
};

class Pass2
{
    int lc;
    string op1, op2, opcode, bit, lcount;
    friend class Code;

public:
    Pass2()
    {
        lc = 0;
        bit = "+";
        op1 = "0";
        op2 = "000";
        opcode = "00";
        lcount = "";
    }
};

class Code
{
    vector<Pass1> code;
    vector<Symbol> symtab;
    vector<Literal> littab;
    vector<Pass2> ans;

public:
    Code()
    {
        code.clear();
        symtab.clear();
        littab.clear();
        ans.clear();
    }
    void extractLiteral()
    {
        fstream f;
        f.open("literal.txt", ios::in);
        string s;
        Literal syms;
        while (getline(f, s))
        {
            string str = "";
            bool flag = false;
            for (int i = 0; i < s.length(); i++)
            {
                if (s[i] == ' ')
                {
                    if (flag == false)
                    {
                        flag = true;
                    }
                    else
                    {
                        syms.lit = str;
                    }
                    str = "";
                }
                while (s[i] == ' ')
                {
                    i += 1;
                }
                str += s[i];
            }
            syms.addr = stoi(str);
            littab.push_back(syms);
        }
        f.close();
    }
    void extractsymbol()
    {
        fstream f;
        f.open("symbol.txt", ios::in);
        string s;
        Symbol syms;
        while (getline(f, s))
        {
            string str = "";
            bool flag = false;
            for (int i = 0; i < s.length(); i++)
            {
                if (s[i] == ' ')
                {
                    if (flag == false)
                    {
                        flag = true;
                    }
                    else
                    {
                        syms.sym = str;
                    }
                    str = "";
                }
                while (s[i] == ' ')
                {
                    i += 1;
                }
                str += s[i];
            }
            syms.addr = stoi(str);
            symtab.push_back(syms);
        }
        f.close();
    }
    void input()
    {
        fstream f;
        f.open("input.txt", ios::in);
        string s;
        while (getline(f, s))
        {
            string str = "";
            Pass1 p;
            bool flag = false;
            for (int i = 0; i < s.length(); i++)
            {
                if (s[i] == ' ')
                {
                    if (flag == false)
                    {
                        if (str[0] != '(')
                        {
                            p.lc = str;
                        }
                        else
                        {
                            p.icOpcode = str;
                            flag = true;
                        }
                    }
                    else if (str.length() == 3)
                    {
                        p.icOp1 = str;
                    }
                    str = "";
                }
                else
                {
                    str += s[i];
                }
            }
            if (flag)
            {
                p.icOp2 = str;
            }
            else
            {
                p.icOpcode = str;
            }
            code.push_back(p);
        }
        f.close();
    }
    void createMachineCode()
    {
        int lct = 0;
        for (int i = 0; i < code.size(); i++)
        {
            Pass2 p;
            p.lcount = code[i].lc;

            string opco = code[i].icOpcode;
            string dire = opco.substr(1, 2);
            string n = opco.substr(4, 2);
            p.opcode = n;
            p.lc = lct;
            if (dire == "AD" && n == "01")
            {
                p.lc = 0;
                p.op1 = p.op2 = p.opcode = "";
                lct = stoi(code[i].icOp2.substr(3, code[i].icOp2.length() - 4));
            }
            else if (dire == "AD")
            {
                p.lc = 0;
                p.op1 = p.op2 = p.opcode = "";
                if (n == "02")
                {
                    ans.push_back(p);
                    continue;
                }
                string oper = code[i].icOp2;
                auto it1 = find(oper.begin(), oper.end(), '+');
                auto it2 = find(oper.begin(), oper.end(), '-');
                string sign = "~", lhs, rhs, ptr = "";
                int idx;
                if (it1 == oper.end() && it2 == oper.end())
                {
                    idx = stoi(oper.substr(3, oper.length() - 4)) - 1;
                    p.op2 = to_string(symtab[idx].addr);
                }
                else
                {
                    for (int i = 0; i < oper.length(); i++)
                    {
                        if (oper[i] == '+' || oper[i] == '-')
                        {
                            sign = oper[i];
                            lhs = ptr;
                            ptr = "";
                        }
                        else
                        {
                            ptr += oper[i];
                        }
                    }
                    rhs = ptr;
                    idx = stoi(lhs.substr(3, lhs.length() - 4)) - 1;
                }
                if (n == "03")
                {
                    if (sign == "~")
                    {
                        lct = symtab[idx].addr;
                    }
                    else if (sign == "+")
                    {
                        lct = symtab[idx].addr + stoi(rhs);
                    }
                    else if (sign == "-")
                    {
                        lct = symtab[idx].addr - stoi(rhs);
                    }
                }
                else if (n == "05")
                {
                }
            }
            else if (dire == "DL" && n == "02")
            {
                p.op1 = "";
                p.op2 = "";
                p.opcode = "";
                int t = stoi(code[i].icOp2.substr(3, code[i].icOp2.length() - 4));
                while (t > 0)
                {
                    p.lc = lct;
                    p.lcount = to_string(lct);
                    ans.push_back(p);
                    lct += 1;
                    t -= 1;
                }
                continue;
                // lct+=stoi(code[i].icOp2.substr(3,code[i].icOp2.length()-4));
            }
            else if (dire == "DL" && n == "01")
            {
                p.op1 = "0";
                p.opcode = "00";
                p.op2 = code[i].icOp2.substr(3, code[i].icOp2.length() - 4);
                while (p.op2.length() < 3)
                {
                    p.op2 = '0' + p.op2;
                }
                lct += 1;
            }
            if (code[i].icOp1.length() != 0)
            {
                p.op1 = code[i].icOp1.substr(1, 1);
            }
            if (dire != "DL" && dire != "AD" && n != "00")
            {
                int idx = stoi(code[i].icOp2.substr(3, code[i].icOp2.length() - 4)) - 1;
                if (code[i].icOp2[1] == 'L')
                {
                    p.op2 = to_string(littab[idx].addr);
                }
                else
                {
                    p.op2 = to_string(symtab[idx].addr);
                }
                while (p.op2.length() < 3)
                {
                    p.op2 = '0' + p.op2;
                }
                lct += 1;
            }
            if (n == "00")
            {
                lct += 1;
            }
            ans.push_back(p);
            // displayMachineCode();
        }
    }
    void display()
    {
        for (int i = 0; i < code.size(); i++)
        {
            cout << code[i].lc << " " << code[i].icOpcode << " " << code[i].icOp1 << "  " << code[i].icOp2 << endl;
        }
    }
    void displayMachineCode()
    {
        fstream f;
        f.open("machineCode.txt", ios::out);
        for (int i = 0; i < ans.size(); i++)
        {
            // string l=(ans[i].lc==0)?ans[i].lcount:to_string(ans[i].lc);
            string l = ans[i].lcount;
            if (l.length() == 0)
            {
                cout << "-------x------" << endl;
                f << "-------x------" << endl;
                continue;
            }
            cout << setw(3) << l;
            cout << " " << ans[i].bit << " " << ans[i].opcode << " " << ans[i].op1 << " " << ans[i].op2 << endl;
            f << setw(3) << l;
            f << " " << ans[i].bit << " " << ans[i].opcode << " " << ans[i].op1 << " " << ans[i].op2 << endl;
        }
        f.close();
    }
    void displaysymtab()
    {
        for (int i = 0; i < symtab.size(); i++)
        {
            cout << i + 1 << " " << symtab[i].sym << " " << symtab[i].addr << endl;
        }
    }
};

int main()
{
    Code obj;
    obj.input();
    obj.extractsymbol();
    obj.extractLiteral();
    // obj.display();
    obj.createMachineCode();
    obj.displayMachineCode();
    return 0;
}