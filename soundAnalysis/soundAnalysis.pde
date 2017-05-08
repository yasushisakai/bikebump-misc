import ddf.minim.*;
import ddf.minim.analysis.*;

String fileName;
Minim minim;
AudioPlayer player;
FFT fft;
int bufferSize = 1024;
int specSize;
float[] bands;

// layout
int margin = 10;
PFont font;

void setup () {

  size(640,480);
  
  fileName = "sound.wav";

  minim = new Minim(this);
  player = minim.loadFile(fileName, bufferSize);
  player.loop();

  fft = new FFT(player.bufferSize(), player.sampleRate());
  fft.noAverages();
  specSize = fft.specSize();
  bands = new float[specSize];
  float bandWidth = (width - margin * 2) / float(specSize);

  //
  font = createFont("Helvetica", 12, false);
  textSize(12);
  textFont(font);
  textAlign(LEFT, TOP);

}

void draw () {
  background(220);
  noFill();
  stroke(20);
  // rect(margin, margin, width - margin*2, height - margin*2);
  
  drawTitle(fileName);
  
  if (player.isPlaying()) {
    fft.forward(player.mix);
  }
  // println("spectrum size", fft.specSize());
  
  noFill();
  pushMatrix();
  translate(margin, margin * 6);
  beginShape();
  
  for (int i = 0; i < specSize; i++) {
    
    if (player.isPlaying()){
      float bandValue = fft.getBand(i);
      // https://github.com/ddf/Minim/blob/master/examples/Analysis/FFT/Windows/Windows.pde
      float bandDB = max(-150, 20 * log(2 * bandValue / fft.timeSize())); // clamp minimum to -150
      float x = map(i, 0, specSize, 0, width - margin * 2);
      float y = getBandY(bandDB, 200);
      vertex(x, y);
    }else { 
    }
  }
  endShape();
  popMatrix();

  // println(maxValue, minValue);

  // drawint circle indicating current position 
  float posX = map(player.position(), 0, player.length(), margin, width - margin * 2);
  
  line(margin, height - margin - 20, width - margin, height - margin - 20);

  fill(240);
  ellipse(posX, height - margin - 20, 10, 10);
}


void mousePressed () {
  if (player.isPlaying()) {
    player.pause();
  } else {
    player.loop();
  }
}

void keyPressed () {

  switch (key) {
    case 'q' :
      terminateSketch();
      break;
    case ' ' :
      println("SPACE");
      break;
  }
}

float getBandY (float dB, float plotHeight) {
  return map(dB, 0, -150, 0.0, plotHeight);
}

float ampDB (float amp) {
  return 20.0 * log10(amp);
}

float log10 (float x) {
  return log(x) / log(10);
}

void terminateSketch () {
  player.close();
  minim.stop();
  exit();
}

void drawTitle (String fileName) {
  pushStyle();
  fill(20);
  noStroke();
  rect(margin, margin * 2 - 3, 5 * 2 + textWidth(fileName), 3 * 2 + 13);
  fill(240);
  text(fileName, margin + 5, margin * 2);
  popStyle();
}
