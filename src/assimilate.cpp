#include "../inc/assimilate.hpp"
#include <spacy/doc.h>

int main(int argc, char** argv) {
    int HelpFlag = 0;
    int VersionFlag = 0;
    int FRFlag = 0;
    int UnderstandingFlag = 0;
    int TestFlag = 0;
    std::string Parse = "";
    int opt;

    // Get opt option defenitions
    struct option Opts[] = {
        { "help", no_argument, &HelpFlag, 1 },
        { "version", no_argument, &VersionFlag, 1 },
        { "assimilate", no_argument, &UnderstandingFlag, 1 },
        { "parse", required_argument, NULL, 'p' },
        { "test", no_argument, &TestFlag, 't' },
        { 0 }
    };

    // Infinite loop, to be broken when we are done parsing options
    while (1) {
        opt = getopt_long(argc, argv, "hvfap:t", Opts, 0);

        // A return value of -1 indicates that there are no more options
        if (opt == -1) {
            if(HelpFlag && VersionFlag) {
                Usage();
                return EXIT_FAILURE;
            }
            break;
        } 

        switch (opt) {
        case 'h':
            HelpFlag = 1;
            break;
        case 'v':
            VersionFlag = 1;
            break;
        case 'f':
            FRFlag = 1;
            break;
        case 'a':
            UnderstandingFlag = 1;
            break;
        case 'p':
            Parse = optarg;
            UnderstandingFlag = 1;
            break;
        case 't':
            TestFlag = 1;
            break;
        case '?':
            Usage();
            return EXIT_FAILURE;
        default:
            Usage();
            break;
        }
    }

    // Act on flags
    if(HelpFlag) {
        Usage();
        return EXIT_SUCCESS;
    }
    if(VersionFlag) {
        PrintVersion();
        return EXIT_SUCCESS;
    }

    if(TestFlag) {
        k::Time Time;
        Spacy::Spacy spacy;
        Spacy::Nlp nlp(spacy.load("en_core_web_md"));
        Commands Commands(spacy, nlp);
        Understanding Assimilate(Commands, spacy, nlp);;
        std::cout << Time.Close() << "ms" << std::endl;
        /* Commands.Create(); */

        /* std::vector<Command> Commands; */
        /* LoadCommands(Commands); */
        /* CreateCommandIterations(); */
    }

    if(UnderstandingFlag) {
        k::Time Time;
        Spacy::Spacy spacy;
        Spacy::Nlp nlp(spacy.load("en_core_web_md"));
        Commands Commands(spacy, nlp);
        Understanding Assimilate(Commands, spacy, nlp);;
        std::cout << Time.Close() << "ms" << std::endl;

        if(Parse == "") {
            /* std::cout << Meaning.Compare("called", "with the name of") << std::endl; */
            /* std::cout << Meaning.Attribute("turn off the lights") << std::endl; */
            /* std::cout << Meaning.Attribute("create a cpp project called assimilate") << std::endl; */
            /* std::cout << Meaning.Attribute("create a cpp program named assimilate") << std::endl; */
            /* std::cout << Meaning.Attribute("add a cpp class called assimilate") << std::endl; */
            /* std::cout << Meaning.Attribute("add a class file named assimilate of type cpp") << std::endl; */
            std::string Corpus;
            std::cout << "Enter corpus:" << std::endl;
            getline(std::cin, Corpus);
            while(Corpus != "") {
                std::string CMD;
                int Read = Assimilate.Attribute(Corpus, CMD);
                if(Read != -1) {
                    std::cout
                            << std::endl
                            << "*************************" << std::endl
                            << CMD << std::endl
                            << "*************************" << std::endl
                            << std::endl;
#ifndef TEST
                    if(Read == 1) Execute Execute(CMD);
                    if(Read == 0) Child Child(CMD.c_str());
#endif
                }
                std::cout << std::endl << "Enter corpus:" << std::endl;
                getline(std::cin, Corpus);
            }
        } else  {
            std::string CMD;
            int Read = Assimilate.Attribute(Parse, CMD);
            if(Read != -1) {
                std::cout
                        << std::endl
                        << "*************************" << std::endl
                        << CMD << std::endl
                        << "*************************" << std::endl
                        << std::endl;
#ifndef TEST
                if(Read == 1) Execute Execute(CMD);
                if(Read == 0) Child Child(CMD.c_str());
#endif
            }
        }
        return EXIT_SUCCESS;
    }

    Usage();
    return EXIT_FAILURE;
}

void Usage() {
    std::cout << UsageNotes << std::endl;
}

void Usage(std::string Message) {
    std::cout << Message << std::endl;
    std::cout << UsageNotes << std::endl;
}

void PrintVersion() {
    std::cout << ProgramName << ": version " << Version << std::endl;
}

// Copyright (c) 2023, Maxamilian Kidd-May
// All rights reserved.

// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree. 

