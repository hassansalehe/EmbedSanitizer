//===-- Runtime race detection module of EmbedSanitizer - for Embeded ARM--===//
//
//
// This file is distributed under the BSD 3-clause "New" or "Revised" License
// License. See LICENSE.md for details.
//
//===----------------------------------------------------------------------===//
//
// (c) 2017, 2018 - Hassan Salehe Matar, Koc University
//            Email: hmatar@ku.edu.tr
//===----------------------------------------------------------------------===//

#ifndef ETSAN_FILEDICTIONARY_H_
#define ETSAN_FILEDICTIONARY_H_

#include <unordered_set>
#include <set>
#include <string>

namespace etsan {

/**
 * This class holds the list of files for debuggability of
 * races detected by EmbedSanitizer race detector.
 */
class FileDictionary {
  std::set<std::string> module_paths;
  std::unordered_set<char *> files;

  void inserFile(char *file_name) {
    files.insert(file_name);
  }

  bool exists(char *file_name) {
    return files.find(file_name) != files.end();
  }

  void saveModule(char *module_path) {
    std::string path(module_path);
    module_paths.insert(path);
  }
}; // FileDictionary

}; // etsan
#endif // ETSAN_FILEDICTIONARY_H_
