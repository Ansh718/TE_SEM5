#include <bits/stdc++.h>
using namespace std;
map<string, map<string, int>> dir;
map<string, int> rel, reg;

class Code;
string lowercase(string s)
{
    for (int i = 0; i < s.length(); i++)
    {
        s[i] = tolower(s[i]);
    }
    return s;
}

string uppercase(string s)
{
    for (int i = 0; i < s.length(); i++)
    {
        s[i] = toupper(s[i]);
    }
    return s;
}

class Syntax
{
private:
    string label, opcode, op1, op2;
    friend class Code;

public:
    Syntax();
};

Syntax::Syntax()
{
    label = opcode = op1 = op2 = "_";
}

class Pass1
{
private:
    int lc;
    string lcount, icOpcode, icOp1, icOp2;
    friend class Code;

public:
    Pass1();
};

Pass1::Pass1()
{
    lc = 0;
    icOpcode = icOp1 = icOp2 = lcount = "";
}

class Symbol
{
private:
    int addr;
    string sym;
    friend class Code;

public:
    Symbol();
};

Symbol::Symbol()
{
    sym = "";
    addr = 0;
}

class Literal
{
private:
    int addr;
    string lit;
    friend class Code;

public:
    Literal();
};

Literal::Literal()
{
    lit = "";
    addr = 0;
}

class Code
{
private:
    vector<Syntax> code;
    vector<Pass1> ans;
    vector<Symbol> symtab;
    vector<Literal> littab;
    vector<int> ltorg;
    vector<int> check;
    int lcount = 0;
    vector<pair<pair<string, string>, pair<string, int>>> v;

public:
    Code();
    void input();
    void display();
    void createIc();
    int findSymbol(string);
    int findLiteral(string);
    void displayIc();
    void displaySymtab();
    void displayLittab();
    void forwardReference();
};

Code::Code()
{
    code.clear();
    ans.clear();
    symtab.clear();
    littab.clear();
    ltorg.clear();
    check.clear();
    v.clear();
    ltorg.push_back(0);
}

void Code::input()
{
    fstream f;
    f.open("input.txt", ios::in);
    string s;
    while (getline(f, s))
    {
        string str = "";
        Syntax t;
        bool flag = false;
        for (int i = 0; i < s.length(); i++)
        {
            if (s[i] == ' ')
            {
                str = lowercase(str);
                if (dir[str].size() == 0 && flag == false)
                {
                    t.label = str;
                }
                else if (dir[str].size() != 0)
                {
                    t.opcode = str;
                    flag = true;
                }
                else if (reg[str] != 0 || rel[str] != 0)
                {
                    t.op1 = str;
                }
                str = "";
            }
            else
            {
                str += s[i];
            }
        }
        str = lowercase(str);
        if (dir[str].size() != 0)
        {
            t.opcode = str;
        }
        else
        {
            t.op2 = str;
        }
        code.push_back(t);
    }
    f.close();
}

