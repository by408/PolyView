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
#ifndef POLY_UTILS_H
#define POLY_UTILS_H
#include <sstream>
#include <vector>
#include <fstream>
#include <cmath>
#include <set>
#include <complex>
#include <map>
#include <dPoly.h>
#include <geomUtils.h>
#include <dTree.h>

namespace utils{

  void findPolyDiff(const dPoly & P, const dPoly & Q, // inputs
                    std::vector<dPoint> & vP, std::vector<dPoint> & vQ); // outputs

  void bdBox(const std::vector<dPoly> & polyVec,
             // outputs
             double & xll, double & yll,
             double & xur, double & yur);

  void findClosestAnnotation(// inputs
                             double x0, double y0,
                             const std::vector<dPoly> & polyVec,
                             // outputs
                             int & polyVecIndex,
                             int & annoIndexInCurrPoly,
                             double & minDist
                             );

  void findClosestPolyVertex(// inputs
                             double x0, double y0,
                             const std::vector<dPoly> & polyVec,
                             // outputs
                             int & polyVecIndex,
                             int & polyIndexInCurrPoly,
                             int & vertIndexInCurrPoly,
                             double & minX, double & minY,
                             double & minDist
                             );

  void findDistanceBwPolys(// inputs
                           const dPoly & poly1,
                           const dPoly & poly2,
                           // outputs
                           std::vector<segDist> & distVec
                           );

  void findDistanceFromPoly1ToPoly2(// inputs
                                    const dPoly & poly1,
                                    const dPoly & poly2,
                                    // outputs
                                    std::vector<segDist> & distVec
                                    );

  void findDistanceBwPolysBruteForce(// inputs
                                     const dPoly & poly1,
                                     const dPoly & poly2,
                                     // outputs
                                     std::vector<segDist> & distVec);

  void putPolyInMultiSet(const dPoly & P, std::multiset<dPoint> & mP);

  std::pair<std::complex<double>, std::complex<double>>
  getClosestPolyEdge( double x0, double y0,
                      const std::vector<dPoly> & polyVec);

  void findClosestPolyEdge(// inputs
                           double x0, double y0,
                           const std::vector<dPoly> & polyVec,
                           // outputs
                           int & vecIndex, int & polyIndex,
                           int & vertIndex,
                           double & minX, double & minY, double & minDist);
  
  void alignPoly1ToPoly2(dPoly       & poly1,
                         const dPoly & poly2,
                         utils::linTrans & T); // save the applied transform
  
  void findDistanceFromVertsOfPoly1ToVertsPoly2(// inputs
                                                const dPoly & poly1,
                                                const dPoly & poly2,
                                                // outputs
                                                std::vector<segDist> & distVec);


  void markPolysInHlts(// Inputs
                       const std::vector<dPoly> & polyVec,
                       const std::vector<dPoly> & highlights,
                       // Outputs
                       std::map<int, std::vector<int>> & markedPolyIndices,
                       std::map<int, std::vector<int>> & markedAnnoIndices);

  void shiftMarkedPolys(// Inputs
                        std::map<int, std::vector<int>> const& markedPolyIndices,
                        std::map<int, std::vector<int>> const& markedAnnoIndices,
                        double shift_x, double shift_y,
                        // Inputs-outputs
                        std::vector<dPoly> & polyVec);
  void scaleMarkedPolys(// Inputs
                        std::map<int, std::vector<int>> const& markedPolyIndices,
                        std::map<int, std::vector<int>> const& markedAnnoIndices,
                        double scale,
                        // Inputs-outputs
                        std::vector<dPoly> & polyVec);
  void rotateMarkedPolys(// Inputs
                         std::map<int, std::vector<int>> const& markedPolyIndices,
                         std::map<int, std::vector<int>> const& markedAnnoIndices,
                         double angle,
                         // Inputs-outputs
                         std::vector<dPoly> & polyVec);

  void transformMarkedPolys(// Inputs
                            std::map<int, std::vector<int>> const& markedPolyIndices,
                            std::map<int, std::vector<int>> const& markedAnnoIndices,
                            const utils::matrix2 & M,
                            // Inputs-outputs
                            std::vector<dPoly> & polyVec);

  void transformMarkedPolysAroundCtr(// Inputs
                                     std::map<int, std::vector<int>> const& markedPolyIndices,
                                     std::map<int, std::vector<int>> const& markedAnnoIndices,
                                     const utils::matrix2 & M,
                                     // Inputs-outputs
                                     std::vector<dPoly> & polyVec);

  void transformMarkedPolysAroundCtr(// Inputs
                                     std::map<int, std::vector<int>> const& markedPolyIndices,
                                     std::map<int, std::vector<int>> const& markedAnnoIndices,
                                     const utils::matrix2 & M,
                                     dPoint const & ctr,
                                     // Inputs-outputs
                                     std::vector<dPoly> & polyVec);
  void reverseMarkedPolys(// Inputs
                          std::map<int, std::vector<int>> const& markedPolyIndices,
                          // Inputs-outputs
                          std::vector<dPoly> & polyVec);
  void eraseMarkedPolys(// Inputs
                        std::map<int, std::vector<int>> const& markedPolyIndices,
                        std::map<int, std::vector<int>> const& markedAnnoIndices,
                        // Inputs-outputs
                        std::vector<dPoly> & polyVec);
  void extractMarkedPolys(// Inputs
                          const std::vector<dPoly> & polyVec,
                          std::map<int, std::vector<int>> const& markedPolyIndices,
                          // Outputs
                          std::vector<dPoly> & extractedPolyVec);
  int getNumElements(std::map<int, std::vector<int>> const& index_map);

}


#endif
