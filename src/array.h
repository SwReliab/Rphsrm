#pragma once

namespace marlib {

class tmp_array {
private:
  int m_n;
  double* m_ptr;
public:
  tmp_array(int n, double* ptr) : m_n(n), m_ptr(ptr) {}
  virtual ~tmp_array() {}
  double& operator[](int i) { return m_ptr[i]; }
  const double& operator[](int i) const { return m_ptr[i]; }
  int size() const { return m_n; }
};

class tmp_array1 {
private:
  int m_n;
  double* m_ptr;
public:
  tmp_array1(int n) : m_n(n) {
    m_ptr = new double [m_n];
  }
  virtual ~tmp_array1() {
    delete [] m_ptr;
  }
  double& operator[](int i) { return m_ptr[i]; }
  const double& operator[](int i) const { return m_ptr[i]; }
  int size() const { return m_n; }
};

class tmp_array2 {
private:
  int m_m;
  int m_n;
  double* m_ptr;
  std::vector<tmp_array*> m_elem;
  
public:
  tmp_array2(int m, int n) : m_m(m), m_n(n), m_elem(m) {
    m_ptr = new double [m_m * m_n];
    double* p = m_ptr;
    for (int i=0; i<m_m; i++, p+=m_n) {
      m_elem[i] = new tmp_array (m_n, p);
    }
  }
  
  virtual ~tmp_array2() {
    for (int i=0; i<m_m; i++) {
      delete m_elem[i];
    }
    delete [] m_ptr;
  }
  
  tmp_array& operator[](int i) {
    return *m_elem[i];
  }

  const tmp_array& operator[](int i) const {
    return *m_elem[i];
  }
};

}
