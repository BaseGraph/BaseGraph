#include "pgl/inputoutput/inputoutput.h"
#include <exception>
#include <set>
// #include <unordered_set>
// #include <map>

// #include <algorithm>


using namespace std;


namespace PGL{

int addVertexToEdgelist(const string name_str, map<string, int> &Name2Num)
{
  int v;
  map< string, int >::iterator name_it = Name2Num.find(name_str);
  if(name_it == Name2Num.end())  // True if this is a new vertex.
    Name2Num[name_str] = Name2Num.size();
  else
    v = name_it->second
  return v;
}


bool addEdgeToEdgelist(string name1_str, string name2_str, multiset< pair<int,int> > &edgelist, map<string, int> &Name2Num, bool allow_selfloops) {
  if(!allow_selfloops)
    if(name1_str == name2_str)
      return false;

  int v1 = addVertexToEdgelist(name1_str, Name2Num);
  int v2 = addVertexToEdgelist(name2_str, Name2Num);

  edgelist.insert(make_pair(v1, v2));
  return true;
}


map<string, int> loadGraphFromEdgelist(const string edgelistFilename, DirectedGraph& graph, bool allow_multiedges, bool allow_selfloops) {
  map<string, int> Name2Num;
  multiset< pair<int, int> > edgelist;
  fstream edgelistFile;
  stringstream currentLine;
  string fullLine, name1_str, name2_str;

  edgelistFile.open(edgelistFilename.c_str(), fstream::in);
  if( !edgelistFile.is_open() )
    throw invalid_argument("ERROR: Could not open file containing edge list.");
  else {
    while( !edgelistFile.eof() ) {
      getline(edgelistFile, fullLine);  // Reads a line of the file.
      edgelistFile >> ws;
      currentLine.str(fullLine);
      currentLine >> ws;
      currentLine >> name1_str >> ws;
      if(name1_str == "#")  // Skips a line of comment.
      {
        currentLine.clear();
        continue;
      }
      currentLine >> name2_str >> ws;
      currentLine.clear();

      addEdgeToEdgelist(name1_str, name2_str, edgelist, Name2Num, allow_selfloops);
    }
  }
  edgelistFile.close();

  graph.resize(Name2Num.size());

  if(allow_multiedges)
    for(auto el : edgelist)
      graph.addEdgeIdx(el.first, el.second, true);
  else {
    set< pair<int, int> > simpleEdgelist(edgelist.begin(), edgelist.end());
    for(auto el : simpleEdgelist)
      graph.addEdgeIdx(el.first, el.second, true);
  }

  return Name2Num;
}


} // namespace PGL
