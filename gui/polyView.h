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
#ifndef POLYVIEW_H
#define POLYVIEW_H

#include <QPolygon>
#include <QMenu>
#include <QContextMenuEvent>
#include <QEvent>
#include <QKeyEvent>
#include <QMouseEvent>
#include <QPaintEvent>
#include <QPixmap>
#include <QWheelEvent>
#include <QWidget>
#include <vector>
#include <map>
#include <utils.h>
#include <chooseFilesDlg.h>

struct cmdLineOptions;

class polyView : public QWidget{
  Q_OBJECT
public:
  polyView(QWidget *parent, const cmdLineOptions & options);
  void runCmd(std::string cmd);

public slots:

  // File menu
  void chooseFilesToShow();
  void openPoly();
  void saveOnePoly();
  void overwriteMultiplePolys();
  void saveAsMultiplePolys();
  void writeMultiplePolys(bool overwrite);
  void reloadPolys();

  // View menu
  void zoomOut();
  void zoomIn();
  void shiftLeft();
  void shiftRight();
  void shiftUp();
  void shiftDown();
  void resetView();
  void changeOrder();
  bool getStringVectorFromGui(std::string title,
                              std::string description,
                              std::vector<std::string> & values);
  void toggleAnno();
  void toggleFilled();
  void toggleShowGrid();
  void toggleDifferentColors();
  void toggleShowPolyDiff();
  void plotNextDiff();
  void plotPrevDiff();
  void plotDiff(int direction);
  void toggleShowPointsEdges();
  void toggleVertOrPolyIndexAnno();
  void toggleLayerAnno();

  // Edit menu
  void undo();
  void redo();
  void create45DegIntPoly();
  void createArbitraryPoly();

  // Transform menu
  void enforce45();
  void enforce45AndSnapToGrid();
  void translatePolys();
  void rotatePolys();
  void scalePolys();

  // Highlights menu
  void createHlt();
  void moveSelectedPolys();
  void deselectPolysDeleteHlts();
  void cutToHlt();
  void deleteSelectedPolys();

  // Options menu
  void setLineWidth();
  void setGridWidth();
  void setGridSize();
  void setGridColor();
  void setBgColor();

  // Right-click menu
  void saveMark();
  void plotMark(double x, double y);
  void toggleMovePolys();
  void toggleMoveVertices();
  void toggleMoveEdges();
  void toggleAlignMode();
  void addAnno();
  void deleteAnno();
  void insertVertex();
  void deleteVertex();
  void deletePoly();
  void copyPoly();
  void translateSelectedPolys();
  void rotateSelectedPolys();
  void scaleSelectedPolys();
  void transformSelectedPolys();
  void pasteSelectedPolys();
  void reverseSelectedPolys();
  void pastePoly();
  void reversePoly();
  void align_rotate90();
  void align_rotate180();
  void align_rotate270();
  void align_flip_against_y_axis();
  void align_flip_against_x_axis();
  void performAlignmentOfClosePolys();

protected:

