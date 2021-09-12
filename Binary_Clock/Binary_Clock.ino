// author eimon

#include <math.h>

// Rows and Columns
const int R[] = {2, 7, A5, 5, 13, A4, 12, A2};
const int C[] = {6, 11, 10, 3, A3, 4, 8, 9};

// Button
// Yellow +1 min (m2++)
const int yellowButton = 1;
int yellowPressed = 0;

unsigned char a[8][8] =
{
  0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 1, 0, 0, 1, 0, 0,
  0, 0, 1, 0, 0, 1, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0,
};

//starting hour - add manually (decimal)
unsigned int h1 = 1;
unsigned int h2 = 7;
// starting min - add manually (decimal)
unsigned int m1 = 4;
unsigned int m2 = 8;    // Add one less - when program begins counts +1 cause of the button
// starting sec - add manually (decimal)
unsigned int s1 = 0;
unsigned int s2 = 0;

void setup()
{
  Serial.begin(9600);

  for (int i = 0; i < 8; i++)
  {
    pinMode(R[i], OUTPUT);
    pinMode(C[i], OUTPUT);
  }

  // Using pullup so external resistor not needed
  pinMode(yellowButton, INPUT_PULLUP);
}

void loop()
{
  // debug
  unsigned long timeBegin = micros();
  unsigned long timeEnd = timeBegin;

  // loop
  yellowPressed = digitalRead(yellowButton);
  if (yellowPressed == 1)
  {
    m2++;
  }

  manipulate_array(h1, h2, m1, m2);
  blink_a_sec(a);

  yellowPressed = 0;

  s2++;
  if (s2 > 9)
  {
    if (h1 == 2 and h2 == 3 and m1 == 5 and m2 == 9 and s1 == 5)
    {
      h1 = 0; h2 = 0;
      m1 = 0; m2 = 0;
      s1 = 0; s2 = 0;
    }
    else
    {
      s1++;
      s2 = 0;
    }
  }
  if (s1 > 5)
  {
    m2++;
    s1 = 0;
  }
  if (m2 > 9)
  {
    m1++;
    m2 = 0;
  }
  if (m1 > 5)
  {
    h2++;
    m1 = 0;
  }
  if (h2 > 9)
  {
    h1++;
    h2 = 0;
  }

  // debug
  timeEnd = micros();

  // program duration in millis
  Serial.println((timeEnd - timeBegin)/1000);
}

bool is_power_of_two(unsigned int x)
{
    return (x & (x - 1)) == 0;
}

void blink_a_sec(unsigned char data[8][8])
{
  // a second
  unsigned long starttime = micros();
  unsigned long endtime = starttime;
  while ((endtime - starttime) <= 996000) // 1 000 000 - approx program delay
  {
    for (int c = 0; c < 8; c++)
    {
      digitalWrite(C[c], LOW);
      for (int r = 0; r < 8; r++)
      {
        digitalWrite(R[r], data[r][c]);
      }
      delay(1);
      Clear();
    }
    endtime = micros();
  }
  // a second passed

  // debug
  // function duration in micros
  Serial.print("Func: ");
  Serial.println(endtime - starttime);
}

void Clear()
{
  for (int i = 0; i < 8; i++)
  {
    digitalWrite(R[i], LOW);
    digitalWrite(C[i], HIGH);
  }
}


int log_base_two(unsigned int x)
{
  return (int((log(x)/log(2))));
}

void max_value_five(unsigned int t, int c)
{
  if (t == 0)
    for (int r = 0; r < 7; r++)
      a[r][c] = 0;
  else if (is_power_of_two(t))
  {
    for (int r = 0; r < 7; r++)
      a[r][c] = 0;
    a[int(log_base_two(t))][c] = 1;
  }
  else
  {
    switch(t)
    {
      case 3:
      for (int r = 0; r < 7; r++)
        a[r][c] = 0;
      a[1][c] = 1;
      a[0][c] = 1;
      break;
      case 5:
      for (int r = 0; r < 7; r++)
        a[r][c] = 0;
      a[2][c] = 1;
      a[0][c] = 1;
      break;
    }
  }
}

void max_value_nine(unsigned int t, int c)
{
  if (t == 0)
    for (int r = 0; r < 7; r++)
      a[r][c] = 0;
  else if (is_power_of_two(t))
  {
    for (int r = 0; r < 7; r++)
      a[r][c] = 0;
    a[int(log_base_two(t))][c] = 1;
  }
  else
  {
    switch(t)
    {
      case 3:
        for (int r = 0; r < 7; r++)
          a[r][c] = 0;
        a[1][c] = 1;
        a[0][c] = 1;
        break;
      case 5:
        for (int r = 0; r < 7; r++)
          a[r][c] = 0;
        a[2][c] = 1;
        a[0][c] = 1;
        break;
      case 6:
        for (int r = 0; r < 7; r++)
          a[r][c] = 0;
        a[2][c] = 1;
        a[1][c] = 1;
        break;
      case 7:
        for (int r = 0; r < 7; r++)
          a[r][c] = 0;
        a[2][c] = 1;
        a[1][c] = 1;
        a[0][c] = 1;
        break;
      case 9:
        for (int r = 0; r < 7; r++)
          a[r][c] = 0;
        a[3][c] = 1;
        a[0][c] = 1;
        break;
    }
  }
}

void manipulate_array(unsigned int h1, unsigned int h2, unsigned int m1, unsigned int m2)
{
  // hours
  // h1
  if (h1 == 0)
    for (int r = 0; r < 7; r++)
      a[r][7] = 0;
  else if (is_power_of_two(h1))
  {
    for (int r = 0; r < 7; r++)
      a[r][7] = 0;
    a[int(log_base_two(h1))][7] = 1;
  }

  // h2
  max_value_nine(h2, 6);

  // minutes
  // m1
  max_value_five(m1, 4);

  // m2
  max_value_nine(m2, 3);

  // seconds
  // s1
  max_value_five(s1, 1);

  // s2
  max_value_nine(s2, 0);
}
