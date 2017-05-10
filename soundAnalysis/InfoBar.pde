// this file is responsible on drawing the top info bar

public class InfoBar {
  private float width, height;
  private String fileName;
  private float titleWidth;
  private int padding;

  public InfoBar (String _fileName) {
     width = innerWidth;
     height = innerHeight * 0.1;

     fileName = _fileName;
     titleWidth = textWidth(fileName);
     padding = 5;
  }

  void draw() {
    pushMatrix();
    pushStyle();
    translate(margin, margin);

    // file 
    fill(20);
    noStroke();
    rect(0, 0, padding * 2 + titleWidth, padding * 2 + 12);
    
    fill(240);
    text(fileName, padding, padding);
    popStyle();
    popMatrix();
  }
}
