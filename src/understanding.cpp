#include "../inc/understanding.hpp"
#include <spacy/doc.h>

Understanding::Understanding(class Commands Commands, Spacy::Spacy &spacy, Spacy::Nlp &nlp) : \
        Cmds(Commands), spacy(spacy), nlp(nlp) {

    /* for(JCommand &C: Commands) { */
    /*     std::vector<Spacy::Doc> TmpDocs; */
    /*     for(std::string &B: C.Bases) { */
    /*         /1* std::cout << "parse:" << C.Name << std::endl; *1/ */
    /*         TmpDocs.push_back(nlp.parse(B)); */
    /*     } */
    /*     C.BaseDocs = TmpDocs; */
    /* } */
}

Understanding::~Understanding() {
    for(auto &C: JCommands)
        C.BaseDocs.clear();
}

double Understanding::Compare(std::string One, std::string Two) {
    double Similarity;
    auto Doc1 = nlp.parse(One);
    auto Doc2 = nlp.parse(Two);
    Similarity = Doc1.similarity(Doc2);
    /* std::cout << doc1.similarity(doc2) << std::endl; */
    return Similarity;
}

int Understanding::Attribute(std::string Input, std::string &CMD) {
    /* k::Time Time; */
    /* std::vector<std::string> Commands(10); */
    /* int n(0); */
    /* Commands.at(n) = "lights on"; */
    /* Commands.at(++n) = "lights off"; */
    /* Commands.at(++n) = "create new program"; */
    /* Commands.at(++n) = "add new program"; */
    /* Commands.at(++n) = "take a photo"; */
    /* Commands.at(++n) = "delete a photo"; */
    /* Commands.at(++n) = "create named cpp"; */
    /* Commands.at(++n) = "add project cpp"; */
    /* Commands.at(++n) = "create program type new destroy"; */
    /* Commands.at(++n) = "create program destroy new type"; */
    /* k::VPrint(Commands); */

    RemoveStopwords(Input);

    Spacy::Doc D = nlp.parse(Input);
    double Highest(0);
    std::string HighestCMD;
    std::string Name;
    int i(0);
    for(auto C: Cmds.GetCommandsMaster()) {
        int j(0);
        for(auto S: C.BaseSentences) {
            // Make this rely on an already created doc
            double Similarity = D.similarity(S.Doc);
            std::cout << Similarity << ": " << S.String << std::endl;
            if(Similarity > Highest) {
                Name = C.Name;
                Highest = Similarity;
                HighestCMD = S.String;
            }
            j++;
        }
        i++;
    }

    Highest = 0;
    for(auto C: Cmds.GetCommandsMaster()) {
        if(C.Name == Name) {
            std::cout << std::endl <<  "Command: " << C.Name << std::endl \
                << "(" << HighestCMD << ")" << std::endl;
            for(auto S: C.Sentences) {
                double Similarity = D.similarity(S.Doc);
                if(Similarity > Highest) {
                    /* std::cout << Similarity << ": " << S.Command << std::endl; */
                    Highest = Similarity;
                    CMD = S.Command;
                    HighestCMD = S.String;
                    if(S.Optargs.size() > 0) {
                        for(std::string O: S.Optargs) {
                            /* std::cout << O << std::endl; */
                            size_t pos = Input.find(O);
                            std::string Optarg;
                            if (pos != std::string::npos) {
                                // If substring is found, get the word next to it
                                size_t start = pos + O.length() + 1; // Start after the substring and the space
                                size_t end = Input.find(" ", start); // Find the next space after the substring
                                Optarg = Input.substr(start, end - start); // Extract the word
                                
                                /* std::cout << "Substring found at position " << pos << std::endl; */
                                /* std::cout << "The word next to it is: " << Optarg << std::endl; */
                                pos = CMD.find("optarg");
                                
                                if (pos != std::string::npos) {
                                    // If word is found, replace it
                                    CMD.replace(pos, 6, Optarg);
                                    /* std::cout << CMD << std::endl; */
                                } else {
                                    /* std::cout << "Word not found." << std::endl; */
                                }
                            } else {
                                /* std::cout << "Substring not found." << std::endl; */
                            }
                        }
                    }
                }
            }
        }
    }
    
    std::cout << "(" << HighestCMD << ")" << std::endl;

    return 0;
}

