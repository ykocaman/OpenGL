PShape country;
PShape shape;
City[] cities;
City hoverCity;
int maxStudent;
int maxPopulation;
String message;

void setup() {
  size(1052, 680);

  country = loadShape("tr.svg");
  cities = new City[81];
  int i = 1;
  for(City city : cities){
    city = new City(i);
    city.setShape(country.getChild(city.getShapeId()));
    
    city.setPopulation((int)random(1000,100000));
    if(city.getPopulation() > maxPopulation)
        maxPopulation =city.getPopulation();
    
    city.setStudent((int)random(500,5000));
    if(city.getStudent() > city.getPopulation())
      city.setStudent(city.getPopulation());
      
    if(city.getStudent() > maxStudent)
        maxStudent =city.getStudent();
        
    cities[i-1] = city;
    i++;
  }
  
  println("maxPopulation:" + maxPopulation);
  println("maxStudent:" + maxStudent);
  
  textSize(14);
     smooth();
     noStroke();
} 

void draw(){
  background(0);

  country.disableStyle();
  fill(125);
  shape(country);
   
  hoverCity = null;
  for(City city : cities){
      shape = city.getShape();
      
      shape.disableStyle();
      fill(255,0,0,(int)(float)city.getPopulation() / (float)maxPopulation * 255);
      shape(shape); 

      fill(255,150);
      PVector vector = city.getCenterPoints();
      int studentPercentege = (int)((float)city.getStudent() / (float)maxStudent * 25);
      ellipse (vector.x,vector.y,studentPercentege,studentPercentege);
      
      if(shape.contains(mouseX,mouseY)){
        hoverCity = city;
        if(mousePressed){
          background(0);
          fill(255,50); 
          shape(hoverCity.getShape());
          message = hoverCity.getName() + "\nNüfus : " + hoverCity.getPopulation() + "\nÖğrenci: "+ hoverCity.getStudent();
          fill(255);   
          textSize(26);
          text(message,width/3,50); 
          return;
        }
      }
  }
  
  if(!mousePressed && hoverCity != null){
      fill(255,50);
      shape(hoverCity.getShape()); 
      message = hoverCity.getName() + "\nNüfus : " + hoverCity.getPopulation() + "\nÖğrenci: "+ hoverCity.getStudent();
      fill(0,150);

      rect(mouseX-60,mouseY + 10,130,70,10);
      fill(255);   
      textSize(14);
      text(message,mouseX-50,mouseY+30); 
  }
  
  
}
