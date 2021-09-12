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
unsigned int h2 = 6;
// starting min - add manually (decimal)
unsigned int m1 = 0;
unsigned int m2 = 0;    // Add one less - when program begins counts +1 cause of the button
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
  if (h2 == 0)
    for (int r = 0; r < 7; r++)
      a[r][6] = 0;
  else if (is_power_of_two(h2)) 
  {
    for (int r = 0; r < 7; r++)
      a[r][6] = 0;
    a[int(log_base_two(h2))][6] = 1;
  }
  else
  {
    switch(h2)
    {
      case 3:
        for (int r = 0; r < 7; r++)
          a[r][6] = 0;
        a[1][6] = 1;
        a[0][6] = 1;  
        break;
      case 5:
        for (int r = 0; r < 7; r++)
          a[r][6] = 0;
        a[2][6] = 1;
        a[0][6] = 1;  
        break;
      case 6:
        for (int r = 0; r < 7; r++)
          a[r][6] = 0;
        a[2][6] = 1;
        a[1][6] = 1;  
        break;
      case 7:
        for (int r = 0; r < 7; r++)
          a[r][6] = 0;
        a[2][6] = 1;
        a[1][6] = 1;  
        a[0][6] = 1; 
        break;
      case 9:
        for (int r = 0; r < 7; r++)
          a[r][6] = 0;
        a[3][6] = 1;
        a[0][6] = 1; 
        break;
    }
  }

  // minutes
  // m1
  if (m1 == 0)
    for (int r = 0; r < 7; r++)
      a[r][4] = 0;
  else if (is_power_of_two(m1)) 
  {
    for (int r = 0; r < 7; r++)
      a[r][4] = 0;
    a[int(log_base_two(m1))][4] = 1;
  }
  else
  {
    switch(m1)
    {
      case 3:
      for (int r = 0; r < 7; r++)
        a[r][4] = 0;
      a[1][4] = 1;
      a[0][4] = 1;  
      break;
      case 5:
      for (int r = 0; r < 7; r++)
        a[r][4] = 0;
      a[2][4] = 1;
      a[0][4] = 1;  
      break;
    }
  }
  // m2
  if (m2 == 0)
    for (int r = 0; r < 7; r++)
      a[r][3] = 0;
  else if (is_power_of_two(m2)) 
  {
    for (int r = 0; r < 7; r++)
      a[r][3] = 0;
    a[int(log_base_two(m2))][3] = 1;
  }
  else
  {
    switch(m2)
    {
      case 3:
        for (int r = 0; r < 7; r++)
          a[r][3] = 0;
        a[1][3] = 1;
        a[0][3] = 1;  
      break;
      case 5:
        for (int r = 0; r < 7; r++)
          a[r][3] = 0;
        a[2][3] = 1;
        a[0][3] = 1;  
        break;
      case 6:
        for (int r = 0; r < 7; r++)
          a[r][3] = 0;
        a[2][3] = 1;
        a[1][3] = 1;  
        break;
      case 7:
        for (int r = 0; r < 7; r++)
          a[r][3] = 0;
        a[2][3] = 1;
        a[1][3] = 1;  
        a[0][3] = 1; 
        break;
      case 9:
        for (int r = 0; r < 7; r++)
          a[r][3] = 0;
        a[3][3] = 1;
        a[0][3] = 1; 
        break;
    }
  }

  // seconds
  // s1
  if (s1 == 0)
    for (int r = 0; r < 7; r++)
      a[r][1] = 0;
  else if (is_power_of_two(s1)) 
  {
    for (int r = 0; r < 7; r++)
      a[r][1] = 0;
    a[int(log_base_two(s1))][1] = 1;
  }
  else
  {
    switch(s1)
    {
      case 3:
      for (int r = 0; r < 7; r++)
        a[r][1] = 0;
      a[1][1] = 1;
      a[0][1] = 1;  
      break;
      case 5:
      for (int r = 0; r < 7; r++)
        a[r][1] = 0;
      a[2][1] = 1;
      a[0][1] = 1;  
      break;
    }
  }

  // s2
  if (s2 == 0)
    for (int r = 0; r < 7; r++)
      a[r][0] = 0;
  else if (is_power_of_two(s2))
  { 
    for (int r = 0; r < 7; r++)
      a[r][0] = 0;
    a[int(log_base_two(s2))][0] = 1;
  }
  else
  {
    switch(s2)
    {
      case 3:
        for (int r = 0; r < 7; r++)
          a[r][0] = 0;
        a[1][0] = 1;
        a[0][0] = 1;  
      break;
      case 5:
        for (int r = 0; r < 7; r++)
          a[r][0] = 0;
        a[2][0] = 1;
        a[0][0] = 1;  
        break;
      case 6:
        for (int r = 0; r < 7; r++)
          a[r][0] = 0;
        a[2][0] = 1;
        a[1][0] = 1;  
        break;
      case 7:
        for (int r = 0; r < 7; r++)
          a[r][0] = 0;
        a[2][0] = 1;
        a[1][0] = 1;  
        a[0][0] = 1; 
        break;
      case 9:
        for (int r = 0; r < 7; r++)
          a[r][0] = 0;
        a[3][0] = 1;
        a[0][0] = 1; 
        break;
    }
  }  
}
