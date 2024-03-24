#ifndef K_CONFIG_ASSIMILATE
#define K_CONFIG_ASSIMILATE

#include <iostream>
#include <std-k.hpp>

const std::string ProgramName = "assimilate";
const std::string Version = "0.0.0";
const std::string UsageNotes = R"(usage: assimilate [ -h/-v ]
options:
    -h / --help         show help and usage notes
    -v / --version      print version and exit)";

const std::string HamzaDir = HOME + "/.hamza";

const std::string CommandsDir = HamzaDir + "/commands/";

const std::vector<std::string> Stopwords = {
    "a", "an", "and", "are", "as", "at", "be", "but", "by", "for", "if", "in",
    "into", "is", "it", "no", "not", "of", "on", "or", "such", "that", "the",
    "their", "then", "there", "these", "they", "this", "to", "was", "will",
    "with"
};


#endif

// Copyright (c) 2023, Maxamilian Kidd-May
// All rights reserved.

// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree. 

