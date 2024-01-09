// Taken from: https://piandmore.wordpress.com/2020/08/15/rgb-to-hsv-and-vice-versa/

void HSVtoRGB(double hsv[], uint8_t rgb[])
{
  double h = hsv[0];
  double s = hsv[1];
  double v = hsv[2];
  double c = v * s;
  double tmp = h / 60.0;
  double tmp2 = tmp - 2 * floor(tmp / 2);
  double x = c * (1 - abs(tmp2 - 1));
  double m = v - c;
  double r, g, b;
  int i = floor(tmp);

  switch (i) {
    case 0:
      r = c;
      g = x;
      b = 0;
      break;
    case 1:
      r = x;
      g = c;
      b = 0;
      break;
    case 2:
      r = 0;
      g = c;
      b = x;
      break;
    case 3:
      r = 0;
      g = x;
      b = c;
      break;
    case 4:
      r = x;
      g = 0;
      b = c;
      break;
    case 5:
      r = c;
      g = 0;
      b = x;
      break;
  }
  rgb[0] = constrain((int)255 * (r + m), 0, 255);
  rgb[1] = constrain((int)255 * (g + m), 0, 255);
  rgb[2] = constrain((int)255 * (b + m), 0, 255);
}

void RGBtoHSV(uint8_t rgb[], double hsv[])
{
  uint8_t xMin = rgb[0];
  if (rgb[1] < xMin) {
    xMin = rgb[1];
  }
  if (rgb[2] < xMin) {
    xMin = rgb[2];
  }
  uint8_t xMax = rgb[0];   if (rgb[1] > xMax) {
    xMax = rgb[1];
  }
  if (rgb[2] > xMax) {
    xMax = rgb[2];
  }
  hsv[2] = xMax;

  uint8_t delta = xMax - xMin;

  if (xMax != 0) {
    hsv[1] = (int)(delta) * 255 / xMax;
  } else {
    hsv[0] = 0;
    hsv[1] = 0;
    return;
  }

  if (rgb[0] == xMax) {
    hsv[0] = (rgb[1] - rgb[2]) * 60 / delta;
  } else if (rgb[1] == xMax) {
    hsv[0] = 120 + (rgb[2] - rgb[0]) * 60 / delta;
  } else {
    hsv[0] = 240 + (rgb[0] - rgb[1]) * 60 / delta;
  }
  if (hsv[0] < 0) {
    hsv[0] += 360;
  }
}
