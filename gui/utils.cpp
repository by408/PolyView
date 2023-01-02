// MIT License Terms (http://en.wikipedia.org/wiki/MIT_License)
//
// Copyright (C) 2011 by Oleg Alexandrov
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.
#include <iostream>
#include <cassert>
#include <cmath>
#include <cstdlib>
#include <limits>
#include <cstring>
#include <algorithm>
#include <gui/utils.h>

// For Visual studio
#ifdef _MSC_VER 
#define strcasecmp _stricmp
#endif

using namespace std;

void utils::printUsage(std::string progName){
  cout << "Usage: " << progName << " "
       << "[ -geo[metry] 1200x800 ] [-bg | -backgroundColor black ] "
       << "[ -c | -color yellow ] "
       << "[ -nc | -noColorOverride ] "
       << "[ -fs | -fontSize 10 ] "
       << "[ -lw | -lineWidth 2 ] "
       << "[ -p | -points ] [ -cp | -closedPoly ] [ -ncp | -nonClosedPoly ] "
       << "[ -f | -filledPoly ] [ -nf | -nonFilledPoly ] [ -cw | clockwisePoly ]"
       << "[ -grid on | off ] [ -gridSize 10 ] [ -gridWidth 1 ] "
       << "[ -gridColor green ] [ -panelRatio 0.2] "
       << "file_1.xg ... file_N.xg " << endl;
}

void utils::extractWindowDims(// inputs
                              int numArgs, char ** args,
                              // outputs
                              int & windowWidX, int & windowWidY){

  // Parse the command line arguments '-geo[metry] 500x600'

  windowWidX = 1200; windowWidY = 800; // defaults

  for (int s = 1; s < numArgs; s++){

    if ( !strstr(args[s-1], "-geo") ) continue;

    string lineStr = args[s];
    char * line    = (char*) lineStr.c_str();

    // Blank the geometry settings once located
    // to not confuse other parsers.
    args[s-1][0] = '\0';
    args[s  ][0] = '\0';

    char * pch;
    char delimiter[] = "x";

    pch = strtok (line, delimiter);
    if (pch == NULL) continue;
    int windowWidX_tmp = atoi(pch);

    pch = strtok (NULL, delimiter);
    if (pch == NULL) continue;
    int windowWidY_tmp = atoi(pch);

    if (windowWidX_tmp > 0 && windowWidY_tmp > 0){
      windowWidX = windowWidX_tmp;
      windowWidY = windowWidY_tmp;
    }

  }

}