  void paintEvent(QPaintEvent *);
  void drawPolyBeingPlotted(const std::vector<double> & polyX,
                            const std::vector<double> & polyY,
                            QPainter * paint);
  void resizeEvent(QResizeEvent*);
  void popUp(std::string msg);
  bool getStringFromGui(std::string title, std::string description,
                        std::string inputStr,
                        std::string & outputStr // output
                        );
  bool getRealValuesFromGui(// Inputs
                            std::string title,
                            std::string description,
                            const std::vector<double> & inputVec,
                            // Outputs
                            std::vector<double> & values
                            );
  void setBgFgColorsFromPrefs();
  bool eventFilter(QObject *obj, QEvent *E);
  void mousePressEvent( QMouseEvent *E);
  void mouseMoveEvent( QMouseEvent *E);
  void keyPressEvent( QKeyEvent *K );
  void mouseReleaseEvent ( QMouseEvent * E );
  bool isShiftLeftMouse(QMouseEvent * E);
  void wheelEvent(QWheelEvent *E);
  void contextMenuEvent(QContextMenuEvent *E);

private slots:
 void showFilesChosenByUser();

private:
  void setupViewingWindow();
  void readAllPolys();
  void refreshPixmap();
  void printCmd(std::string cmd, const std::vector<double> & vals);
  void printCmd(std::string cmd, double xll, double yll,
                double widX, double widY);
  void printCmd(std::string cmd);
  void translatePolys(std::vector<double> & shiftVec);
  void rotatePolys(std::vector<double> & angle);
  void translateSelectedPolys(std::vector<double> & shiftVec);
  void rotateSelectedPolys(std::vector<double> & angle);
  void scaleSelectedPolys(std::vector<double> & scale);
  void transformSelectedPolys(std::vector<double> & T);
  void scalePolys(std::vector<double> & scale);
  void transformPolys(std::vector<double> & M);
  void drawMark(int x0, int y0, QColor color, int lineWidth,
                QPainter * paint);
  void setupDisplayOrder(// Inputs
                         int                 numPolys,
                         // Input-output
                         bool              & changeDisplayOrder,
                         std::vector<int>  & polyVecOrder
                         );
  void drawPolyLine(const std::vector<double> & polyX,
                    const std::vector<double> & polyY,
                    QPainter * paint);
  void addPolyVert(int px, int py);
  void appendToPolyVec(const dPoly & P);
  double pixelToWorldDist(int pd);
  void createHighlightWithPixelInputs(int pxll, int pyll, int pxur, int pyur);

  void createHighlightWithRealInputs(double xll, double yll, double xur, double yur);

  void printCurrCoords(const Qt::MouseButton & state, // input
                       int & currX, int  & currY      // in-out
                       );
  bool readOnePoly(// inputs
                   std::string   & filename,
                   bool            plotPointsOnly,
                   closedPolyInfo  isPolyClosed,
                   // output
                   dPoly & poly
                   );
  bool isClosestGridPtFree(std::vector< std::vector<int> > & Grid,
                           int x, int y);
  void initTextOnScreenGrid(std::vector< std::vector<int> > & Grid);
  bool isPolyZeroDim(const QPolygon & pa);
  void centerViewAtPoint(double x, double y);
  void drawOneVertex(int x0, int y0, QColor color, int lineWidth,
                     int drawVertIndex, QPainter * paint);
  void updateRubberBand(QRect & R);

  void displayData( QPainter *paint );
  void plotDPoly(bool plotPoints, bool plotEdges,
                 bool plotFilled, bool showAnno,
                 int lineWidth,
                 int drawVertIndex, // 0 is a good choice here
                 // An empty grid is a good choice if not text is present
                 std::vector< std::vector<int> > & textOnScreenGrid,
                 QPainter *paint,
                 dPoly currPoly // Make a local copy on purpose
                 );
  void resetTransformSettings();
  void pixelToWorldCoords(int px, int py,
                          double & wx, double & wy);
  void worldToPixelCoords(double wx, double wy,
                          int & px,  int & py);


  void setStandardCursor();
  void setPolyDrawCursor();

  void plotDistBwPolyClips( QPainter *paint );

  void saveDataForUndo(bool resetViewOnUndo);
  void restoreDataAtUndoPos();
  double calcGrid(double widx, double widy);

  double m_zoomFactor, m_shiftX, m_shiftY;
  int m_mousePrsX,  m_mousePrsY, m_mouseRelX,  m_mouseRelY;
  int m_screenXll,  m_screenYll, m_screenWidX, m_screenWidY;
  double m_viewXll, m_viewYll,   m_viewWidX,   m_viewWidY;
  double m_prevClickedX, m_prevClickedY;
  double m_screenRatio, m_pixelSize;

  // Polygons
  std::vector<dPoly>       m_polyVec;

  std::vector<polyOptions> m_polyOptionsVec; // options for exiting polygons
  polyOptions m_prefs;                       // options for future polygons

