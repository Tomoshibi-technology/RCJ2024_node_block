#ifndef _H_COSSIM_
#define _H_COSSIM_

#include <Arduino.h>

struct Vector {
  int x;
  int y;
};

double dotProduct(const Vector& a, const Vector& b);
double norm(const Vector& v);
double cosineSimilarity(const Vector& a, const Vector& b);

#endif
