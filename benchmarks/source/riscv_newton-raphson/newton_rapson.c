#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <uncertain.h>
#include <printf.h>
#include <assert.h>

float c = 0;
float b = 0;

float f(float x)
{
	return x*x + b*x - c;
}

float df(float x)
{
	return 2*x + b;
}

float percentage_error(float x, float y) {
	if (x == y) {
		return 0;
	}
	float diff = fabsf(x - y);
	float min = fminf(fabsf(x), fabsf(y));
	return diff / min * 100;
}

void newton_rapson() {
	float x = 0;
	float x_new = x - f(x)/df(x);
	float diff = x_new - x;

	while(fabs(diff) > 1e-5){
		x = x_new;
		x_new = x - f(x)/df(x);
		diff = x_new - x;
	}
	printf_(
		"%.7f\t%.7f\t%.7f\t%.7f\t%.7f\t%.7f\t%.2f%%\t%.2f%%\t%.2f%%\n",
		unf_var(b), unf_var(c), unf_covar(b, c),
		unf_var(x), unf_covar(x, b), unf_covar(x, c),
		percentage_error(unf_var(x), 0.09673 * unf_var(b) - 0.11755 * unf_covar(b, c) + 0.03571 * unf_var(c)),
		percentage_error(unf_covar(x, b), -0.31102 * unf_var(b) + 0.18898 * unf_covar(b, c)),
		percentage_error(unf_covar(x, c), -0.31102 * unf_covar(b, c) + 0.18898 *unf_var(c))
	);
}

int main()
{
	float var_b, var_c, four, stddev_ratio;
	printf_("\n");
	printf_("variance[b]\tvariance[c]\tcovariance[bc]\tvariance[x]\tcovariance[xb]\tcovariance[xc]\terr[x]\terr[xb]\terr[xc]\n");
	for (var_b = 0; var_b < 0.5; var_b += 0.1) {
		b = unf_create(2, var_b);
		for (var_c = 0; var_c < 0.5; var_c += 0.1) {
			if (var_b != 0) {
				stddev_ratio = unf_create_exact(sqrtf(var_c / var_b));

				// corr[b, c] = -1
				c = 6 + stddev_ratio * (2 - b);
				newton_rapson();

				// corr[b, c] = 1
				c = 6 + stddev_ratio * (b - 2);
				newton_rapson();
			}
			if (var_c > var_b) {
				four = unf_create(4, var_c - var_b);

				// covar[b, c] = var[b]
				c = four + b;
				newton_rapson();
			}
			// corr[b, c] = 0
			c = unf_create(6, var_c);
			newton_rapson();
		}
	}

	return 0;
}