  // Used for undo
  int m_posInUndoStack;
  std::vector< std::vector<dPoly> >       m_polyVecStack;
  std::vector< std::vector<polyOptions> > m_polyOptionsVecStack;
  std::vector< std::vector<dPoly> >       m_highlightsStack;
  std::vector<char>                       m_resetViewStack;

  bool m_resetView;
  bool m_prevClickExists;
  bool m_firstPaintEvent;

  // Use double buffering: draw to a pixmap first, refresh it only
  // if really necessary, and display it when paintEvent is called.
  QPixmap m_pixmap;

  std::vector<QPoint> m_snappedPoints, m_nonSnappedPoints;
  int m_smallLen;

  QRect   m_emptyRubberBand;
  QRect   m_rubberBand;

  bool m_showAnnotations;
  bool m_showFilledPolys;

  std::vector<dPoly> m_highlights;

  int m_showEdges, m_showPoints, m_showPointsEdges, m_displayMode;
  bool m_changeDisplayOrder, m_showLayerAnno;
  int m_showVertOrPolyIndexAnno; // these are conveniently grouped together
  
  bool m_createPoly, m_snapPolyTo45DegreeIntGrid;
  std::vector<double> m_currPolyX, m_currPolyY;
  std::vector<double> m_markX, m_markY;

  bool m_zoomToMouseSelection, m_viewChanged;

  // For the right-click menu
  QMenu  * m_ContextMenu;
  double   m_menuX, m_menuY;
  QAction* m_createArbitraryPoly;
  QAction* m_create45DegIntPoly;
  QAction* m_insertVertex;
  QAction* m_moveVertices;
  QAction* m_moveEdges;
  QAction* m_movePolys;
  QAction* m_deleteVertex;
  QAction* m_showIndices;
  QAction* m_showPolysFilled;
  QAction* m_deletePoly;
  QAction* m_reversePoly;
  QAction* m_copyPoly;
  QAction* m_pastePoly;
  QAction* m_saveMark;
  QAction* m_addAnno;
  QAction* m_deleteAnno;
  QAction* m_alignModeAction;

  QAction* m_align_rotate90;
  QAction* m_align_rotate180;
  QAction* m_align_rotate270;
  QAction* m_align_flip_against_x_axis;
  QAction* m_align_flip_against_y_axis;
  QAction* m_performAlignmentOfClosePolys;

  // If the current point on the polygon being created is closer than
  // this distance (in pixels) from the first point of the polygon, we
  // will assume we arrived back to the first point so we finished
  // creating the polygon.
  int m_pixelTol;

  std::vector<int> m_polyVecOrder;

  // For plotting in diff mode
  bool                        m_diffColorsMode;
  bool                        m_polyDiffMode;
  std::vector<dPoly>          m_polyVecBk;
  std::vector<polyOptions>    m_polyOptionsVecBk;
  std::vector<utils::segDist> m_distVec;       // distances b/w polys to diff
  std::vector<double>         m_segX, m_segY;  // segment to plot
  int                         m_indexOfDistToPlot;


  // Choose which files to hide/show in the GUI
  chooseFilesDlg        m_chooseFilesDlg;
  std::set<std::string> m_filesToHide;

  // Edit mode
  bool   m_movingVertsOrEdgesOrPolysNow;
  bool   m_deletingPolyNow;
  int    m_displayModeBk;
  int    m_polyVecIndex;
  int    m_polyIndexInCurrPoly;
  int    m_vertIndexInCurrPoly;
  double m_mousePressWorldX, m_mousePressWorldY;
  dPoly  m_polyBeforeShift;
  std::map<int, std::map<int, int>> m_selectedPolyIndices;
  std::map<int, std::map<int, int>> m_selectedAnnoIndices;
  std::vector<dPoly> m_polyVecBeforeShift;
  std::vector<dPoly> m_copiedPolyVec;
  bool m_movingPolysInHlts;

  // Align mode (align one file with another file via linear transform)
  bool m_alignMode;
  bool m_aligningPolysNow;
  utils::linTrans m_T, m_totalT;

  bool m_counter_cc; // true if counter-clockwise polygons have positive orientation
};

#endif // POLYVIEW_H
