#include <bits/stdc++.h>
using namespace std;
class MNT
{
private:
    int pp;
    int kp;
    int mdtp;
    int kpdtp;
    friend class Macro;

public:
    MNT();
};

MNT::MNT()
{
    pp = kp = mdtp = kpdtp = 0;
}

class Macro
{
private:
    vector<pair<string, MNT>> MNTAB;
    vector<pair<string, string>> KPDTAB;
    vector<vector<string>> MDTAB;
    vector<string> PNTAB;
    fstream file;
    fstream macro_name_table;
    fstream macro_definition_table;
    fstream keyword_parameter_definition_table;
    fstream parameter_name_table;

public:
    Macro();
    int findParameter(string, string);
    void pass1();
    void displayMNTAB();
    void displayKPDTAB();
    void displayMDTAB();
    void displayPNTAB();
};

Macro::Macro()
{
    file.open("input.txt", ios::in);
    if (!file.is_open())
    {
        cout << "\nError\n";
    }
    macro_name_table.open("macro_name_table.txt", ios::out);
    if (!macro_name_table.is_open())
    {
        cout << "\nError\n";
    }
    macro_definition_table.open("macro_definition_table.txt", ios::out);
    if (!macro_definition_table.is_open())
    {
        cout << "\nError\n";
    }
    keyword_parameter_definition_table.open("keyword_parameter_definition_table.txt", ios::out);
    if (!keyword_parameter_definition_table.is_open())
    {
        cout << "\nError\n";
    }
    parameter_name_table.open("parameter_name_table.txt", ios::out);
    if (!parameter_name_table.is_open())
    {
        cout << "\nError\n";
    }
}

int Macro::findParameter(string parameter, string name)
{
    int reference;
    for (int i = 0; i < PNTAB.size(); i++)
    {
        if (PNTAB[i] == name)
        {
            reference = i;
            break;
        }
    }
    for (int i = 0; i < PNTAB.size(); i++)
    {
        if (PNTAB[i] == parameter)
        {
            return i - reference;
        }
    }
    return -1;
}

void Macro::pass1()
{
    vector<string> words;
    vector<string> temp;
    string line;
    string name;
    int pcounter = 0;
    int mcounter = 0;
    int kcounter = 0;
    bool insideMacro = false;
    bool firstLine = false;
    MNT m;
    if (file.is_open())
    {
        while (!file.eof())
        {
            getline(file, line);
            string w = "";
            for (int i = 0; i < int(line.size()); i++)
            {
                if (line[i] != ' ')
                {
                    w += line[i];
                }
                else
                {
                    words.push_back(w);
                    w = "";
                }
            }
            words.push_back(w);
            if (words[0] == "MACRO")
            {
                insideMacro = true;
                firstLine = true;
            }
            else if (insideMacro && firstLine)
            {
                name = words[0];
                PNTAB.push_back(name);
                for (int i = 1; i < words.size(); i++)
                {
                    if (words[i].at(0) == '&')
                    {
                        PNTAB.push_back(words[i]);
                        pcounter++;
                    }
                    else if (words[i] == "=")
                    {
                        if (i != words.size() - 1 && words[i + 1].at(0) != '&')
                        {
                            KPDTAB.push_back({words[i - 1], words[i + 1]});
                            kcounter++;
                        }
                        else if (i == words.size() - 1 || words[i + 1].at(0) == '&')
                        {
                            KPDTAB.push_back({words[i - 1], ""});
                            kcounter++;
                        }
                    }
                }
                if (kcounter != 0)
                {
                    m.kpdtp = KPDTAB.size() - kcounter + 1;
                }
                m.kp = kcounter;
                m.pp = pcounter - kcounter;
                m.mdtp = mcounter + 1;
                firstLine = false;
                pcounter = kcounter = 0;
                MNTAB.push_back({words[0], m});
            }
            else if (!firstLine && insideMacro)
            {
                for (int i = 0; i < words.size(); i++)
                {
                    if (words[i].at(0) == '&')
                    {
                        string param = "(P,";
                        param += to_string(findParameter(words[i], name));
                        param += ")";
                        temp.push_back(param);
                    }
                    else
                    {
                        temp.push_back(words[i]);
                    }
                }
                MDTAB.push_back(temp);
                temp.clear();
                mcounter++;
            }
            if (words[0] == "MEND")
            {
                insideMacro = false;
            }
            words.clear();
        }
    }
    else
    {
        cout << "\nError" << endl;
    }
    file.close();
}

void Macro::displayMNTAB()
{
    for (auto x : MNTAB)
    {
        macro_name_table << x.first << " " << x.second.kp << " " << x.second.pp << " " << x.second.mdtp << " " << x.second.kpdtp << endl;
    }
    macro_name_table.close();
}

void Macro::displayKPDTAB()
{
    for (auto x : KPDTAB)
    {
        keyword_parameter_definition_table << x.first << " " << x.second << endl;
    }
    keyword_parameter_definition_table.close();
}

void Macro::displayMDTAB()
{
    for (int i = 0; i < MDTAB.size(); i++)
    {
        for (int j = 0; j < MDTAB[i].size(); j++)
        {
            macro_definition_table << MDTAB[i][j] << " ";
        }
        macro_definition_table << endl;
    }
    macro_definition_table.close();
}

void Macro::displayPNTAB()
{
    for (auto x : PNTAB)
    {
        parameter_name_table << x << endl;
    }
    parameter_name_table.close();
}

int main()
{
    Macro m;
    m.pass1();
    m.displayMNTAB();
    m.displayPNTAB();
    m.displayKPDTAB();
    m.displayMDTAB();
    return 0;
}