void Code::createIc()
{
    int lct = 0;
    for (int i = 0; i < code.size(); i++)
    {
        Pass1 p;
        auto itr = dir[code[i].opcode].begin();
        string s = itr->first;
        uppercase(s);
        p.icOpcode = "(" + s + "," + to_string(itr->second) + ")";
        if (code[i].op1 != "_")
        {
            auto itr = reg.find(code[i].op1);
            if (itr == reg.end())
            {
                p.icOp1 = "(" + to_string(rel[code[i].op1]) + ")";
            }
            else
            {
                p.icOp1 = "(" + to_string(reg[code[i].op1]) + ")";
            }
        }
        if (code[i].opcode == "start")
        {
            lct = stoi(code[i].op2);
            p.lc = 0;
            p.icOp2 = "(C," + code[i].op2 + ")";
        }
        else if (itr->first == "AD")
        {
            string lhs, rhs, sign = "~";
            int index2, val;
            bool pushed = false;
            if (code[i].op2 != "_")
            {
                string ptr = code[i].op2;
                auto itr1 = find(ptr.begin(), ptr.end(), '+');
                auto itr2 = find(ptr.begin(), ptr.end(), '-');
                if (itr1 != ptr.end() || itr2 != ptr.end())
                {
                    string sep = "";
                    for (int i = 0; i < ptr.length(); i++)
                    {
                        if (ptr[i] == '+' || ptr[i] == '-')
                        {
                            sign = ptr[i];
                            lhs = sep;
                            sep = "";
                        }
                        else
                        {
                            sep += ptr[i];
                        }
                    }
                    rhs = sep;
                }
                if (sign != "~")
                {
                    index2 = findSymbol(lhs);
                    val = stoi(rhs);
                    p.icOp2 = "(S," + to_string(index2 + 1) + ")" + sign + rhs;
                }
                else
                {
                    index2 = findSymbol(code[i].op2);
                    p.icOp2 = "(S," + to_string(index2 + 1) + ")";
                }
            }
            if (code[i].opcode == "equ")
            {
                int index1 = findSymbol(code[i].label);
                if (index2 == -1)
                {
                    check.push_back(i);
                    pair<pair<string, string>, pair<string, int>> p;
                    p.first.first = code[i].label;
                    p.first.second = sign;
                    if (sign == "~")
                    {
                        p.second.first = code[i].op2;
                        p.second.second = 0;
                    }
                    else
                    {
                        p.second.first = lhs;
                        p.second.second = val;
                    }
                    v.push_back(p);
                }
                if (sign == "+")
                {
                    symtab[index1].addr = symtab[index2].addr + val;
                }
                else if (sign == "-")
                {
                    symtab[index1].addr = symtab[index2].addr - val;
                }
                else
                {
                    symtab[index1].addr = symtab[index2].addr;
                }
            }
            else if (code[i].opcode == "origin")
            {
                if (sign == "+")
                {
                    lct = symtab[index2].addr + val;
                }
                else if (sign == "-")
                {
                    lct = symtab[index2].addr - val;
                }
                else
                {
                    lct = symtab[index2].addr;
                }
            }
            else if (code[i].opcode == "ltorg" || code[i].opcode == "end")
            {
                if (code[i].opcode == "end")
                {
                    p.lc = 0;
                    ans.push_back(p);
                    pushed = true;
                }
                if (littab.size() > 0)
                {
                    ltorg.push_back(littab.size() - 1);
                    for (int i = ltorg.size() - 2; i >= 0 && i < ltorg.size(); i++)
                    {
                        if (littab[ltorg[i]].addr == 0)
                        {
                            p.lc = lct;
                            p.lcount = to_string(lct);
                            string ltt = littab[ltorg[i]].lit;
                            ltt = ltt.substr(2, 1);
                            p.icOp1 = "(C," + ltt + ")";
                            p.icOpcode = "(DL,1)";
                            littab[ltorg[i]].addr = lct;
                            ans.push_back(p);
                            pushed = true;
                            lct += 1;
                        }
                    }
                }
            }
            if (pushed == true)
            {
                continue;
            }
            p.lc = 0;
        }
        else if (code[i].opcode == "ds")
        {
            p.lc = lct;
            p.lcount = to_string(lct);
            p.icOp2 = "(C," + code[i].op2 + ")";
            Symbol st;
            int index = findSymbol(code[i].label);
            if (index == -1)
            {
                st.sym = code[i].label;
                st.addr = lct;
                symtab.push_back(st);
            }
            else
            {
                symtab[index].sym = code[i].label;
                symtab[index].addr = lct;
            }
            lct += stoi(code[i].op2);
        }
        else
        {
            p.lc = lct;
            p.lcount = to_string(lct);
            if (code[i].opcode == "dc")
            {
                string temp = code[i].op2;
                temp = temp.substr(1, temp.length() - 2);
                p.icOp2 = "(C," + temp + ")";
                int index = findSymbol(code[i].label);
                symtab[index].sym = code[i].label;
                symtab[index].addr = lct;
            }
            else if (code[i].label != "_")
            {
                int index = findSymbol(code[i].label);
                if (index == -1)
                {
                    Symbol ptr;
                    ptr.sym = code[i].label;
                    ptr.addr = lct;
                    symtab.push_back(ptr);
                }
                else
                {
                    symtab[index].addr = lct;
                }
            }
            if (code[i].op2 != "_" && code[i].opcode != "dc" && code[i].op2[0] != '=')
            {
                int index = findSymbol(code[i].op2);
                if (index == -1)
                {
                    Symbol ptr;
                    ptr.sym = code[i].op2;
                    ptr.addr = 0;
                    symtab.push_back(ptr);
                    index = findSymbol(code[i].op2);
                    p.icOp2 = "(S," + to_string(index + 1) + ")";
                }
                else
                {
                    p.icOp2 = "(S," + to_string(index + 1) + ")";
                }
            }
            if (code[i].op2 != "_" && code[i].opcode != "dc" && code[i].op2[0] == '=')
            {
                Literal ltr;
                ltr.lit = code[i].op2;
                ltr.addr = 0;
                littab.push_back(ltr);
                lcount += 1;
                p.icOp2 = "(L," + to_string(lcount) + ")";
            }
            lct += 1;
        }
        ans.push_back(p);
    }
}

