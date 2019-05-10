#ifndef  UNCERTAIN_H_
#define  UNCERTAIN_H_

typedef float approximate_t;

float gcovunf(approximate_t, approximate_t);
float best_guessunf(approximate_t);
approximate_t svarunf(approximate_t, float);
approximate_t cvarunf(approximate_t, float);

approximate_t sqrtunf(approximate_t);
approximate_t asinunf(approximate_t);
approximate_t atan2unf(approximate_t y, approximate_t x);

#endif
