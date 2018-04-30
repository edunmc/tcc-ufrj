void setup_display() {
  display.begin(SSD1306_SWITCHCAPVCC);
  display.dim(true);
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.clearDisplay();
  display.display();
}

