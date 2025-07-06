/*
 * conditional(x, y, z): Return z if x is 0, return y otherwise.
 * - Ex) conditional(2, 4, 5) = 4, conditional(0, 1, 2) = 2
 */
int conditional(int x, int y, int z) {
  x = !!x;
  x = ~x + 1;
  return (y&x) | (z&~x); 
}
