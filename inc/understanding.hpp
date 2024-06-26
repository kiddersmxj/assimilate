#ifndef K_UNDERSTANDING

#include "config.hpp"
#include "commands.hpp"
#include "child.hpp"

#include <sstream>
#include <algorithm>
#include <spacy/spacy>
#include <std-k.hpp>

#define NLOHMANN_JSON_PRESERVE_ORDER
#include "nlohmann/json.hpp"

class Understanding {
    public:
        Understanding(Commands Commands, Spacy::Spacy &spacy, Spacy::Nlp &nlp);
        ~Understanding();
        double Compare(std::string One, std::string Two);
        int Attribute(std::string Input, std::string &CMD);
    private:
        std::vector<JCommand> JCommands;
        Commands Cmds;
        Spacy::Spacy spacy;
        Spacy::Nlp nlp;
        Spacy::Token GetRootClause(Spacy::Doc doc, std::string &Clause);
        std::vector<Spacy::Token> GetClauses(Spacy::Doc doc, std::vector<std::string> &Clauses);
        void DisplayParts(Spacy::Token token, std::vector<std::string> children, \
                                    std::vector<std::string> ancestors);
        void PopulateFamily(Spacy::Token token, std::vector<std::string> &children, \
                                    std::vector<std::string> &ancestors);
        void RemoveBase(std::string Base, std::string &Clause);
        void RemoveStopwords(std::string &Input);
        double FindInWords(std::string Opt, std::vector<Spacy::Doc> WordDoc);
        double FindInWords(std::string Opt, std::vector<Spacy::Doc> WordDoc, std::string &Return);
        std::string ParseCommand(JCommand C, std::vector<std::string> PassingArgs, \
                                 std::vector<std::string> PassingFlags, \
                                 std::vector<std::string> PassingNoOptFlags);
        std::vector<std::string> GetListArgs(int t, JCommand C, std::string Cmd);
        std::string RemoveExtraWhitespace(const std::string& str);
        std::string RemoveUsedOption(const std::string& str);
};

#endif


// Copyright (c) 2023, Maxamilian Kidd-May
// All rights reserved.

// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree. 