int Code::findSymbol(string temp)
{
    for (int i = 0; i < symtab.size(); i++)
    {
        if (symtab[i].sym == temp)
        {
            return i;
        }
    }
    return -1;
}

int Code::findLiteral(string temp)
{
    for (int i = 0; i < littab.size(); i++)
    {
        if (littab[i].lit == temp)
        {
            return i;
        }
    }
    return -1;
}

void Code::displayIc()
{
    fstream ic;
    ic.open("ic.txt", ios::out);

    for (int i = 0; i < ans.size(); i++)
    {
        ic << ans[i].lcount << " ";   // LC
        ic << ans[i].icOpcode << " "; // Opcode
        ic << ans[i].icOp1 << " ";    // Operand1
        ic << ans[i].icOp2 << endl;   // Operand2
    }

    ic.close();
}

void Code::displaySymtab()
{
    fstream symtabfile;
    symtabfile.open("symbol.txt", ios::out);

    for (int i = 0; i < symtab.size(); i++)
    {
        symtabfile << i + 1 << " ";                    // Sr. No.
        symtabfile << uppercase(symtab[i].sym) << " "; // Symbol
        symtabfile << symtab[i].addr << endl;          // Address
    }

    symtabfile.close();
}

void Code::displayLittab()
{
    fstream littabfile;
    littabfile.open("literal.txt", ios::out);

    for (int i = 0; i < littab.size(); i++)
    {
        littabfile << i + 1 << " ";           // Sr. No.
        littabfile << littab[i].lit << " ";   // Literal
        littabfile << littab[i].addr << endl; // Address
    }

    littabfile.close();
}

void Code::forwardReference()
{
    for (int i = 0; i < v.size(); i++)
    {
        int index1, index2;
        index1 = findSymbol(v[i].first.first);
        index2 = findSymbol(v[i].second.first);
        if (v[i].first.second == "~")
        {
            symtab[index1].addr = symtab[index2].addr;
            ans[check[i]].icOp2 = "(S," + to_string(index2 + 1) + ')';
        }
        else if (v[i].first.second == "+")
        {
            symtab[index1].addr = symtab[index2].addr + v[i].second.second;
            ans[check[i]].icOp2 = "(S," + to_string(index2 + 1) + ')' + v[i].first.second + to_string(v[i].second.second);
        }
        else
        {
            symtab[index1].addr = symtab[index2].addr - v[i].second.second;
            ans[check[i]].icOp2 = "(S," + to_string(index2 + 1) + ')' + v[i].first.second + to_string(v[i].second.second);
        }
    }
}

void createReg(map<string, int> &m)
{
    m["areg"] = 1;
    m["breg"] = 2;
    m["creg"] = 3;
    m["dreg"] = 4;
}

void createRel(map<string, int> &m)
{
    m["lt"] = 1;
    m["le"] = 2;
    m["eq"] = 3;
    m["gt"] = 4;
    m["ge"] = 5;
    m["any"] = 6;
}

void createDir(map<string, map<string, int>> &m)
{
    m["stop"].insert(make_pair("IS", 0));
    m["add"].insert(make_pair("IS", 1));
    m["sub"].insert(make_pair("IS", 2));
    m["mult"].insert(make_pair("IS", 3));
    m["mover"].insert(make_pair("IS", 4));
    m["movem"].insert(make_pair("IS", 5));
    m["comp"].insert(make_pair("IS", 6));
    m["bc"].insert(make_pair("IS", 7));
    m["div"].insert(make_pair("IS", 8));
    m["read"].insert(make_pair("IS", 9));
    m["print"].insert(make_pair("IS", 10));
    m["start"].insert(make_pair("AD", 1));
    m["end"].insert(make_pair("AD", 2));
    m["origin"].insert(make_pair("AD", 3));
    m["equ"].insert(make_pair("AD", 4));
    m["ltorg"].insert(make_pair("AD", 5));
    m["dc"].insert(make_pair("DL", 1));
    m["ds"].insert(make_pair("DL", 2));
}

int main()
{
    createDir(dir);
    createRel(rel);
    createReg(reg);
    Code obj;
    obj.input();
    obj.createIc();
    obj.forwardReference();
    obj.displayIc();
    obj.displaySymtab();
    obj.displayLittab();
    return 0;
}