void utils::parseCmdOptions(//inputs
                            int argc, char** argv, std::string exeName,
                            // outputs
                            int & windowWidX, int & windowWidY, cmdLineOptions & options){

  options.polyOptionsVec.clear();

  polyOptions opt; // Each polygon file will have one such entry

  // Skip argv[0] as that's the program name
  extractWindowDims(argc - 1, argv + 1, windowWidX, windowWidY);

  for (int argIter = 1; argIter < argc; argIter++){

    char * currArg = argv[argIter];

    if (currArg == NULL || strlen(currArg) == 0) continue;

    if (currArg[0] == '-'){
      // Transform -P into -p, etc.
      transform(currArg, currArg + strlen(currArg), currArg, ::tolower);
    }

    if (strcasecmp( currArg, "-h"     ) == 0 || strcasecmp( currArg, "--h"    ) == 0 ||
        strcasecmp( currArg, "-help"  ) == 0 || strcasecmp( currArg, "--help" ) == 0 ||
        strcasecmp( currArg, "-?"     ) == 0 || strcasecmp( currArg, "--?"    ) == 0 ){
      printUsage(exeName);
      exit(0);
    }

    if ( strcasecmp(currArg, "-p") == 0 || strcasecmp(currArg, "-points") == 0 ){
      opt.plotAsPoints = !opt.plotAsPoints;
      continue;
    }

    if ( strcasecmp(currArg, "-f") == 0 || strcasecmp(currArg, "-filledpoly") == 0 ){
      opt.isPolyFilled = true;
      continue;
    }

    if ( strcasecmp(currArg, "-nf") == 0 || strcasecmp(currArg, "-nonfilledpoly") == 0 ){
      opt.isPolyFilled = false;
      continue;
    }

    if ( strcasecmp(currArg, "-cw") == 0 || strcasecmp(currArg, "-clockwisepoly") == 0 ){
      opt.clockwisePoly = true;
      continue;
    }

    if ( strcasecmp(currArg, "-cp") == 0 || strcasecmp(currArg, "-closedpoly") == 0 ){
      // Plot as closed polygons
      opt.isPolyClosed = forceClosedPoly;
      continue;
    }

    if ( strcasecmp(currArg, "-ncp") == 0 || strcasecmp(currArg, "-nonclosedpoly") == 0 ){
      // Plot as polygonal lines
      opt.isPolyClosed = forceNonClosedPoly;
      continue;
    }

    if ((strcasecmp(currArg, "-bg") == 0 || strcasecmp(currArg, "-backgroundcolor") == 0) &&
        argIter < argc - 1) {
      opt.bgColor = argv[argIter + 1];
      argIter++;
      continue;
    }

    if ((strcasecmp(currArg, "-fs") == 0 || strcasecmp(currArg, "-fontsize") == 0 ) &&
        argIter < argc - 1) {
      int fs = (int)round(atof(argv[argIter + 1]));
      if (fs > 0) opt.fontSize = fs;
      argIter++;
      continue;
    }

    if ((strcasecmp(currArg, "-lw") == 0 || strcasecmp(currArg, "-linewidth") == 0) &&
         argIter < argc - 1) {
      int lw = (int)round(atof(argv[argIter + 1]));
      if (lw > 0) opt.lineWidth = lw;
      argIter++;
      continue;
    }

    if (strcasecmp(currArg, "-gridsize") == 0 && argIter < argc - 1){
      double gs = atof(argv[argIter + 1]);
      if (gs > 0) opt.gridSize = gs;
      argIter++;
      continue;
    }

    if ( strcasecmp(currArg, "-gridwidth") == 0  &&
         argIter < argc - 1
         ){
      int gw = (int)round(atof(argv[argIter + 1]));
      if (gw > 0) opt.gridWidth = gw;
      argIter++;
      continue;
    }

    if ( strcasecmp(currArg, "-gridcolor") == 0  &&
         argIter < argc - 1
         ){
      opt.gridColor = argv[argIter + 1];
      argIter++;
      continue;
    }

    if (strcasecmp(currArg, "-grid") == 0  &&
        argIter < argc - 1             &&
        strcasecmp(argv[argIter + 1], "on") == 0){
      opt.isGridOn = true;
      argIter++;
      continue;
    }

    if ((strcasecmp(currArg, "-c"    ) == 0 ||
         strcasecmp(currArg, "-color") == 0 )
        && argIter < argc - 1){
      opt.useCmdLineColor = true;
      opt.cmdLineColor    = argv[argIter + 1];
      argIter++;
      continue;
    }

    if ((strcasecmp(currArg, "-panelRatio") == 0) &&
        argIter < argc - 1) {
      opt.panelRatio = std::max(std::min(atof(argv[argIter + 1]), 0.9), 0.0);
      argIter++;
      continue;
    }
    
    if ((strcasecmp(currArg, "-nc") == 0 ||
         strcasecmp(currArg, "-nocoloroverride") == 0)){
      opt.useCmdLineColor = false;
      continue;
    }

    // Other command line options are ignored
    if (currArg[0] == '-') continue;

    opt.polyFileName = currArg;

    options.polyOptionsVec.push_back(opt);
  }

  // Push one more time, to guarantee that the options vector is
  // non-empty even if no polygons were provided as input, and to make
  // sure we also parsed the options after the last polygon filename.
  // TODO(oalexan1): This is awkward, it better be stored separately
  // to start with.
  options.polyOptionsVec.push_back(opt);

  return;
}

std::string utils::inFileToOutFile(const std::string & inFile){

  string outFile = "";

  bool lastDot = true;
  for (int s = (int)inFile.length() - 1; s >= 0; s--){

    string currChar = inFile.substr(s, 1);
    if (currChar == "." && lastDot){
      outFile = string("_out") + currChar + outFile;
      lastDot = false;
    }else{
      outFile = currChar + outFile;
    }

  }

  if (outFile.length() == 0){
    cerr << "Invalid filename" << endl;
  }

  return outFile;

}

