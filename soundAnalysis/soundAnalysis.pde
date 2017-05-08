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
float lowerBound = 1000;
int lowerIndex;
float higherBound = 5000;
int higherIndex;
float[] gridFreq = {1000, 1500, 2000, 2500, 3000, 3500, 4000, 4500, 5000};
int [] gridIndex = new int [gridFreq.length];
float [] readMidFreq = new float [gridFreq.length]; 
int graphHeight = 250;

void setup () {

  size(640,480);
  
  fileName = "sound.wav";

  minim = new Minim(this);
  player = minim.loadFile(fileName, bufferSize);
  player.play();

  fft = new FFT(player.bufferSize(), player.sampleRate());
  fft.noAverages();
  specSize = fft.specSize();
  bands = new float[specSize];
  float bandWidth = (width - margin * 2) / float(specSize);

  lowerIndex = fft.freqToIndex(lowerBound);
  higherIndex = fft.freqToIndex(higherBound);

  for (int i = 0; i < gridFreq.length; i++) {
    gridIndex[i] = fft.freqToIndex(gridFreq[i]);
    readMidFreq[i] = fft.indexToFreq(gridIndex[i]);
  }

  //
  font = createFont("Helvetica", 12, false);
  textSize(12);
  textFont(font);
  textAlign(LEFT, TOP);
  player.skip(-6000);
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
  
  pushStyle();
  for (int i = 0; i < gridFreq.length; i++) {
    float x = map(gridIndex[i], lowerIndex, higherIndex, margin, width - margin * 2);
    float sy = margin * 6;
    float ey = margin * 6 + graphHeight;
    stroke(20);
    noFill();
    line (x, sy, x, ey);
    fill(20);
    noStroke();
    text(readMidFreq[i], x - textWidth(round(readMidFreq[i]) + "") / 2.0, margin * 6 + graphHeight + 5); 
  }
  popStyle();


  noFill();
  pushMatrix();
  translate(margin, margin * 6);
  beginShape();
  
  for (int i = lowerIndex; i < higherIndex; i++) {
    if (player.isPlaying()){
      float bandValue = fft.getBand(i);
      // https://github.com/ddf/Minim/blob/master/examples/Analysis/FFT/Windows/Windows.pde
      float bandDB = max(-150, 20 * log(2 * bandValue / fft.timeSize())); // clamp minimum to -150
      float x = map(i, lowerIndex, higherIndex, 0, width - margin * 2);
      float y = getBandY(bandDB, graphHeight);
      vertex(x, y);
    }
  }

  endShape();
  popMatrix();
  // println(maxValue, minValue);

  // drawint circle indicating current position 
  float posX = map(player.position(), 0, player.length(), margin, width * 0.80);
  line(margin, height - margin - 20, width * 0.80, height - margin - 20);
  
  String currentFrame = String.format("%04d", player.position());
  String posText = currentFrame + "/" + player.length();
  pushStyle();
  fill(20);
  noStroke();
  rect(width * 0.80 + margin, height - margin - 20 - 3 - 6, textWidth(posText) + 5 * 2, 3 * 2 + 13);
  fill(240);
  text(posText, width * 0.80 + margin + 5, height - margin - 20 - 6);
  popStyle();

  fill(240);
  ellipse(posX, height - margin - 20, 10, 10);

  save("out/frame_" + currentFrame + ".png");

  if(player.position() == player.length()-1) terminateSketch();
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