Spacy::Token Understanding::GetRootClause(Spacy::Doc doc, std::string &Clause) {
    for (const auto& token: doc.tokens()) {
        std::vector<std::string> children;
        std::vector<std::string> ancestors;
        PopulateFamily(token, children, ancestors);
        
        /* DisplayParts(token, children, ancestors); */

        /* std::cout << token.dep_() << "-" << token.text() << std::endl; */

        if(token.dep_() == "ROOT") {
            /* std::cout << "v: " << token.text() << std::endl; */
            Clause = token.text();
            for(auto Part: token.children()) {
                /* std::cout << "C: " << Part.text() << std::endl; */
                Clause = Clause + " " + Part.text();
            }
            auto doc1 = nlp.parse(Clause);
            return token;
        }
    }
}

std::vector<Spacy::Token> Understanding::GetClauses(Spacy::Doc doc, std::vector<std::string> &Clauses) {
    std::vector<Spacy::Token> tokens;
    std::vector<std::string> Verbs;
    for (const auto& token: doc.tokens()) {

        std::vector<std::string> children;
        std::vector<std::string> ancestors;
        PopulateFamily(token, children, ancestors);
        
        /* DisplayParts(token, children, ancestors); */

        /* std::cout << token.pos_() << "-" << token.text() << std::endl; */

        if(token.pos_() == "VERB") {
            /* std::cout << "v: " << token.text() << std::endl; */
            Verbs.push_back(token.text());
            std::string Clause = token.text();
            for(auto Part: token.children()) {
                /* std::cout << "C: " << Part.text() << std::endl; */
                Clause = Clause + " " + Part.text();
            }
            tokens.push_back(token);
            auto doc1 = nlp.parse(Clause);
            Clauses.push_back(Clause);
        }
    }
    return tokens;
}
void Understanding::DisplayParts(Spacy::Token token, std::vector<std::string> children, \
                        std::vector<std::string> ancestors) {
        std::cout << token.text() << "\t" << token.i() << "\t"
                  << token.pos_() << "\t" << token.dep_() << "\t"
                  << "[";
        for (size_t i = 0; i < ancestors.size(); ++i) {
            std::cout << ancestors[i];
            if (i != ancestors.size() - 1) {
                std::cout << ", ";
            }
        }
        std::cout << "]" << "\t" << "[";
        for (size_t i = 0; i < children.size(); ++i) {
            std::cout << children[i];
            if (i != children.size() - 1) {
                std::cout << ", ";
            }
        }
        std::cout << "]" << std::endl;
}

void Understanding::PopulateFamily(Spacy::Token token, std::vector<std::string> &children, \
                        std::vector<std::string> &ancestors) {
        for (const auto& t : token.ancestors())
            ancestors.push_back(t.text());
        for (const auto& t : token.children())
            children.push_back(t.text());
}

void Understanding::RemoveBase(std::string Base, std::string &Clause) {
    // construct a stream from the string 
    std::stringstream ss(Base);
 
    std::string B; 
    while (std::getline(ss, B, ' ')) { 
        std::string::size_type i = Clause.find(B);

        if (i != std::string::npos)
           Clause.erase(i, B.length());
    }
}

void Understanding::RemoveStopwords(std::string &sentence) {
    // Create a lambda function to check if a given word is a stopword
    auto isStopword = [&](const std::string& word) {
        return std::find(Stopwords.begin(), Stopwords.end(), word) != Stopwords.end();
    };

    // Split the sentence into words
    std::vector<std::string> words;
    size_t startPos = 0;
    size_t foundPos = sentence.find_first_of(' ');
    while (foundPos != std::string::npos) {
        words.push_back(sentence.substr(startPos, foundPos - startPos));
        startPos = foundPos + 1;
        foundPos = sentence.find_first_of(' ', startPos);
    }
    words.push_back(sentence.substr(startPos));

    // Remove stopwords from the words vector
    words.erase(std::remove_if(words.begin(), words.end(), isStopword), words.end());

    // Reconstruct the sentence without stopwords
    sentence.clear();
    for (const std::string& word : words) {
        sentence += word + ' ';
    }

    // Remove the trailing space, if any
    if (!sentence.empty()) {
        sentence.pop_back();
    }
}

