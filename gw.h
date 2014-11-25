#ifndef GW_H
#define GW_H

#include <QWidget>
#include <QPainter>
#include <QMouseEvent>

#define WINDOW_SIZE 700
#define MIN_SCALE 0.0625
#define MAX_SCALE 128
#define MULTI_SCALE 1.2
#define MULTI_COORDS_TO_ANGLE 0.005
#define CRIT_COORDS_X 3
#define CRIT_COORDS_Y 3

struct Point { double x, y, z; };
struct Face { Point *p[4], c; };

namespace Ui { class GW; }

class MW;
class Calc;

class GW : public QWidget
{
  Q_OBJECT

public:
  explicit GW(QWidget *parent = 0);
  ~GW();

  void calcobject(Calc *);
  void mainform(MW *);

  void mousePressEvent(QMouseEvent *);
  void mouseMoveEvent(QMouseEvent *);
  void wheelEvent(QWheelEvent *);
  void showEvent(QShowEvent *);
  void paintEvent(QPaintEvent *);
  void closeEvent(QCloseEvent *);

private slots:
  void on_pbapplychangepoints_clicked();
  void on_cbgraphstyle_currentIndexChanged(int);
  void on_cbzscale_stateChanged(int);
  void on_pbapplyaxis_clicked();

private:
  /* froms */
  MW *_mw; //main form
  Ui::GW *_gw; //graphic form
  int _mouse_pos_x, _mouse_pos_y;

  /* misc */
  bool _initialized; //indicates wheather points where initialized
  int *_face_order; //array of face order to display
  Calc *_calc; //calculating class
  Point **_p; //points to display
  Face *_f; //faces to display
  double
    _phi, //angle of rotation in xOy-plate
    _psi, //angle between Oz and xOy-plate
    _scale, //global scale
    _scale_z, //scale of z-axis
    _perspective_half_z; //like a half-life but for perspective ;)
  Point
    _eye_point, //point of view (not moving)
    _center_point; //calculated center due to all coordinates

  void _initialize(); //init memory and bindings
  void _eraseinitialization(); //erases memory

  void _rotate(Point &, double, double, double); //rotate point in spheric
  void _scale_init_points(); //scale points from Calc using "_scale" variable
  void _scale_points(double); //scale points using multiplier
  void _rotate_points(); //rotate all points
  void _set_axises_points(); //set and rotate all axises' points
  void _paint_axises(); //display axises with legend
  void _paint_points(); //display all points
  void _paint_lines(); //display grid of a function
  void _paint_faces(); //display faces of a function
};

#endif // GW_H
