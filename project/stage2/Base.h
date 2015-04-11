#ifndef BASE_H
#define BASE_H
class Base {
 protected:
  int **_data;
  int _col, _row;
  void create(int col, int row = 1);
  void clean();
public:
  Base();
  Base(const Base &);
  Base(int col, int row = 1);
  ~Base();
  void set(int **, int col, int row = 1);
  void set_one(int value, int pos_x, int pos_y);
  int *size() const;
  int get(int pos_x , int pos_y) const;
  bool all(int **keys, int col, int row = 1) const;
  bool any(int **keys, int col, int row = 1) const;
};
#endif