double Understanding::FindInWords(std::string Opt, std::vector<Spacy::Doc> WordDoc) {
    double Highest = 0;
    auto OptDoc = nlp.parse(Opt);
    for(Spacy::Doc doc: WordDoc) {
        double Sim = doc.similarity(OptDoc);
        /* std::cout << "Finding: -" << Opt << "- In Word: -" << doc.text() << "- = " << Sim << std::endl; */
        if(Sim > Highest)
            Highest = Sim;
    }
    return Highest;
}

double Understanding::FindInWords(std::string Opt, std::vector<Spacy::Doc> WordDoc, std::string &Return) {
    double Highest = 0;
    auto OptDoc = nlp.parse(Opt);
    for(Spacy::Doc doc: WordDoc) {
        double Sim = doc.similarity(OptDoc);
        /* std::cout << "Finding: " << Opt << " In Word: " << doc.text() << " = " << Sim << std::endl; */
        if(Sim > Highest) {
            Highest = Sim;
            Return = doc.text();
        }
    }
    return Highest;
}

std::string Understanding::ParseCommand(JCommand C, std::vector<std::string> PassingArgs, \
                                     std::vector<std::string> PassingFlags, \
                                     std::vector<std::string> PassingNoOptFlags) {
    std::string CMD = C.Command;
    for(int i(0); i<PassingArgs.size(); i++)
        CMD = CMD + " " + PassingFlags.at(i) + " " + PassingArgs.at(i); // need to add distinction between flags that tak args and dont so iterators line up
    for(std::string Flag: PassingNoOptFlags)
        CMD = CMD + " " + Flag;
    for(std::string Flag: C.DefaultFlags)
        CMD = CMD + " " + Flag;
    return CMD;
}

std::vector<std::string> Understanding::GetListArgs(int t, JCommand C, std::string Cmd) {
    std::vector<std::string> Args;
    Cmd = Cmd + " " + C.Flags.at(t) + " list";
    /* std::cout << Cmd << std::endl; */
    Child Command(Cmd.c_str());
    while(!Command.QuestionExit()) {
        std::vector<std::string> tmp;
        std::string read = k::StripTrailingNL(Command.Read());
        k::SplitString(read, '.', tmp, 1);
        Args.push_back(tmp.at(0));
    }
    /* std::cout << "args:\n"; */
    /* k::VPrint(Args); */
    return Args;
}

std::string Understanding::RemoveExtraWhitespace(const std::string& str) {
    std::string result;
    bool previousSpace = false;
    bool leadingSpace = true;

    for (char c : str) {
        if (std::isspace(c)) {
            if (!previousSpace) {
                if (!leadingSpace) {
                    result.push_back(' ');
                }
                previousSpace = true;
            }
        } else {
            result.push_back(c);
            previousSpace = false;
            leadingSpace = false;
        }
    }

    return result;
}

std::string Understanding::RemoveUsedOption(const std::string& str) {
    std::istringstream iss(str);
    std::string token;
    std::string result;

    bool prevSpace = false;

    while(std::getline(iss, token, ' ')) {
        if (token.empty()) {
            if (prevSpace) {
                result += "OPTUSED ";
            }
            prevSpace = true;
        } else {
            result += token + " ";
            prevSpace = false;
        }
    }

    // Remove trailing whitespace
    if (!result.empty() && result[result.length() - 1] == ' ') {
        result.erase(result.length() - 1);
    }

    return result;
}


// Copyright (c) 2023, Maxamilian Kidd-May
// All rights reserved.

// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree. 

