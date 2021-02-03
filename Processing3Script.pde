import processing.serial.*; // add the serial library
Serial myPort; // define a serial port object to monitor

float x, y, z, X = 0.0, Y = 0.0; // intitialize x, y, and z float values
String text = "", inString = "", lastInput = ""; // initialize strings
String[] xyzaRaw; // raw data array of strings

void setup() {
  printArray(Serial.list()); // list all available serial ports
  myPort = new Serial(this, Serial.list()[11], 115200); // define input port and baud rate
  myPort.clear(); // clear the port of any initial junk
  fullScreen(P3D); // create fullscreen window
  x = width/2; // set x to center of screen
  y = height/2; // set y to center of screen
  z = 0; // set z to zero
  fill(255, 255, 255); // make the fill white
  stroke(0, 0, 0); // make the stroke black
}

void draw () {
  if (myPort.available () > 0) { // make sure port is open
    inString = ""; // empty string
    lastInput = null; // null string
    while (inString != null) { // empty the buffer
      lastInput = inString; // lastInput is updated
      inString = myPort.readStringUntil('\n'); // read input string
    }
    inString = lastInput; // set inString to the most recent string in the buffer after emptying it
    if (inString != null) { // ignore null strings
      inString = trim(inString); // trim off any whitespace
      xyzaRaw = splitTokens(inString, "\t"); // extract x & y into an array
      // proceed only if correct # of values extracted from the string:
      if (xyzaRaw.length == 2) {
        X = PI * float(xyzaRaw[0]) / 180.0; // calculate exact value to rotate the plane to
        Y = PI * float(xyzaRaw[1]) / 180.0; // calculate exact value to rotate the plane to

        text = "FW/BW angle = " + xyzaRaw[0] + " degrees, L/R angle = " + xyzaRaw[1] + " degrees"; // create a text string with the current relevant data
        background(255, 0, 0); // call background to wipe previous output
        textSize(50); // set text size
        textAlign(CENTER); // set text to align to the center
        text(text, width/2, 100); // print text on screen in the center

        translate(x, y, z); // place the plane in the center of the screen
        rotateX(PI/2 - .05 + X); // rotate about the x axis by PI/2 - .05 + X to get desired orientation + X
        rotateY(Y); // roate about the y axis by Y
        box(600, 600, 10); // draw box 600x600x100 pixels
      }
    }
  }
}