// Get filename extension and make it lowercase
std::string utils::getFilenameExtension(std::string filename){

  std::string ext;
  std::string::size_type idx;
  idx = filename.rfind('.');

  if (idx != std::string::npos)
    ext = filename.substr(idx+1);

  std::transform(ext.begin(), ext.end(), ext.begin(), ::tolower);
  
  return ext;
}

// Remove everything after the last dot and the dot itself
std::string utils::removeExtension(std::string filename){

  std::string ext;
  std::string::size_type idx;
  idx = filename.rfind('.');

  if (idx == std::string::npos)
    return filename;

  return filename.substr(0, idx);
}

bool utils::isImage(std::string const& filename) {
  string type = utils::getFilenameExtension(filename);

  return (type == "jpg" || type == "jpeg" || type == "png" || type == "tif" || type == "gif" ||
          type == "bmp" || type == "xpm");

}

std::string utils::replaceAll(std::string result,
                              const std::string & replaceWhat,
                              const std::string & replaceWithWhat){

  while(1){
    const int pos = result.find(replaceWhat);
    if (pos == -1) break;
    result.replace(pos,replaceWhat.size(),replaceWithWhat);
  }
  return result;
}

bool utils::readImagePosition(std::string const& filename, std::vector<double> & pos) {

  std::ifstream fh(filename.c_str());
  if (!fh) {
    cerr << "Error: Could not open metadata file: " << filename << endl;
    return false;
  }

  pos.clear();
  double val = -1;
  while (fh >> val) {
    pos.push_back(val);
    if (pos.size() >= 4) {
      break;
    }
  }

  if (pos.size() < 4) {
    std::cerr << "Could not read four values from metadata file " << filename << ".\n";
    return false;
  }

  if (pos[2] == 0 || pos[3] == 0) {
    std::cerr << "Expecting last two values to be non-zero in " << filename << ".\n";
    return false;
  }
  
  return true;
}

// Convert from world coordinates to this image's pixel coordinates.
void utils::worldToImage(double wx, double wy, utils::PositionedImage const& img, // inputs
                  double & ix, double & iy) { // outputs
   ix = (wx - img.pos[0]) / img.pos[2];
   iy = (wy - img.pos[1]) / img.pos[3];

   // Flip in y
   iy = img.qimg.height() - 1 - iy;
}

// The inverse of worldToImage()
void utils::imageToWorld(double ix, double iy, utils::PositionedImage const& img,
                         double & wx, double & wy) { // outputs
  
   // Flip in y
  iy = img.qimg.height() - 1 - iy;
  
  wx = ix * img.pos[2] + img.pos[0];
  wy = iy * img.pos[3] + img.pos[1];
}

// Find the box containing all polygons and images
void utils::setUpViewBox(// inputs
                         const std::vector<dPoly> & polyVec,
                         // outputs
                         double & xll,  double & yll,
                         double & widx, double & widy) {

  // TODO(oalexan1): Must take into account images here
  // TODO(oalexan1): This function must move to polyView.cc as it needs images.
  
  // Given a set of polygons, set up a box containing these polygons.

  double xur, yur; // local variables

  bdBox(polyVec,             // inputs
        xll, yll, xur, yur); // outputs

  // Treat the case of empty polygons
  if (xur < xll || yur < yll) {
    xll = 0.0; yll = 0.0; xur = 1000.0; yur = 1000.0;
  }

  // Treat the case when the polygons are degenerate
  if (xur == xll) { xll -= 0.5; xur += 0.5; }
  if (yur == yll) { yll -= 0.5; yur += 0.5; }

  widx = xur - xll; assert(widx > 0.0);
  widy = yur - yll; assert(widy > 0.0);

  // Expand the box slightly for plotting purposes
  double factor = 0.05;
  xll -= widx*factor; xur += widx*factor; widx *= 1.0 + 2*factor;
  yll -= widy*factor; yur += widy*factor; widy *= 1.0 + 2*factor;

  return;
}
